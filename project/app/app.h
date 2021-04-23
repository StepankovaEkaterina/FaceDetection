#ifndef APP_H
#define APP_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "../faceDet/faceDet.h"

namespace facedetection
{
    class App
    {
    public:        
        App(const int argc, const char *argv[]);

        ~App();
        
        int run();

    private:
        boost::program_options::variables_map m_OptMap;
        boost::program_options::options_description m_options;              
        boost::filesystem::path m_AppPath;
        std::string m_InputPath;
        std::vector<boost::filesystem::path> m_FilePaths;

        void scanDir();
        void findFases();        
    };
}

#endif