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
    #include <list>

    #include <boost/program_options.hpp>
    #include <boost/regex.hpp>
    #include <boost/bind.hpp>
    #include <boost/smart_ptr.hpp>
    #include <boost/asio.hpp>
    #include <boost/thread.hpp>

    #include "location.h"
    #include "city.h"
    #include "search_trie.h"
    #include "country.h"


    using namespace std;
    using boost::asio::ip::tcp;

    namespace Altumo{

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

                void benchmarkSearchTrie();

                //static unsigned int getIpAddressFromString( string ip_address );

            protected:
                void loadStatesFile();
                void loadCitiesFile();
                void loadLocationsFile();
                void loadBlocksFile();
                void handleSession( socket_ptr sock );

                string states_filename;
                string cities_filename;
                string blocks_filename;
                string locations_filename;

                LocationMap *address_table; //ip address as int
                LocationMap *locations_table; //location id as int

                std::forward_list< City* > *cities_table; //a list of all cities for searching
                SearchTrie< City > *cities_index; //full text index of city names

                CountryMap *countries; //map of all countries (which have states)

        };




    }

#endif //GEOIP_SERVER_H
