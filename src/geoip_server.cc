#include "geoip_server.h"

#include <vector>
#include <set>
#include <sys/time.h>
#include <math.h>
#include <forward_list>

using namespace std;

namespace Altumo{

    /**
    * Contructor. Creates the database connection.
    *
    */
    GeoIpServer::GeoIpServer(){

        this->address_table = new LocationMap;
        this->locations_table = new LocationMap;

        this->cities_table = new std::forward_list< City* >;
        this->cities_index = new SearchTrie< City >;

    }


    /**
    * Destructor. Cleans up allocated memory.
    *
    */
    GeoIpServer::~GeoIpServer(){

        //delete all of the locations
            for(
                LocationMap::iterator iterator = locations_table->begin();
                iterator != locations_table->end();
                iterator++
            ){
                delete iterator->second;
            }

        delete locations_table;
        delete address_table;
        delete cities_index;
        delete cities_table;

    }


    /**
    * Parses the command line arguments.
    *
    */
    int GeoIpServer::setProgramOptions( int argc, char** argv ){



        //handle the command line arguments
            boost::program_options::options_description desc("Options");
            desc.add_options()
                ( "help", "produce help message" )
                ( "locations-file", boost::program_options::value<string>(), "The locations csv eg. GeoIPCity-134-Location.csv" )
                ( "blocks-file", boost::program_options::value<string>(), "The blocks csv eg. GeoIPCity-134-Blocks.csv" )
                ( "cities-file", boost::program_options::value<string>(), "The cities csv eg. worldcitiespop.txt" )
            ;

            boost::program_options::variables_map variables_map;
            boost::program_options::store( boost::program_options::parse_command_line(argc, argv, desc), variables_map );
            boost::program_options::notify( variables_map );

            if( variables_map.count("help") ){
                cout << desc << "\n";
                return 1;
            }

            /*
            if( variables_map.count("locations-file") ){
                this->locations_filename = variables_map["locations-file"].as<string>();
            }else{
                cout << "The locations file is required.\n";
                cout << desc << "\n";
                return 1;
            }


            if( variables_map.count("blocks-file") ){
                this->blocks_filename = variables_map["blocks-file"].as<string>();
            }else{
                cout << "The blocks file is required.\n";
                cout << desc << "\n";
                return 1;
            }
            */


            if( variables_map.count("cities-file") ){
                this->cities_filename = variables_map["cities-file"].as<string>();
            }else{
                cout << "The cities file is required.\n";
                cout << desc << "\n";
                return 1;
            }

            return 0;

    }


    /**
    * Loads everything needed for the server to run into memory.
    *
    */
    void GeoIpServer::loadData(){

        //this->loadLocationsFile();
        //this->loadBlocksFile();
        this->loadCitiesFile();

    }




    /**
    * Loads the GeoIPCity-134-Location.csv file, provided by MaxMind,
    * to memory.
    *
    */
    void GeoIpServer::loadCitiesFile(){

        //declare and initialize local variables
            boost::cmatch result;
            string line;

        //import the cities section
            ifstream cities_file( this->cities_filename.c_str() );
            const boost::regex cities_pattern( "(..),(.*?),(.*?),(.*?),(.*?),(.*?),(.*?)" );
            int number_of_imported_records = 0;
            int number_of_skipped_records = 0;

            while( !cities_file.eof() ){

                //if( number_of_imported_records >= 100000 ){
                //    break;
                // }

                getline( cities_file, line );

                if( boost::regex_match(line.c_str(), result, cities_pattern) ){

                    number_of_imported_records++;

                    City *new_city = new City(
                        //result[1].str(),
                        result[3].str()
                    );

                    /*
                    location_id = std::strtoul( result[1].str().c_str(), NULL, 0 );
                    if( location_id == ULONG_MAX ){
                        cout << "Location line: " << line << " id is out of range. Skipping." << endl;
                        number_of_skipped_records++;
                        continue;
                    }
                    */

                    //cities_table->insert( new_city );
                    cities_index->addWord( result[2].str(), new_city );
                    //locations_table->insert( LocationMapPair( location_id, new_location ) );

                }else{

                    number_of_skipped_records++;

                }

            }

            cities_index->debug();

            cout << endl << number_of_imported_records << " city records imported.";
            cout << endl << number_of_skipped_records << " city records skipped." << endl;
            flush( cout );
            cities_file.close();

            this->benchmarkSearchTrie();
            while( 1 );

    }



