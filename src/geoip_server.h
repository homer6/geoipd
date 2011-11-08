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
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

    #include <boost/program_options.hpp>
    #include <boost/regex.hpp>

    #include "location.h"
    #include "common.h"

    using namespace std;

    namespace Altumo{

        typedef map< unsigned long, Location* > LocationMap;
        typedef pair< unsigned long, Location* > LocationMapPair;

        class GeoIpServer{

            public:
                GeoIpServer();
                ~GeoIpServer();
                int setProgramOptions( int argc, char** argv );
                void loadData();
                Location *getLocationByIp( const string ip_address );
                Location *getLocationByIp( unsigned long ip_address );
                void listenForConnections();

                //static unsigned int getIpAddressFromString( string ip_address );

            protected:
                void loadLocationsFile();
                void loadBlocksFile();

                string blocks_filename;
                string locations_filename;
                LocationMap *address_table; //ip address as int
                LocationMap *locations_table; //location id as int

        };

    }

#endif //GEOIP_SERVER_H
