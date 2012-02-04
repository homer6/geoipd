
#include "city.h"


namespace Altumo{

    //unsigned City::next_id = 0;

    std::ostream& operator<<( std::ostream &os, const City &city ){
        os << "City(" << city.name << ")" ;
        return os;
    }

}

