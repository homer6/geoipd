#include "connector.h"
#include <cstring>
#include <string.h>

namespace Altumo{

    pthread_mutex_t number_of_workers_mutex = PTHREAD_MUTEX_INITIALIZER;
    int number_of_workers = 0;

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
                std::auto_ptr< sql::Connection > tmp_connection( driver->connect(url, username, password) );

                /* Creating a "simple" statement - "simple" = not a prepared statement */
                std::auto_ptr< sql::Statement > tmp_statement( tmp_connection->createStatement() );

                //select the current database
                tmp_statement->execute( "USE " + database );

                statement = tmp_statement;
                connection = tmp_connection;
                connected = true;

        }

    }


    void Connector::disconnect(){

        connection->close();
        this->connected = false;

    }


    /**
    * Reads the max_allowed_packet variable from the remote server.
    *
    */
    unsigned long Connector::readMaxPacketSize(){

        //read the max packet size from remote server
            ResultSet result_set = this->executeQuery(
                "SHOW VARIABLES LIKE 'max_allowed_packet'"
            );

            while( result_set->next() ){
                return result_set->getUInt64( "Value" );
            }

            return 4000000;

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
    void Connector::executeStatement( const string &statement_str, bool asynchronous ){


        if( asynchronous ){

            while( number_of_workers >= 40 ){
                usleep( 500 );
            }

            string *statement_string = new string( statement_str );

            pthread_t thread;
            pthread_mutex_lock( &number_of_workers_mutex );
                number_of_workers++;
            pthread_mutex_unlock( &number_of_workers_mutex );
            int error = pthread_create( &thread, NULL, executeStatementThread, (void *) statement_string );

            if( error != 0 ){

                pthread_mutex_lock( &number_of_workers_mutex );
                    number_of_workers--;
                pthread_mutex_unlock( &number_of_workers_mutex );

                //error (switch to synchronous)
                    delete statement_string;
                    if( !connected ){
                        connect();
                    }
                    this->statement->execute( statement_str );

            }

        }else{

            if( !connected ){
                connect();
            }
            this->statement->execute( statement_str );

        }

    }


    Connector::~Connector(){

        this->disconnect();

    }



    bool Connector::hasOpenConnections(){

        if( number_of_workers > 0 ){
            return true;
        }else{
            return false;
        }

    }




    int Connector::getNumberOfActiveConnections() const{

        return number_of_workers;

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


    void *executeStatementThread( void *arg ){

        string *statement_str = ((string *) arg);
        int temp_workers;

        pthread_mutex_lock( &number_of_workers_mutex );
            sql::Driver *driver = sql::mysql::get_driver_instance();
            driver->threadInit();
            temp_workers = number_of_workers;
        pthread_mutex_unlock( &number_of_workers_mutex );

                cout << temp_workers << " - ";
                flush( cout );

                Connector *connector = new Connector();

                //usleep( 100000 );

                connector->executeStatement( *statement_str, false );



        pthread_mutex_lock( &number_of_workers_mutex );
            driver->threadEnd();
            number_of_workers--;
            temp_workers = number_of_workers;
        pthread_mutex_unlock( &number_of_workers_mutex );

        delete statement_str;
        delete connector;
        //delete driver;

        cout << temp_workers << " - ";
        flush( cout );

        return NULL;

    }





}
