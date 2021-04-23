#include "app.h"

using namespace facedetection;

App::App(const int argc, const char *argv[]): m_options("options")
{
    m_options.add_options()        
        ("input,I", boost::program_options::value<std::string>(), "input path");
    
    boost::program_options::options_description desc("allowed options");
    desc.add(m_options);
        
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), m_OptMap);
    boost::program_options::notify(m_OptMap);
        
    m_AppPath = boost::filesystem::system_complete(argv[0]).parent_path();
}

App::~App() 
{

}

int App::run()
{
    int hr = -1;

    if (m_OptMap.count("input"))
    {
        m_InputPath = m_OptMap["input"].as<std::string>();
        if (!boost::filesystem::exists(m_InputPath) || 
            !boost::filesystem::is_directory(m_InputPath))
        {
            std::cout << "input path " << m_InputPath << " not found";
            return hr;
        }
        std::cout << "set path for scan: '" << m_InputPath << "'" << std::endl;
    }
    
    this->scanDir();
    if(m_FilePaths.size())
    {
        std::cout << "found " << m_FilePaths.size() << " files" << std::endl;
        App::findFases();

        hr = 0;
    }

    return hr;
}

void App::scanDir()
{
    std::vector<std::string> imgTypes {".jpg",".jpeg"};

    boost::filesystem::recursive_directory_iterator dir(m_InputPath), end;
    
    bool isExt = false;
    while(dir != end)
    {
        isExt = std::find(imgTypes.cbegin(), imgTypes.cend(), dir->path().extension()) != imgTypes.cend();
        if (isExt && boost::filesystem::is_regular_file(*dir))
        {
            m_FilePaths.push_back(boost::filesystem::absolute(dir->path()));
        }
        dir++;
    }
}

void App::findFases()
{
    cv::CascadeClassifier cascade;
    std::string cascadePath = (m_AppPath / "haarcascade_frontalface_alt2.xml").string();
    if (cascade.load(cascadePath))
    {
        boost::property_tree::ptree resJSON;
        std::vector<cv::Rect> resRects;
        for (boost::filesystem::path &path : m_FilePaths)
        {
            resRects = facedetection::faceDet(path.string(), cascade);

            boost::property_tree::ptree parentNode;
            if(resRects.size())
            {
                for(cv::Rect &rect: resRects)
                {
                    boost::property_tree::ptree childNode;
                    childNode.put("x", rect.x);
                    childNode.put("y", rect.y);
                    childNode.put("width", rect.width);
                    childNode.put("height", rect.height);
                    parentNode.push_back(std::make_pair("", childNode));
                }
            }
            resJSON.push_back(std::make_pair(path.string(), parentNode));
        }
        std::ofstream jsonOutStream;
        std::string jsonPath = "result.json";
        jsonOutStream.open(jsonPath);
        boost::property_tree::json_parser::write_json(jsonOutStream, resJSON, true);
        jsonOutStream.close();
    }
}