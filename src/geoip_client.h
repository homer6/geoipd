#ifndef GEOIP_CLIENT_H
#define GEOIP_CLIENT_H

    #include <stdio.h>
    #include <string>

    #include <boost/program_options.hpp>

    #include "location.h"

    using namespace std;

    namespace Altumo{

        class GeoIpClient{

            public:
                GeoIpClient();
                ~GeoIpClient();
                int setProgramOptions( int argc, char** argv );
                Location *getLocationByIp( const string ip_address );
                Location *getLocationByIp( unsigned ip_address );

            protected:
                string ip_address;

        };

    }

#endif //GEOIP_CLIENT_H
