#include "../geoip_client.h"


using namespace Altumo;
using namespace std;


int main( int argc, char** argv ){

    GeoIpClient *client = new GeoIpClient();
    int error_number = client->setProgramOptions( argc, argv );
    if( error_number > 0 ){
        return error_number;
    }


    string *location_result;

    location_result = client->getLocationByIp();

    if( location_result == NULL ){
        cout << "{}" << endl;
    }else{
        cout << *location_result << endl;
        delete location_result;
    }

    delete client;

    return 0;

}



