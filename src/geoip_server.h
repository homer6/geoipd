#ifndef GEOIP_SERVER_H
#define GEOIP_SERVER_H

    #include <sys/time.h>
    #include <unistd.h>
    #include <err.h>
    #include <stdio.h>
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <stdlib.h>
    #include <map>


    #include <boost/program_options.hpp>
    #include <boost/regex.hpp>

    #include "location.h"

    using namespace std;

    namespace Altumo{

        class GeoIpServer{

            public:
                GeoIpServer();
                ~GeoIpServer();
                int setProgramOptions( int argc, char** argv );
                void loadData();
                Location *getLocationByIp( const string ip_address );
                Location *getLocationByIp( unsigned long ip_address );

                //static unsigned int getIpAddressFromString( string ip_address );

            protected:
                void loadLocationsFile();
                void loadBlocksFile();

                string blocks_filename;
                string locations_filename;
                map< unsigned long, Location* > *address_table; //ip address as int
                map< unsigned long, Location* > *locations_table; //location id as int

        };

    }

#endif //GEOIP_SERVER_H
