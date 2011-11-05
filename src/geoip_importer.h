#ifndef GEOIP_IMPORTER_H
#define GEOIP_IMPORTER_H

    #include <sys/time.h>
    #include <unistd.h>
    #include <err.h>
    #include <stdio.h>
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <stdlib.h>

    #include <boost/program_options.hpp>
    #include <boost/regex.hpp>

    #include "connector.h"


    using namespace std;

    namespace Altumo{

        class GeoipImporter{

            public:
                GeoipImporter();
                ~GeoipImporter();
                int setProgramOptions( int argc, char** argv );
                void clearExistingTables();
                void createNewTables();
                void importBlocksFile();
                void importLocationsFile();

            protected:
                string blocks_filename;
                string locations_filename;
                Connector *connector;

        };

    }

#endif //GEOIP_IMPORTER_H
