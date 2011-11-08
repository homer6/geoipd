#include "../geoip_server.h"
#include <stdlib.h>

using namespace Altumo;
using namespace std;


int main( int argc, char** argv ){

    GeoIpServer *server = new GeoIpServer();
    int error_number = server->setProgramOptions( argc, argv );
    if( error_number > 0 ){
        return error_number;
    }

    cout << "Loading the server. This may take a minute." << endl;
    server->loadData();
    cout << "Ready." << endl;

    //start the service
    if( daemon(0,0) == -1 ){
        err( 1, NULL );
    }
    server->listenForConnections();


    delete server;

    return 0;

}



