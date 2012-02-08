#ifndef STATE_H
#define STATE_H

    #include <string>


namespace Altumo{

    class State{

        public:
            State( std::string code, std::string name );
            ~State();

            std::string code;
            std::string name;

    };

    typedef map< std::string, State > StateMap;
    typedef pair< std::string, State > StateMapPair;


}



#endif //STATE_H
