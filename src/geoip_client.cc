#include "geoip_client.h"


using namespace std;

namespace Altumo{

    /**
    * Contructor.
    *
    */
    GeoIpClient::GeoIpClient(){

    }


    /**
    * Destructor. Cleans up allocated memory.
    *
    */
    GeoIpClient::~GeoIpClient(){

    }


    /**
    * Parses the command line arguments.
    *
    */
    int GeoIpClient::setProgramOptions( int argc, char** argv ){

        //handle the command line arguments
            boost::program_options::options_description desc("Options");
            desc.add_options()
                ( "help", "produce help message" )
                ( "ip-address", boost::program_options::value<string>(), "The IP address to lookup." )
            ;

            boost::program_options::variables_map variables_map;
            boost::program_options::store( boost::program_options::parse_command_line(argc, argv, desc), variables_map );
            boost::program_options::notify( variables_map );

            if( variables_map.count("help") ){
                cout << desc << "\n";
                return 1;
            }

            if( variables_map.count("ip-address") ){
                this->ip_address = variables_map["ip-address"].as<string>();
            }else{
                cout << "The IP address is required.\n";
                cout << desc << "\n";
                return 1;
            }

            return 0;

    }





    /**
    * Gets a location by IP.
    *
    * Returns NULL if not found.
    *
    */
    Location *GeoIpClient::getLocationByIp( const string ip_address ){

        //convert string to ip integer        
        unsigned int_ip_address = 1234U;

        return this->getLocationByIp( int_ip_address );

    }


    /**
    * Gets a location by IP.
    *
    * Returns NULL if not found.
    *
    */
    Location *GeoIpClient::getLocationByIp( unsigned ip_address ){

        ip_address = ip_address;

        return NULL;


    }



}
