//#include <pthread.h>
#include <stdio.h>

#include "geoip_importer.h"

using namespace Altumo;
using namespace std;

/*
void *print_message_function( void *arg ){

    char *thread_id = *((char **)arg);

    cout << "Sleep on thread " << thread_id << endl;
    sleep(2);

    cout << "Hello, I'm thread " << thread_id << endl;

    return NULL;

}
*/


int main( int argc, char** argv ){

    /*
    pthread_t thread1, thread2;
    int thread_id_1, thread_id_2;
    const char *message_1 = "1";
    const char *message_2 = "2";

    GeoipImporter *importer = new GeoipImporter();
    int error_number = importer->setProgramOptions( argc, argv );
    if( error_number > 0 ){
        return error_number;
    }


    thread_id_1 = pthread_create( &thread1, NULL, print_message_function, (void *) &message_1 );
    thread_id_2 = pthread_create( &thread2, NULL, print_message_function, (void *) &message_2 );

    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
    */


    GeoipImporter *importer = new GeoipImporter();
    int error_number = importer->setProgramOptions( argc, argv );
    if( error_number > 0 ){
        return error_number;
    }


    importer->clearExistingTables();
    importer->createNewTables();
    importer->importLocationsFile();
    while( !importer->readyToClose() ){
        usleep( 300000 );
    }

    importer->importBlocksFile();
    while( !importer->readyToClose() ){
        usleep( 300000 );
    }


    delete importer;

    return 0;

}
