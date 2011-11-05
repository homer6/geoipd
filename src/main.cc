#include "geoip_importer.h"


int main( int argc, char** argv ){

    Altumo::GeoipImporter *importer = new Altumo::GeoipImporter();
    int error_number = importer->setProgramOptions( argc, argv );
    if( error_number > 0 ){
        return error_number;
    }

    importer->clearExistingTables();
    importer->createNewTables();
    importer->importLocationsFile();
    importer->importBlocksFile();

    delete importer;

    return 0;

}
