#ifndef GEOIP_CLIENT_H
#define GEOIP_CLIENT_H

    #include <stdio.h>
    #include <string>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>

    #include <boost/program_options.hpp>
    #include <boost/bind.hpp>
    #include <boost/smart_ptr.hpp>
    #include <boost/asio.hpp>
    #include <boost/thread.hpp>

    #include "location.h"

    using namespace std;
    using boost::asio::ip::tcp;

    namespace Altumo{

        const int max_length = 1024;
        typedef boost::shared_ptr<tcp::socket> socket_ptr;

        class GeoIpClient{

            public:
                GeoIpClient();
                ~GeoIpClient();
                int setProgramOptions( int argc, char** argv );
                string *getLocationByIp();

            protected:
                string *getLocationByIp( uint32_t ip_to_lookup );

                string host;
                string ip_address;

        };

    }

#endif //GEOIP_CLIENT_H
