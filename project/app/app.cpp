#include "app.h"

using namespace facedetection;

App::App(const int argc, const char *argv[])
{
    m_AppPath = boost::filesystem::system_complete(argv[0]).parent_path();
}

App::~App() 
{

}

int App::run()
{
    int hr = -1;

    return hr;
}