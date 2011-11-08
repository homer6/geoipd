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

        //convert string to ip integer        
            struct sockaddr_in lookup_ip;
            bzero( &lookup_ip, sizeof(lookup_ip) );
            lookup_ip.sin_family = AF_INET;
            lookup_ip.sin_port = htons( 3600 );

        if( inet_pton(AF_INET, this->ip_address.c_str(), &lookup_ip.sin_addr) <= 0 ){
            cout << "Invalid IP to query: %s" << this->ip_address.c_str() << endl;
            return NULL;
        }
        uint32_t query_address = ntohl( lookup_ip.sin_addr.s_addr );

        return this->getLocationByIp( query_address );

    }


    /**
    * Gets a location by IP.
    *
    * Returns NULL if not found.
    *
    * You must delete the string that is returned after use.
    *
    *
    */
    string *GeoIpClient::getLocationByIp( uint32_t ip_to_lookup ){

        //create a socket to connect with
            typedef struct sockaddr socket_address;

            /* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
               kernels still #define it as 5, while actually supporting many more */
            #define	LISTENQ		1024	/* 2nd argument to listen() */

            /* Miscellaneous constants */
            #define	MAXLINE		4096	/* max text line length */
            #define	BUFFSIZE	8192	/* buffer size for reads and writes */

            int sockfd, number_of_bytes_read;
            char recvline[MAXLINE + 1];

            if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
                cout << "socket error";
                return NULL;
            }


        //convert server address from string to ip
            struct sockaddr_in servaddr;
            bzero( &servaddr, sizeof(servaddr) );
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons( 3600 );
            if( inet_pton(AF_INET, this->host.c_str(), &servaddr.sin_addr) <= 0 ){
                cout << "Invalid server IP: %s" << this->host.c_str() << endl;
                return NULL;
            }

        //connect
            if( connect(sockfd, (socket_address*) &servaddr, sizeof(servaddr)) < 0 ){
                cout << "Could not connect to server." << endl;
                return NULL;
            }

         //make query
            uint32_t network_ip_to_lookup = htonl( ip_to_lookup );
            char query_message[4];
            memcpy( &query_message, &network_ip_to_lookup, strlen(query_message) );
            //snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            writen( sockfd, query_message, strlen(query_message) );



         //read the response
            //number_of_bytes_read = read( sockfd, recvline, MAXLINE );
            number_of_bytes_read = 0;
            cout << "okay" << endl;
            //recvline[number_of_bytes_read] = 0;
            if( fputs(recvline, stdout) == EOF ){
                cout << "fputs error" << endl;
                return NULL;
            }

            if( number_of_bytes_read < 0 ){
                cout << "Read Error" << endl;
            }else{
                cout << number_of_bytes_read << " bytes read:" << endl;
                cout << recvline << endl;
            }

        //create new string to hold the response
            string *response = new string( recvline );

        return response;


    }



}
