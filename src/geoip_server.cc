#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "geoip_server.h"


using namespace std;

namespace Altumo{

    /**
    * Contructor. Creates the database connection.
    *
    */
    GeoIpServer::GeoIpServer(){


    }


    /**
    * Destructor. Cleans up allocated memory.
    *
    */
    GeoIpServer::~GeoIpServer(){


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

        this->address_table = new map< unsigned long, Location* >;
        this->locations_table = new map< unsigned long, Location* >;

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

                    locations_table->insert( pair< unsigned long, Location* >( location_id, new_location ) );

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
            map< unsigned long, Location* >::iterator iterator;

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

                    address_table->insert( pair< unsigned long, Location* >( ip_address, iterator->second ) );

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

        //convert string to ip integer
        unsigned long int_ip_address = inet_addr( ip_address.c_str() );

        if( int_ip_address == INADDR_NONE || int_ip_address == INADDR_ANY ){
            cout << "Not an IP address: " << int_ip_address << endl;
            return NULL;
        }

        return this->getLocationByIp( int_ip_address );

    }


    /**
    * Gets a location by IP.
    *
    * Returns NULL if not found.
    *
    */
    Location *GeoIpServer::getLocationByIp( unsigned long ip_address ){

        map< unsigned long, Location* >::iterator iterator;

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



    /*
    static unsigned int GeoIpServer::getIpAddressFromString( string ip_address ){





    }
    */


}