    /**
    * Loads the GeoIPCity-134-Location.csv file, provided by MaxMind,
    * to memory.
    *
    */
    void GeoIpServer::loadLocationsFile(){

        //declare and initialize local variables
            boost::cmatch result;
            string line;
            unsigned long location_id;

        //import the locations section
            ifstream locations_file( this->locations_filename.c_str() );
            const boost::regex locations_pattern( "(\\d+),\"(..)\",\"(.*?)\",\"(.*?)\",\"(.*?)\",(.*?),(.*?),(\\d*),(\\d*)" );
            int number_of_imported_records = 0;
            int number_of_skipped_records = 0;

            while( !locations_file.eof() ){

                getline( locations_file, line );

                if( boost::regex_match(line.c_str(), result, locations_pattern) ){

                    number_of_imported_records++;

                    Location *new_location = new Location(
                        result[2].str(),
                        result[3].str(),
                        result[4].str(),
                        result[5].str(),
                        result[6].str(),
                        result[7].str(),
                        result[8].str(),
                        result[9].str()
                    );

                    location_id = std::strtoul( result[1].str().c_str(), NULL, 0 );
                    if( location_id == ULONG_MAX ){
                        cout << "Location line: " << line << " id is out of range. Skipping." << endl;
                        number_of_skipped_records++;
                        continue;
                    }

                    locations_table->insert( LocationMapPair( location_id, new_location ) );

                }else{

                    number_of_skipped_records++;

                }

            }

            cout << endl << number_of_imported_records << " locations records imported.";
            cout << endl << number_of_skipped_records << " locations records skipped." << endl;
            flush( cout );
            locations_file.close();

    }



    /**
    * Imports the GeoIPCity-134-Blocks.csv file, provided by MaxMind,
    * to memory.
    *
    */
    void GeoIpServer::loadBlocksFile(){

        //declare and initialize local variables
            int number_of_imported_records = 0;
            int number_of_skipped_records = 0;
            boost::cmatch result;
            string line;
            unsigned long location_id;
            unsigned long ip_address;
            LocationMap::iterator iterator;

        //import the blocks section
            ifstream blocks_file( this->blocks_filename.c_str() );
            const boost::regex blocks_pattern( "\"(\\d+)\",\"(\\d+)\",\"(\\d+)\"" );

            while( !blocks_file.eof() ){

                getline( blocks_file, line, '\n' );

                if( boost::regex_match(line.c_str(), result, blocks_pattern) ){

                    number_of_imported_records++;

                    location_id = std::strtoul( result[3].str().c_str(), NULL, 0 );
                    if( location_id == ULONG_MAX ){
                        cout << "Blocks line: " << line << " location_id is out of range. Skipping." << endl;
                        number_of_skipped_records++;
                        continue;
                    }

                    iterator = locations_table->find( location_id );
                    if( iterator == locations_table->end() ){
                        cout << "Blocks line: " << line << " location_id was not found in the location map. Skipping." << endl;
                        number_of_skipped_records++;
                        continue;
                    }

                    ip_address = std::strtoul( result[1].str().c_str(), NULL, 0 );
                    if( ip_address == ULONG_MAX ){
                        cout << "Blocks line: " << line << " ip_address is out of range. Skipping." << endl;
                        number_of_skipped_records++;
                        continue;
                    }

                    address_table->insert( LocationMapPair( ip_address, iterator->second ) );

                }else{

                    number_of_skipped_records++;

                }

            }

            cout << endl << number_of_imported_records << " block records imported.";
            cout << endl << number_of_skipped_records << " block records skipped." << endl;
            flush( cout );
            blocks_file.close();


    }



    /**
    * Gets a location by IP.
    *
    * Returns NULL if not found.
    *
    */
    Location *GeoIpServer::getLocationByIp( const string ip_address ){

        //convert the IP address string to an integer
            struct sockaddr_in sa;
            int result = inet_pton( AF_INET, ip_address.c_str(), &(sa.sin_addr) );

        if( result == 1 ){
            return this->getLocationByIp( ntohl(sa.sin_addr.s_addr) );
        }else{
            return NULL;
        }

    }


    /**
    * Gets a location by IP.
    *
    * Returns NULL if not found.
    *
    */
    Location *GeoIpServer::getLocationByIp( unsigned long ip_address ){

        LocationMap::iterator iterator;

        iterator = address_table->upper_bound( ip_address );

        if( iterator != address_table->end() ){

            if( iterator == address_table->begin() ){
                return NULL;
            }else{
                iterator--;
                return iterator->second;
            }

        }else{

            return NULL;

        }


    }


    /**
    *
    *
    */
    void GeoIpServer::listenForConnections(){

        boost::asio::io_service io_service;
        short port = 3600;

        tcp::endpoint localhost( boost::asio::ip::address::from_string("127.0.0.1"), port );

        tcp::acceptor acceptor( io_service, localhost );
        for( ;; ){
            socket_ptr sock(new tcp::socket(io_service));
            acceptor.accept(*sock);
            boost::thread thread( boost::bind(&GeoIpServer::handleSession, this ,sock) );
        }

    }


