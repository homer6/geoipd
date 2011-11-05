#ifndef CONNECTOR_H
#define CONNECTOR_H

    //required to connect
        #include "config.h"
        #include <iostream>
        #include <string>
        #include "mysql/driver/mysql_public_iface.h"
        #include <pthread.h>

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
            bool hasOpenConnections();
            int getNumberOfActiveConnections() const;
            unsigned long readMaxPacketSize();

        protected:            
            bool connected;
            std::auto_ptr< sql::Statement > statement;
            std::auto_ptr< sql::Connection > connection;

    };

    string escapeString( const char *pStr );
    void *executeStatementThread( void *arg );


}



#endif //CONNECTOR_H
