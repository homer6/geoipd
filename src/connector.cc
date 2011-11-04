#include "connector.h"
#include <cstring>
#include <string.h>

namespace Altumo{

    pthread_mutex_t number_of_workers_mutex = PTHREAD_MUTEX_INITIALIZER;
    int number_of_workers = 0;

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
    void Connector::executeStatement( const string &statement_str, bool asynchronous ){


        if( asynchronous ){

            while( number_of_workers >= 140 ){

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

            }else{

                //pthread_join( thread, NULL );
                //this->statement_threads.push_back( &thread );

            }

        }else{

            if( !connected ){
                connect();
            }
            this->statement->execute( statement_str );

        }

    }


    void Connector::waitForConnectionsToClose(){

        /*
        vector< pthread_t* >::iterator iterator;

        for(
            iterator = this->statement_threads.begin();
            iterator < this->statement_threads.end();
            iterator++
        ){
            pthread_join( **iterator, NULL );
        }*/

    }


    Connector::~Connector(){

        //this->waitForConnectionsToClose();
        //connected = false;
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

        pthread_mutex_lock( &number_of_workers_mutex );
            sql::Driver *driver = sql::mysql::get_driver_instance();
            driver->threadInit();
        pthread_mutex_unlock( &number_of_workers_mutex );


                Connector *connector = new Connector();

                //sleep( 3 );

                connector->executeStatement( *statement_str, false );



        pthread_mutex_lock( &number_of_workers_mutex );
            driver->threadEnd();
            number_of_workers--;
        pthread_mutex_unlock( &number_of_workers_mutex );

        delete statement_str;
        delete connector;
        //delete driver;

        return NULL;

    }





}
