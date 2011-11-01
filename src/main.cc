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


    cout << blocks_filename << endl;
    cout << location_filename << endl;


    string line;

    ifstream blocks_file( blocks_filename.c_str() );

    const boost::regex expression("\"(\\d+)\",\"(\\d+)\",\"(\\d+)\"");
    boost::cmatch result;
    //char *line_string;
//    size_t string_length;

    int x = 0;

    while( x < 5 ){
        x++;
        getline( blocks_file, line );
        //line_string = (char *) malloc( strlen(line.c_str()) + 1 );
        //line_string = strcpy( line_string, line.c_str() );
        if( boost::regex_match(line.c_str(), result, expression) ){
            cout << result[1] << " " << std::atoi( result[1].str().c_str() ) << endl;
        }
        //free( line_string );
    }

    blocks_file.close();


    return 0;

    Connector connector;

    connector.test();



}
