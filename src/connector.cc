#include "connector.h"
#include <cstring>
#include <string.h>

namespace Altumo{

    pthread_mutex_t number_of_workers_mutex = PTHREAD_MUTEX_INITIALIZER;
    static int number_of_workers = 0;

    Connector::Connector()
        : connected(false)
    {
        //statement_threads = vector< pthread_t* >();

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
    void Connector::executeStatement( const string statement_str, bool asynchronous ){


        if( asynchronous ){

            while( number_of_workers > 10 ){

            }

            string *statement_string = new string( statement_str );

            pthread_t thread;
            int error = pthread_create( &thread, NULL, executeStatementThread, (void *) statement_string );

            if( error != 0 ){

                //error (switch to synchronous)
                    delete statement_string;
                    if( !connected ){
                        connect();
                    }
                    this->statement->execute( statement_str );

            }else{

                this->statement_threads.push_back( &thread );

            }

        }else{

            if( !connected ){
                connect();
            }
            this->statement->execute( statement_str );

        }

    }


    void Connector::waitForConnectionsToClose(){

        vector< pthread_t* >::iterator iterator;

        for(
            iterator = this->statement_threads.begin();
            iterator < this->statement_threads.end();
            iterator++
        ){
            pthread_join( **iterator, NULL );
        }

    }


    Connector::~Connector(){

        this->waitForConnectionsToClose();
        connected = false;

    }



    bool Connector::hasOpenConnections(){

        if( number_of_workers > 0 ){
            return true;
        }else{
            return false;
        }

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

        pthread_mutex_lock( &number_of_workers_mutex );
            number_of_workers++;
            cout << endl << "Running thread " << pthread_self() << ". Workers: " << number_of_workers << ". Statement size: " << statement_str->length() << endl;
            sql::Driver *driver = sql::mysql::get_driver_instance();
            driver->threadInit();
            Connector *connector = new Connector();
        pthread_mutex_unlock( &number_of_workers_mutex );

            connector->executeStatement( *statement_str, false );

        pthread_mutex_lock( &number_of_workers_mutex );
            driver->threadEnd();

            delete statement_str;
            delete connector;
            //delete driver;

            number_of_workers--;

        pthread_mutex_unlock( &number_of_workers_mutex );

        return NULL;

    }





}
