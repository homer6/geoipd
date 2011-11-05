#include <sys/time.h>

#include <vector>
#include <math.h>

#include "geoip_server.h"


using namespace Altumo;
using namespace std;


int main( int argc, char** argv ){

    GeoIpServer *server = new GeoIpServer();
    int error_number = server->setProgramOptions( argc, argv );
    if( error_number > 0 ){
        return error_number;
    }


    //string ip_address_to_find = "70.78.216.29";

    /*
    vector< string > test_addresses;
    vector< string >::interator test_addresses_iterator;
    test_addresses.push_back( "70.78.216.29" );
    test_addresses.push_back( "70.70.137.237" );


    vector< unsigned long > test_addresses;
    vector< unsigned long >::iterator test_addresses_iterator;
    test_addresses.push_back( 1179572253UL );
    test_addresses.push_back( 1179027949UL );
    test_addresses.push_back( 17367340UL );
    test_addresses.push_back( 17367552UL );
    test_addresses.push_back( 17367680UL );
*/



/*
IP: 17367040 - Bukit Mertajam, 09, MY
IP: 17367552 - Jitra, 02, MY
IP: 17367680 - Bukit Mertajam, 09, MY
IP: 17367682 - Kulim, 02, MY
IP: 17367683 - Bukit Mertajam, 09, MY
IP: 17367744 - Jitra, 02, MY
IP: 17367767 - Alor Setar, 02, MY
IP: 17367768 - Jitra, 02, MY
IP: 17367808 - Bukit Mertajam, 09, MY
IP: 17368064 - Kuala Lumpur, 14, MY



"17367040","17367551","125223"
"17367552","17367679","108279"
"17367680","17367681","125223"
"17367682","17367682","108270"
"17367683","17367743","125223"
"17367744","17367766","108279"
"17367767","17367767","108291"
"17367768","17367807","108279"
"17367808","17368063","125223"
"17368064","17368127","23797"


*/



    Location *location_result;

    server->loadData();


    unsigned long int_ip_query = 1179572253UL;


    timeval time_start, time_stop, time_difference;
    double query_time_in_seconds;
    double total_query_times = 0.0;
    vector< double > query_times;

    srand( time(NULL) );

    //cout << "Looking for IP: " << ip_address_to_find << endl;
    int x = 0;

    for( ; x < 5000; x++ ){
    //for( test_addresses_iterator = test_addresses.begin(); test_addresses_iterator < test_addresses.end(); test_addresses_iterator++ ){

        //int_ip_query = *test_addresses_iterator;

        gettimeofday( &time_start, NULL );
        location_result = server->getLocationByIp( int_ip_query );
        gettimeofday( &time_stop, NULL );

        /*
        if( location_result == NULL ){
            cout << "Location not found." << endl;
        }else{
            cout << "-----------------------------" << endl;
            cout << "City found: " << location_result->city << endl;
            cout << "Region: " << location_result->region << endl;
            cout << "Country: " << location_result->country << endl;
        }
        */

        timersub( &time_stop, &time_start, &time_difference );
        query_time_in_seconds = time_difference.tv_sec + time_difference.tv_usec/1000000.0;


        //cout.precision(15);
        //cout << "Query time: " << fixed << query_time_in_seconds << " seconds" << endl;

        query_times.push_back( query_time_in_seconds );
        total_query_times += query_time_in_seconds;

        int_ip_query = rand() % 4294967290UL + 1;

    }

    cout.precision(12);

    double mean = total_query_times / x;
    double sum_of_average_squared_deviation = 0.0;

    vector< double >::iterator iterator;
    for( iterator = query_times.begin(); iterator < query_times.end(); iterator++ ){
        cout << (*iterator) << endl;
        sum_of_average_squared_deviation += (double) pow( (*iterator) - mean, 2 );
    }

    double variance = sum_of_average_squared_deviation / (double)query_times.size();
    double standard_deviation = (double) sqrt( variance );

    cout << "For " << x << " queries:" << endl;

    cout << "Sum: " << total_query_times << "s" << endl;
    cout << "Mean: " << mean << "s" << endl;
    cout << "Variance: " << variance << "s" << endl;
    cout << "Standard Deviation: " << standard_deviation << "s" << endl;


    while( 1 ){
        sleep( 10 );
    }
    delete server;

    return 0;

}



