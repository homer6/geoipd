#ifndef SEARCH_TRIE_NODE_H
#define SEARCH_TRIE_NODE_H

    //#include <stdio.h>
    //#include <string>
    #include <map>
    #include <list>
    //#include <boost/regex.hpp>

    //#include "location.h"


    using namespace std;


    namespace Altumo{

        template <typename storage_type>
        class SearchTrieNode{

            public:
                static unsigned next_id;

                typedef SearchTrieNode< storage_type >* SearchTrieNodePointer;

                SearchTrieNode();
                ~SearchTrieNode();

                SearchTrieNodePointer addChild( char letter, const SearchTrieNodePointer new_child );
                SearchTrieNodePointer findChild( char letter );
                bool hasChildren() const;

                void addTarget( storage_type* new_target );
                void removeTarget( storage_type* target );
                std::list< storage_type* > getTargets() const;
                void getAllDescendentTargets( std::list<storage_type*> *list );
                bool hasTargets() const;

                void debug();

                unsigned id;

            protected:
                std::map< char, SearchTrieNodePointer > children;
                std::list< storage_type* > targets;

        };

        template <typename storage_type>
        unsigned SearchTrieNode< storage_type >::next_id = 0;


        /**
        *
        */
        template<typename storage_type>
        SearchTrieNode< storage_type >::SearchTrieNode() : id(++next_id){

        }


        /**
        * Destructor. Cleans up allocated memory.
        *
        */
        template<typename storage_type>
        SearchTrieNode< storage_type >::~SearchTrieNode(){

        }


        /*
        * Finds a child by the letter.
        * Return NULL if not found.
        *
        */
        template<typename storage_type>
        typename SearchTrieNode< storage_type >::SearchTrieNodePointer SearchTrieNode<storage_type>::findChild( char letter ){

            typename std::map< char, SearchTrieNodePointer >::iterator my_iterator;

            my_iterator = this->children.find( letter );

            if( my_iterator == this->children.end() ){
                return NULL;
            }else{
                return my_iterator->second;
            }

        }


        /*
        * Adds a single child, by the letter.
        * Returns the new_child pointer, if it was added. Else, it returns a pointer to the existing node.
        *
        */
        template<typename storage_type>
        typename SearchTrieNode< storage_type >::SearchTrieNodePointer SearchTrieNode<storage_type>::addChild( char letter, const SearchTrieNodePointer new_child ){

            SearchTrieNodePointer child = this->findChild( letter );

            //if not found, add it
            if( child == NULL ){
                typename std::pair<char, SearchTrieNodePointer> entry(letter, new_child);
                this->children.insert( entry );
                return new_child;
            }else{
                return child;
            }

        }


        template<typename storage_type>
        void SearchTrieNode< storage_type >::addTarget( storage_type* new_target ){

            this->targets.push_back( new_target );

        }


        template<typename storage_type>
        void SearchTrieNode< storage_type >::removeTarget( storage_type* target ){

            this->targets.remove( target );

        }


        template<typename storage_type>
        std::list< storage_type* > SearchTrieNode< storage_type >::getTargets() const{

            return this->targets;

        }


        /*
        * Gets all of the targets in this node and all of its descendents and inserts them into the provided list.
        *
        */
        template<typename storage_type>
        void SearchTrieNode< storage_type >::getAllDescendentTargets( std::list< storage_type* > *list ){

            std::list< storage_type* > copy( this->targets );
            list->merge( copy );

            SearchTrieNodePointer child;
            typename std::map< char, SearchTrieNodePointer >::iterator child_iterator;

            for( child_iterator = this->children.begin(); child_iterator != this->children.end(); child_iterator++ ){

                child = child_iterator->second;
                child->getAllDescendentTargets( list );

            }

        }


        template<typename storage_type>
        bool SearchTrieNode< storage_type >::hasTargets() const{

            return ( this->targets.size() > 0 );

        }


        template<typename storage_type>
        bool SearchTrieNode< storage_type >::hasChildren() const{

            return ( this->children.size() > 0 );

        }


        template<typename storage_type>
        void SearchTrieNode< storage_type >::debug(){

            typename std::map< char, SearchTrieNodePointer >::iterator child_iterator;
            typename std::list< storage_type* >::iterator target_iterator;

            SearchTrieNodePointer child;

            for( child_iterator = this->children.begin(); child_iterator != this->children.end(); child_iterator++ ){

                child = child_iterator->second;

                //display id, letter and parent_id
                    cout << endl << "node_id: " << child->id << "( " << child_iterator->first << " )" << ", parent: " << this->id;

                //display targets
                    if( child->hasTargets() ){
                        cout << ", targets( " ;
                        for( target_iterator = child->targets.begin(); target_iterator != child->targets.end(); target_iterator++ ){
                            cout << **target_iterator << ", " ;
                        }
                        cout << ")";
                    }

                //display all of the children's debug info
                    child->debug();

            }

        }

    }

#endif //SEARCH_TRIE_NODE_H