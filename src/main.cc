#include <sys/time.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include <boost/program_options.hpp>
#include <boost/regex.hpp>

#include "connector.h"


using namespace std;
using namespace Altumo;


int main( int argc, char** argv ){


    string location_filename;
    string blocks_filename;


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
            location_filename = variables_map["locations-file"].as<string>();
        }else{
            cout << "The locations file is required.\n";
            cout << desc << "\n";
            return 1;
        }

        if( variables_map.count("blocks-file") ){
            blocks_filename = variables_map["blocks-file"].as<string>();
        }else{
            cout << "The blocks file is required.\n";
            cout << desc << "\n";
            return 1;
        }

    //declare variables used in both files
        string insert_query, blocks_insert_query, locations_insert_query;
        bool first = true;
        int x = 0;
        boost::cmatch result;
        string line;
        Connector *connector = new Connector();

    //drop the existing table and build the new ones
        connector->executeStatement( "DROP TABLE IF EXISTS `geo_ip_location`" );
        string current_query =
            "CREATE TABLE `geo_ip_location`("
                    "`id` INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,"
                    "`country` CHAR(2) NOT NULL,"
                    "`region` CHAR(2),"
                    "`city` VARCHAR(32),"
                    "`postal_code` VARCHAR(10),"
                    "`latitude` DECIMAL(18,12),"
                    "`longitude` DECIMAL(18,12),"
                    "`metro_code` SMALLINT,"
                    "`area_code` SMALLINT,"
                    "PRIMARY KEY (`id`),"
                    "INDEX `lat_long_index` ( `latitude`, `longitude` ),"
                    "INDEX `country_region_city_index` ( `country`, `region`, `city` ),"
                    "INDEX `postal_code_index` ( `postal_code` ),"
                    "INDEX `area_code_index` ( `area_code` )"
            ") ENGINE=MyISAM"
        ;
        connector->executeStatement( current_query );


        connector->executeStatement( "DROP TABLE IF EXISTS `geo_ip_block`" );
        current_query =
            "CREATE TABLE `geo_ip_block`("
                    "`start_ip` MEDIUMINT UNSIGNED NOT NULL,"
                    "`end_ip` MEDIUMINT UNSIGNED NOT NULL,"
                    "`location_id` INTEGER UNSIGNED NOT NULL,"
                    "INDEX `ip_range_index` ( `start_ip`, `end_ip` ),"
                    "INDEX `location_index` ( `location_id` )"
            ") ENGINE=MyISAM"
        ;
        connector->executeStatement( current_query );


    //import the locations section
        ifstream locations_file( location_filename.c_str() );
        const boost::regex locations_pattern( "(\\d+),\"(..)\",\"(..)\",\"(.*?)\",\"(.*?)\",(.*?),(.*?),(\\d+),(\\d+)" );
        x = 0;
        first = true;
        locations_insert_query = "INSERT INTO geo_ip_location( id, country, region, city, postal_code, latitude, longitude, metro_code, area_code ) VALUES ";
        insert_query = locations_insert_query;

        while( !locations_file.eof() ){

            getline( locations_file, line );
            if( boost::regex_match(line.c_str(), result, locations_pattern) ){

                x++;
                if( !first ){
                    insert_query += ",";
                }else{
                    first = false;
                }
                insert_query += " (" +
                              escapeString( result[1].str().c_str() ) +
                            ", \"" +
                              escapeString( result[2].str().c_str() ) +
                            "\", \"" +
                              escapeString( result[3].str().c_str() ) +
                            "\", \"" +
                              escapeString( result[4].str().c_str() ) +
                            "\", \"" +
                              escapeString( result[5].str().c_str() ) +
                            "\", " +
                              escapeString( result[6].str().c_str() ) +
                            ", " +
                              escapeString( result[7].str().c_str() ) +
                            ", " +
                              escapeString( result[8].str().c_str() ) +
                            ", " +
                              escapeString( result[9].str().c_str() ) +
                            ")";

                if( x % 10000 == 0 ){
                    connector->executeStatement( insert_query );
                    insert_query = locations_insert_query;
                    first = true;
                    cout << ".";
                    flush( cout );
                }
            }

        }
        connector->executeStatement( insert_query );
        insert_query = "";
        first = true;
        cout << ".";
        flush( cout );
        cout << endl << x << " locations records imported." << endl;
        locations_file.close();


    //import the blocks section
        ifstream blocks_file( blocks_filename.c_str() );
        const boost::regex blocks_pattern( "\"(\\d+)\",\"(\\d+)\",\"(\\d+)\"" );
        x = 0;
        first = true;
        blocks_insert_query = "INSERT INTO geo_ip_block( start_ip, end_ip, location_id ) VALUES ";
        insert_query = blocks_insert_query;

        while( !blocks_file.eof() ){

            getline( blocks_file, line );
            if( boost::regex_match(line.c_str(), result, blocks_pattern) ){
                x++;
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

                if( x % 10000 == 0 ){
                    connector->executeStatement( insert_query );
                    insert_query = blocks_insert_query;
                    first = true;
                    cout << ".";
                    flush( cout );
                }
            }

        }
        connector->executeStatement( insert_query );
        insert_query = "";
        first = true;
        cout << ".";
        flush( cout );
        cout << endl << x << " block records imported." << endl;
        blocks_file.close();


    delete connector;

    return 0;


}
