#include <sys/time.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>

#include <boost/program_options.hpp>

#include "connector.h"


using namespace std;
using namespace Altumo;


int main( int argc, char** argv ){


    //handle the command line arguments
        boost::program_options::options_description desc("Options");
        desc.add_options()
            ( "help", "produce help message" )
            ( "frequency", boost::program_options::value<int>(), "The timer frequency, in milliseconds." )
            ( "command", boost::program_options::value<string>(), "The shell command that the cron will run every frequency." )
        ;

        boost::program_options::variables_map variables_map;
        boost::program_options::store( boost::program_options::parse_command_line(argc, argv, desc), variables_map );
        boost::program_options::notify( variables_map );

        if( variables_map.count("help") ){
            cout << desc << "\n";
            return 1;
        }

        if( variables_map.count("frequency") ){
            cout << "Frequency was set to " << variables_map["frequency"].as<int>() << ".\n";
        }else{
            cout << "Frequency was not set.\n";
            return 1;
        }

        if( variables_map.count("command") ){
            cout << "Command was set to " << variables_map["command"].as<string>() << ".\n";
        }else{
            cout << "Command was not set.\n";
            return 1;
        }

    Connector connector;

    connector.test();



}
