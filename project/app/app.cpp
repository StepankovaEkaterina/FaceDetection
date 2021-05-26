#include "app.h"

using namespace facedetection;

App::App(const int argc, const char *argv[])
{
    if(argv[0])
        m_appPath = std::string(argv[0]);
    if(argv[1])
        m_inputPath = std::string(argv[1]);    
}

App::~App() 
{

}

int dirExists(const char *path)
{
    struct stat info;

    if(stat(path, &info) != 0)
        return 0;
    return ( info.st_mode & S_IFDIR ) ? 1 : 0;
}

int App::run()
{
    int hr = -1;

    try
    {
        if(m_inputPath.empty() || !dirExists(m_inputPath.c_str()))
        {  
            std::cout << "input path " << m_inputPath << " not found";
            throw  "input path " + m_inputPath + " not found";
        }

        std::cout << "set path for scan: '" << m_inputPath << "'" << std::endl;
        if(this->scanDir() == 0)
        {
            throw "no files found along path " + m_inputPath;
        }
        
        if(m_filePaths.size())
        {
            std::cout << "found " << m_filePaths.size() << " files" << std::endl;
            this->findFases();
            hr = 0;
        }        
    }
    catch (std::string &e)
    {
        std::cerr << e << std::endl;
    }
    catch (char *e)
    {
        std::cerr << e << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return hr;
}

int App::scanDir()
{    
    cv::glob(m_inputPath, m_filePaths, true);

    return m_filePaths.size();  
}

void App::findFases()
{    
    Json::Value root;
    std::vector<cv::Rect> resRects;
    int i = 0;
    for(auto &path : m_filePaths)
    {
        Json::Value res;
        res["path"] = path;
        
        cv::Mat img = cv::imread(path);
        resRects = facedetection::faceDet(path);
        if(resRects.size())
        {
            blurFaces(img, resRects);
            saveImage(img, path);
        }

        if(resRects.size())
        {
            int j = 0;
            Json::Value coords;
            for(cv::Rect &rect: resRects)
            {
                Json::Value result;
                result["x"] = rect.x;
                result["y"] = rect.y;
                result["width"] = rect.width;
                result["height"] = rect.height;
                coords[j] = result;
                j++;                
            }
            res["coordinates"] = coords;
        }        
        root[i] = res;
        i++;        
    }
    std::ofstream jsonOutStream;
    std::string jsonPath = m_inputPath + "result.json";
    jsonOutStream.open(jsonPath);    
    Json::StreamWriterBuilder writerBuilder;
    std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
    if(writer)
    {        
        writer->write(root, &jsonOutStream);        
    }
    jsonOutStream.close();    
}

void App::blurFaces(cv::Mat &p_img,  std::vector<cv::Rect> &p_resRects)
{        
    if(p_img.data)
    {
        for(cv::Rect &rect : p_resRects)
        {                  
            cv::blur(p_img(rect), p_img(rect), cv::Size(10, 10));            
        }
    }
}   

void App::saveImage(cv::Mat &p_img, std::string p_path)
{
    cv::Size newSize = cv::Size(p_img.size().width / 2, p_img.size().height / 2);
    cv::resize(p_img, p_img, newSize);
    std::string savePath = p_path;
    size_t pos = p_path.find_last_of('.');
    if(pos != std::string::npos)
    {
        savePath.insert(pos, "_saved");
        cv::imwrite(savePath, p_img);
    }
}