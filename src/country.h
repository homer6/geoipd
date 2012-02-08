#ifndef COUNTRY_H
#define COUNTRY_H

    #include <string>
    #include <map>

    #include "state.h"


namespace Altumo{

    class Country{

        public:
            Country( std::string code );
            ~Country();

            std::string code;
            StateMap states;

    };

    typedef map< std::string, Country > CountryMap;
    typedef pair< std::string, Country > CountryMapPair;

}



#endif //COUNTRY_H
