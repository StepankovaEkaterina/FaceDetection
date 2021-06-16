#include "app.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include <opencv2/core/utility.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

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

int scanDir(std::string &p_inputPath, std::vector<std::string> &p_filePaths)
{    
    cv::glob(p_inputPath, p_filePaths, true);

    return p_filePaths.size();  
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
    
    HINSTANCE instanceDll;
    LPCSTR faceDetection = "faceDet";
    LPCSTR init = "initLibrary";
    LPCSTR free = "freeLibrary";    
#ifdef _WIN32		
	instanceDll = LoadLibrary("libfaceDet");		
#else
	instanceDll	= dlopen("libfaceDet",RTLD_LAZY);
#endif    
    
    try
    {
        if(instanceDll == NULL)
	    {
            std::cout <<  "library not loaded";
        }

        FARPROC adresse_init = GetProcAddress(instanceDll, init);
        if(adresse_init == 0)
        {
            std::cout <<  "initLibrary not loaded";
        }

        typedef void (__cdecl *init)();
        ((init)adresse_init)();

        if(m_inputPath.empty() || !dirExists(m_inputPath.c_str()))
        {  
            std::cout << "input path " << m_inputPath << " not found";
            return -1;
        }

        std::cout << "set path for scan: '" << m_inputPath << "'" << std::endl;
        if(scanDir(m_inputPath, m_filePaths) == 0)
        {
            std::cout << "no files found along path " << m_inputPath;
            return -1;
        }
        
        if(m_filePaths.size())
        {
            std::cout << "found " << m_filePaths.size() << " files" << std::endl;
            this->findFacesResult(instanceDll, faceDetection);
            if(!m_result.empty())
            {
                this->saveAndBlurImages();
                this->saveJsonResult();
            }
            hr = 0;
        } 

        FARPROC adresse_free = GetProcAddress(instanceDll, free);
        if(adresse_free != 0)
        {
            typedef void (__cdecl *free)();
            ((free)adresse_free)();
        }              
    }    
    catch (char *e)
    {
        std::cerr << e << std::endl;
    }     

    return hr;
}

void App::findFacesResult(HINSTANCE &p_instanceDll, LPCSTR &p_nameFunction)
{
    FARPROC adresse_faceDet = GetProcAddress(p_instanceDll, p_nameFunction);
    if(adresse_faceDet != 0)
    {        
        for(auto &path : m_filePaths)
        {
            cv::Mat img = cv::imread(path);
            TRectsFace res{};
            typedef int (__cdecl *faceDetection)(const char*, TRectsFace&);
            ((faceDetection)adresse_faceDet)(path.c_str(), res);
            std::vector<cv::Rect> resRects;
            if(res.count > 0)
                std::copy(res.rects, res.rects + res.count, std::back_inserter(resRects));            
            m_result.emplace(std::make_pair(path, resRects));
        }        
    }    
}

void App::saveAndBlurImages()
{
    for(auto& res : m_result)
    {
        cv::Mat img = cv::imread(res.first);
        if(!res.second.empty())
        {
            blurFaces(img, res.second);
            saveImage(img, res.first);
        }
    }
}

void App::saveJsonResult()
{
    Json::Value root;
    int i = 0;
    for(auto& res : m_result)
    {
        Json::Value result1;
        result1["path"] = res.first;
        if(res.second.size())
        {
            int j = 0;
            Json::Value coords;
            for(cv::Rect &rect: res.second)
            {
                Json::Value result2;
                result2["x"] = rect.x;
                result2["y"] = rect.y;
                result2["width"] = rect.width;
                result2["height"] = rect.height;
                coords[j] = result2;
                j++;                
            }
            result1["coordinates"] = coords;
        }        
        root[i] = result1;
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