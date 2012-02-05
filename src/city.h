#ifndef CITY_H
#define CITY_H


    namespace Altumo{

        class City{

            public:
                City(
                    //std::string country_code_var,
                    std::string name_var
                ): name(name_var){

                }

                //std::string country_code;
                std::string name;  //latin 1
                //std::string region_code;
                //unsigned population;
                //float latitude;
                //float longitude;

                bool operator==( const City &other ) const {
                    return true;
                }

        };

        std::ostream& operator<<( std::ostream &os, const City &city );


    }


#endif //CITY_H
