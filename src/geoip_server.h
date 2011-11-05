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


    using namespace std;

    namespace Altumo{

        class GeoIpServer{

            public:
                GeoIpServer();
                ~GeoIpServer();
                int setProgramOptions( int argc, char** argv );
                void loadData();

            protected:
                void loadLocationsFile();
                void loadBlocksFile();

                string blocks_filename;
                string locations_filename;
                map< unsigned long int, Location* > address_table;
                map< int, Location* > locations_table;

        };

    }

#endif //GEOIP_SERVER_H
