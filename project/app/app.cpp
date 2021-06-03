#include "app.h"

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

#ifdef _WIN32		
	m_instanceDll = LoadLibrary("libfaceDet.dll");		
#else
	m_instanceDll	= dlopen("libfaceDet.dll",RTLD_LAZY);
#endif    
    if(m_instanceDll != NULL)
	{
        initLibrary();
    }

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
            this->findFacesResult();
            if(!m_result.empty())
            {
                this->saveAndBlurImages();
                this->saveJsonResult();
            }
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
    
    if(m_instanceDll != NULL)
	{
        freeLibrary();
    }

    return hr;
}

int App::scanDir()
{    
    cv::glob(m_inputPath, m_filePaths, true);

    return m_filePaths.size();  
}

void App::findFacesResult()
{
    std::vector<cv::Rect> resRects;
    for(auto &path : m_filePaths)
    {
        cv::Mat img = cv::imread(path);
        resRects = faceDet(path);        
        m_result.emplace(std::make_pair(path, resRects));
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