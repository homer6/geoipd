#include "geoip_importer.h"

using namespace Altumo;
using namespace std;



int main( int argc, char** argv ){

    GeoipImporter *importer = new GeoipImporter();
    int error_number = importer->setProgramOptions( argc, argv );
    if( error_number > 0 ){
        return error_number;
    }

    importer->clearExistingTables();
    importer->createNewTables();
    importer->importLocationsFile();
    importer->importBlocksFile();

    while( !importer->readyToClose() ){
        usleep( 30000 );
    }
    delete importer;

    return 0;

}
