#include "geoip_server.h"


using namespace std;

namespace Altumo{

    /**
    * Contructor. Creates the database connection.
    *
    */
    GeoIpServer::GeoIpServer(){

        this->address_table = new LocationMap;
        this->locations_table = new LocationMap;

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
            ;

            boost::program_options::variables_map variables_map;
            boost::program_options::store( boost::program_options::parse_command_line(argc, argv, desc), variables_map );
            boost::program_options::notify( variables_map );

            if( variables_map.count("help") ){
                cout << desc << "\n";
                return 1;
            }

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

            return 0;

    }


    /**
    * Loads everything needed for the server to run into memory.
    *
    */
    void GeoIpServer::loadData(){

        this->loadLocationsFile();
        this->loadBlocksFile();

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

        tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
        for( ;; ){
            socket_ptr sock(new tcp::socket(io_service));
            a.accept(*sock);
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


}
