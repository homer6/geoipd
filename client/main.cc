#include "geoip_server.h"


using namespace Altumo;
using namespace std;


int main( int argc, char** argv ){

    GeoIpServer *server = new GeoIpServer();
    int error_number = server->setProgramOptions( argc, argv );
    if( error_number > 0 ){
        return error_number;
    }


    server->loadData();

    Location *location_result;

    location_result = server->getLocationByIp( 1179572253UL );

    if( location_result == NULL ){
        cout << "Location not found." << endl;
    }else{
        cout << "-----------------------------" << endl;
        cout << "City found: " << location_result->city << endl;
        cout << "Region: " << location_result->region << endl;
        cout << "Country: " << location_result->country << endl;
    }

    delete server;

    return 0;

}



