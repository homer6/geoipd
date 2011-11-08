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
                ( "host", boost::program_options::value<string>(), "The IP address of the server to connect to." )
                ( "ip-address", boost::program_options::value<string>(), "The IP address to look up." )
            ;

            boost::program_options::variables_map variables_map;
            boost::program_options::store( boost::program_options::parse_command_line(argc, argv, desc), variables_map );
            boost::program_options::notify( variables_map );

            if( variables_map.count("help") ){
                cout << "Usage: geoip --host=[geoip-server-ip] [ip-to-lookup]" << endl;
                cout << desc << "\n";
                return 1;
            }

            if( variables_map.count("host") ){
                this->host = variables_map["host"].as<string>();
            }else{
                cout << "The geoip-server-ip address is required.\n";
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
    * You must delete the string that is returned after use.
    *
    * Returns NULL if not found.
    *
    */
    string *GeoIpClient::getLocationByIp(){

        try{

            boost::asio::io_service io_service;

            tcp::resolver resolver( io_service );
            tcp::resolver::query query( tcp::v4(), this->host.c_str(), "3600" );
            tcp::resolver::iterator iterator = resolver.resolve( query );

            tcp::socket s( io_service );
            s.connect( *iterator );

            size_t request_length = this->ip_address.size();
            boost::asio::write( s, boost::asio::buffer(this->ip_address.c_str(), request_length) );

            //get the header
                char data[max_length];
                boost::asio::read( s, boost::asio::buffer(data, max_length) );

            string *response = new string( data );

            return response;

        }catch( std::exception& e ){

            string *response = new string( "{\"error\":\"" + string(e.what()) + "\"}" );
            return response;

        }

        return NULL;

    }



}
