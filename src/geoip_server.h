#ifndef GEOIP_SERVER_H
#define GEOIP_SERVER_H

    #include <sys/time.h>
    #include <unistd.h>
    #include <err.h>
    #include <stdio.h>
    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <string>
    #include <stdlib.h>
    #include <map>
    #include <cstdlib>

    #include <boost/program_options.hpp>
    #include <boost/regex.hpp>
    #include <boost/bind.hpp>
    #include <boost/smart_ptr.hpp>
    #include <boost/asio.hpp>
    #include <boost/thread.hpp>

    #include "location.h"


    using namespace std;
    using boost::asio::ip::tcp;

    namespace Altumo{

        typedef map< unsigned long, Location* > LocationMap;
        typedef pair< unsigned long, Location* > LocationMapPair;

        const int max_length = 1024;
        typedef boost::shared_ptr<tcp::socket> socket_ptr;

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
                void handleSession( socket_ptr sock );

                string blocks_filename;
                string locations_filename;
                LocationMap *address_table; //ip address as int
                LocationMap *locations_table; //location id as int

        };




    }

#endif //GEOIP_SERVER_H
