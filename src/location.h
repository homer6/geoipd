#ifndef LOCATION_H
#define LOCATION_H

    //required to connect
        #include <iostream>
        #include <string>

    //required by Test
        #include <stdlib.h>
        #include <iostream>
        #include <sstream>
        #include <stdexcept>


    using namespace std;

namespace Altumo{

    typedef map< unsigned long, Location* > LocationMap;
    typedef pair< unsigned long, Location* > LocationMapPair;

    class Location{

        public:
            Location( string country, string region, string city, string postal_code, string latitude, string longitude, string metro_code, string area_code );
            ~Location();

            string country;
            string region;
            string city;
            string postal_code;
            string latitude;
            string longitude;
            string metro_code;
            string area_code;

    };


}



#endif //LOCATION_H
