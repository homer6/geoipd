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

    while( 1 ){
        sleep( 10 );
    }
    delete server;

    return 0;

}
