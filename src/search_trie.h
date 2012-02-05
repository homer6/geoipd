#ifndef SEARCH_TRIE_H
#define SEARCH_TRIE_H

    //#include <stdio.h>
    #include <algorithm>
    #include <string>
    #include <iostream>
    #include <forward_list>
    //#include <boost/regex.hpp>

    #include "search_trie_node.h"


    using namespace std;

    namespace Altumo{

        template <typename storage_type>
        class SearchTrie{

            public:
                typedef SearchTrieNode< storage_type >* SearchTrieNodePointer;

                SearchTrie();
                ~SearchTrie();

                void addWord( const std::string &word, storage_type* target );
                void search( const std::string &query, std::forward_list< storage_type* >* results );

                SearchTrieNodePointer getRoot();
                void debug();

            protected:
                void addStub( const std::string &stub, storage_type* target );
                SearchTrieNode< storage_type > root;
                size_t size;
                size_t num_targets;

        };


        /**
        *
        */
        template<typename storage_type>
        SearchTrie< storage_type >::SearchTrie() : size(0), num_targets(0) {

        }


        /**
        * Destructor. Cleans up allocated memory.
        *
        */
        template<typename storage_type>
        SearchTrie< storage_type >::~SearchTrie(){

        }



        template<typename storage_type>
        void SearchTrie< storage_type >::addWord( const std::string &word, storage_type* target ){

            std::string word_lowercase = word;
            std::transform( word_lowercase.begin(), word_lowercase.end(), word_lowercase.begin(), ::tolower );

            //unsigned size = word_lowercase.size();
            //unsigned x;
            //std::string stub;

            //for( x = 0; x < size; x++ ){
            //    stub = word_lowercase.substr( x, size - x );
                this->addStub( word_lowercase, target );
            //}
        }


        /*
        * Adds all of the results to the list, given the partial word query.
        *
        */
        template<typename storage_type>
        void SearchTrie< storage_type >::search( const std::string &query, std::forward_list< storage_type* >* results ){

            std::string word_lowercase = query;
            std::transform( word_lowercase.begin(), word_lowercase.end(), word_lowercase.begin(), ::tolower );

            //unsigned size = word_lowercase.size();
            SearchTrieNodePointer current_node = this->getRoot();
            std::string::iterator letter_iterator;

            SearchTrieNodePointer found = NULL;


            //search until the last letter
            for( letter_iterator = word_lowercase.begin(); letter_iterator != word_lowercase.end(); letter_iterator++ ){

                found = current_node->findChild( *letter_iterator );

                if( found == NULL ){
                    //return no results
                    return;
                }else{
                    current_node = found;
                }

            }

            //add the results
            current_node->getAllDescendentTargets( results );

        }



        template<typename storage_type>
        void SearchTrie< storage_type >::addStub( const std::string &stub, storage_type* target ){

            SearchTrieNodePointer current_node = this->getRoot();
            std::string::const_iterator iterator;

            unsigned stub_count = stub.size();
            unsigned x = 0;

            for( iterator = stub.begin(); iterator != stub.end(); iterator++ ){

                ++x;

                SearchTrieNodePointer child = current_node->findChild( *iterator );

                //if not found, add it
                if( child == NULL ){
                    SearchTrieNodePointer new_node = new SearchTrieNode< storage_type >;
                    current_node->addChild( *iterator, new_node );
                    this->size++;
                    current_node = new_node;
                }else{
                    current_node = child;
                }


                //add the target to the last letter's node (unless the target already exists in the descendents)
                    if( x == stub_count ){

                        if( !current_node->isInDescendents(target) ){
                            current_node->addTarget( target );
                            this->num_targets++;
                        }

                    }


            }


        }


        template<typename storage_type>
        typename SearchTrie< storage_type >::SearchTrieNodePointer SearchTrie< storage_type >::getRoot(){

            return &(this->root);

        }


        template<typename storage_type>
        void SearchTrie< storage_type >::debug(){

            //typename SearchTrie< storage_type >::SearchTrieNodePointer current_node = this->getRoot();

            std::cout << "Size: " << this->size << endl;
            std::cout << "Targets: " << this->num_targets << endl;

            //std::cout << "root: " << endl;

            //current_node->debug();

            std::cout << endl;


        }

    }

#endif //SEARCH_TRIE_H
