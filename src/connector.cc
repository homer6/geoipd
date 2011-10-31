#include "connector.h"
#include <cstring>

namespace Altumo{


    Connector::Connector()
        : connected(false)
    {



    }


    /**
    * Creates a new connection to the specified MySQL server and selects the
    * Database. Does nothing if already connected.
    *
    */
    void Connector::connect(){

        if( !connected ){

            string url( MYSQL_HOSTNAME );
            const string username( MYSQL_USERNAME );
            const string password( MYSQL_PASSWORD );
            const string database( MYSQL_DATABASE );


            //connect
                sql::Driver * driver = sql::mysql::get_driver_instance();

                //use the driver to get a connection
                std::auto_ptr< sql::Connection > connection( driver->connect(url, username, password) );

                /* Creating a "simple" statement - "simple" = not a prepared statement */
                std::auto_ptr< sql::Statement > tmp_statement( connection->createStatement() );

                //select the current database
                tmp_statement->execute( "USE " + database );

                statement = tmp_statement;
                connected = true;

        }

    }



    /**
    * Selects a number of rows and outputs to stdout.
    *
    */
    void Connector::test(){

        connect();

        size_t row;

        ResultSet result_set( statement->executeQuery( "SELECT * FROM my_table LIMIT 10" ) );

        //cout << "result_set->rowsCount() = " << result_set->rowsCount() << endl;

        row = 0;
        while( result_set->next() ){
            cout << "#\t\t Fetching row " << row << "\t";
            cout << "id = " << result_set->getInt("id");
            cout << ", title = '" << result_set->getString("title") << "'";
            cout << ", slug = '" << result_set->getString("slug") << "'";
            cout << endl;
            row++;
        }

    }


    /**
    * Runs and SQL query and returns the result.
    *
    */
    ResultSet Connector::executeQuery( const string query ){

        if( !connected ){
            connect();
        }

        ResultSet result_set( statement->executeQuery( query ) );

        return result_set;

    }


    /**
    * Runs and SQL query and returns the result.
    *
    */
    void Connector::executeStatement( const string statement_str ){

        if( !connected ){
            connect();
        }

        statement->execute( statement_str );

    }


    Connector::~Connector(){

        connected = false;

    }


    /**
    * Escapes a string when being passed as input.
    *
    */
    string escapeString( const char *pStr ){

        string result;
        while( *pStr ){
            if( strchr( "\"'\r\n\t", *pStr ) ){
                //bad character, skip
            }else{
                result.push_back( *pStr );
            }
            ++pStr;
        }
        return result;

    }


}
