#ifndef CONNECTOR_H
#define CONNECTOR_H

    //required to connect
        #include "config.h"
        #include <iostream>
        #include <string>
        #include "mysql/driver/mysql_public_iface.h"

    //required by Test
        #include <stdlib.h>
        #include <iostream>
        #include <sstream>
        #include <stdexcept>



    using namespace std;

namespace Altumo{

    typedef std::auto_ptr< sql::ResultSet > ResultSet;

    class Connector{

        public:
            Connector();
            ~Connector();
            void test();
            ResultSet executeQuery( const string query );
            void executeStatement( const string statement_str );
            void connect();

        protected:            
            bool connected;
            std::auto_ptr< sql::Statement > statement;

    };

    string escapeString( const char *pStr );

}



#endif //CONNECTOR_H
