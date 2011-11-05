#include "location.h"


namespace Altumo{


    Location::Location( string country, string region, string city, string postal_code, string latitude, string longitude, string metro_code, string area_code )
        : country(country), region(region), city(city), postal_code(postal_code), latitude(latitude), longitude(longitude), metro_code(metro_code), area_code(area_code)
    {

    }


    Location::~Location(){

    }



}
