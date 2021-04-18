#ifndef APP_H
#define APP_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>

#include <boost/filesystem.hpp>

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
        boost::filesystem::path m_AppPath;
    };

}

#endif