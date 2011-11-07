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
            ;

            boost::program_options::positional_options_description position_arguments;
            position_arguments.add( "ip-address", 1 );

            boost::program_options::variables_map variables_map;
            boost::program_options::store( boost::program_options::parse_command_line(argc, argv, desc).positional(position_arguments).run(), variables_map );
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
    string *GeoIpClient::getLocationByIp( const string ip_address ){

        //convert string to ip integer        
        unsigned int_ip_address = 1234U;

        return this->getLocationByIp( int_ip_address );

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
    string *GeoIpClient::getLocationByIp( unsigned ip_address ){

        typedef struct sockaddr socket_address;

        /* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
           kernels still #define it as 5, while actually supporting many more */
        #define	LISTENQ		1024	/* 2nd argument to listen() */

        /* Miscellaneous constants */
        #define	MAXLINE		4096	/* max text line length */
        #define	BUFFSIZE	8192	/* buffer size for reads and writes */

        ip_address = ip_address;

        int					sockfd, n;
        char				recvline[MAXLINE + 1];
        struct sockaddr_in	servaddr;

        if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            cout << "socket error";
            return NULL;
        }

        bzero( &servaddr, sizeof(servaddr) );
        servaddr.sin_family = AF_INET;
        servaddr.sin_port   = htons( 3600 );	/* daytime server */
        if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0 ){
            cout << "inet_pton error for %s", argv[1] << endl;
            return NULL;
        }


        if( connect(sockfd, (socket_address*) &servaddr, sizeof(servaddr)) < 0 ){
            cout << "connect error" << endl;
            return NULL;
        }


        while( (n = read(sockfd, recvline, MAXLINE)) > 0 ){
            recvline[n] = 0;	/* null terminate */
            if( fputs(recvline, stdout) == EOF ){
                cout << "fputs error" << endl;
                return NULL;
            }

        }
        if( n < 0 ){
            cout << "Read Error" << endl;
        }

        string *response = new string( recvline );

        return response;


    }



}
