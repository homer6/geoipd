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

        this->loadLocationsFile();
        //this->loadBlocksFile();

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
            string metro_code;
            string area_code;

        //import the locations section
            ifstream locations_file( this->locations_filename.c_str() );
            const boost::regex locations_pattern( "(\\d+),\"(..)\",\"(..)\",\"(.*?)\",\"(.*?)\",(.*?),(.*?),(\\d*),(\\d*)" );
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

                    locations_table[ std::atoi(result[1].str().c_str()) ] = new_location;

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

        /*
        //declare and initialize local variables
            string insert_query, blocks_insert_query;
            bool first = true;
            int number_of_imported_records = 0;
            int number_of_skipped_records = 0;
            boost::cmatch result;
            string line;

        //import the blocks section
            ifstream blocks_file( this->blocks_filename.c_str() );
            const boost::regex blocks_pattern( "\"(\\d+)\",\"(\\d+)\",\"(\\d+)\"" );
            blocks_insert_query = "INSERT INTO geo_ip_block( start_ip, end_ip, location_id ) VALUES ";
            insert_query = blocks_insert_query;

            while( !blocks_file.eof() ){

                getline( blocks_file, line, '\n' );

                if( boost::regex_match(line.c_str(), result, blocks_pattern) ){

                    number_of_imported_records++;

                    if( !first ){
                        insert_query += ",";
                    }else{
                        first = false;
                    }
                    insert_query += " (" +
                                  escapeString( result[1].str().c_str() ) +
                                ", " +
                                  escapeString( result[2].str().c_str() ) +
                                ", " +
                                  escapeString( result[3].str().c_str() ) +
                                ")";

                    if( insert_query.length() > this->mysql_max_packet_size ){
                        this->connector->executeStatement( insert_query );
                        insert_query = blocks_insert_query;
                        first = true;
                    }

                }else{

                    number_of_skipped_records++;

                }

            }

            if( !first ){
                this->connector->executeStatement( insert_query );
            }

            first = true;
            cout << endl << number_of_imported_records << " block records imported.";
            cout << endl << number_of_skipped_records << " block records skipped." << endl;
            flush( cout );
            blocks_file.close();
            */

    }


}
