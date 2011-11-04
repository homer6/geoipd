#ifndef CONNECTOR_H
#define CONNECTOR_H

    //required to connect
        #include "config.h"
        #include <iostream>
        #include <string>
        #include "mysql/driver/mysql_public_iface.h"
        #include <pthread.h>
        #include <vector>

    //required by Test
        #include <stdlib.h>
        #include <iostream>
        #include <sstream>
        #include <stdexcept>



    using namespace std;

namespace Altumo{

    typedef std::auto_ptr< sql::ResultSet > ResultSet;

    class Connector;

    class Connector{

        public:
            Connector();
            ~Connector();
            void test();
            ResultSet executeQuery( const string query );
            void executeStatement( const string &statement_str, bool asynchronous = true );
            void connect();
            void disconnect();
            void waitForConnectionsToClose();
            bool hasOpenConnections();
            int getNumberOfActiveConnections() const;

        protected:            
            bool connected;
            std::auto_ptr< sql::Statement > statement;
            std::auto_ptr< sql::Connection > connection;
            //std::vector< pthread_t* > statement_threads;

    };

    string escapeString( const char *pStr );
    void *executeStatementThread( void *arg );


}



#endif //CONNECTOR_H
