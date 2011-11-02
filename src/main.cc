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
            return 1;
        }

        if( variables_map.count("blocks-file") ){
            blocks_filename = variables_map["blocks-file"].as<string>();
        }else{
            cout << "The blocks file is required.\n";
            return 1;
        }


    //write blocks file to database
        cout << blocks_filename << "... ";

        string line;
        ifstream blocks_file( blocks_filename.c_str() );

        const boost::regex blocks_pattern("\"(\\d+)\",\"(\\d+)\",\"(\\d+)\"");
        boost::cmatch result;

        int x = 0;
        while( 1  ){
            x++;
            getline( blocks_file, line );
            if( boost::regex_match(line.c_str(), result, blocks_pattern) ){
                cout << result[1] << " " << std::atoi( result[1].str().c_str() ) << endl;
            }
            if( x > 100 ) break;
        }
        blocks_file.close();

        cout << "[done]" << endl << x << " lines.";

    return 0;

    Connector connector;

    connector.test();



}