    void GeoIpServer::handleSession( socket_ptr sock ){

        string *request;
        Location *location;
        string *response;

        try{

            for (;;){
                char data[max_length];

                boost::system::error_code error;
                size_t length = sock->read_some( boost::asio::buffer(data), error );
                if( error == boost::asio::error::eof ){
                    break; // Connection closed cleanly by peer.
                }else if( error ){
                    throw boost::system::system_error( error ); // Some other error.
                }

                request = new string( data, length );

                location = this->getLocationByIp( *request );

                if( location == NULL ){
                    response = new string( "{}" );
                }else{
                    stringstream response_stream;
                    response_stream << "{"
                                 << "\"city\":\"" << location->city << "\","
                                 << "\"region\":\"" << location->region << "\","
                                 << "\"country\":\"" << location->country << "\","
                                 << "\"latitude\":\"" << location->latitude << "\","
                                 << "\"longitude\":\"" << location->longitude << "\","
                                 << "\"area_code\":\"" << location->area_code << "\","
                                 << "\"metro_code\":\"" << location->metro_code << "\","
                                 << "\"postal_code\":\"" << location->postal_code << "\""
                              << "}";
                    response = new string( response_stream.str() );
                }

                memset( data, 0, max_length );
                strncpy( data, response->c_str(), response->length() );

                boost::asio::write( *sock, boost::asio::buffer(data, max_length) );

                delete request;
                delete response;

                //DO NOT DELETE THE LOCATION

            }

        }catch( std::exception& e ){

            std::cerr << "Exception in thread: " << e.what() << "\n";

        }

    }



    void GeoIpServer::benchmarkSearchTrie(){


        srand( time(NULL) );
        timeval time_start, time_stop, time_difference;
        double query_time_in_seconds;
        double total_query_times = 0.0;
        vector< double > query_times;

            int x = 0;
            std::forward_list< City* >* results;

            for( ; x < 5000000; x++ ){

                gettimeofday( &time_start, NULL );

                    results = new std::forward_list< City* >;
                    this->cities_index->search( std::string("andor"), results );
                    delete results;

                gettimeofday( &time_stop, NULL );

                timersub( &time_stop, &time_start, &time_difference );
                query_time_in_seconds = time_difference.tv_sec + time_difference.tv_usec/1000000.0;

                query_times.push_back( query_time_in_seconds );
                total_query_times += query_time_in_seconds;

            }

            cout.precision(12);

            double mean = total_query_times / x;
            double sum_of_average_squared_deviation = 0.0;

            vector< double >::iterator iterator;
            for( iterator = query_times.begin(); iterator < query_times.end(); iterator++ ){
                //cout << (*iterator) << endl;
                sum_of_average_squared_deviation += (double) pow( (*iterator) - mean, 2 );
            }

            double variance = sum_of_average_squared_deviation / (double)query_times.size();
            double standard_deviation = (double) sqrt( variance );

            cout << "For " << x << " queries:" << endl;

            cout << "Sum: " << total_query_times << "s" << endl;
            cout << "Mean: " << mean << "s" << endl;
            cout << "Variance: " << variance << "s" << endl;
            cout << "Standard Deviation: " << standard_deviation << "s" << endl;


        //search_trie.debug();

        cout << "size of SearchTrie< City >: " << sizeof( SearchTrie< City > ) << endl;
        cout << "size of SearchTrieNode< City >: " << sizeof( SearchTrieNode< City > ) << endl;
        cout << "size of std::map< char, City >: " << sizeof( std::map< char, City > ) << endl;
        cout << "size of std::list< SearchTrieNode<City>::SearchTrieNodePointer >: " << sizeof( std::list< SearchTrieNode<City>::SearchTrieNodePointer > ) << endl;
        cout << "size of std::forward_list< SearchTrieNode<City>::SearchTrieNodePointer >: " << sizeof( std::forward_list< SearchTrieNode<City>::SearchTrieNodePointer > ) << endl;
        cout << "size of std::vector< SearchTrieNode<City>::SearchTrieNodePointer >: " << sizeof( std::vector< SearchTrieNode<City>::SearchTrieNodePointer > ) << endl;
        cout << "size of std::vector< char >: " << sizeof( std::vector<char> ) << endl;
        cout << "size of std::set< SearchTrieNode<City>::SearchTrieNodePointer >: " << sizeof( std::set< SearchTrieNode<City>::SearchTrieNodePointer > ) << endl;
        cout << "size of std::set< char >: " << sizeof( std::set<char> ) << endl;


    }


}
