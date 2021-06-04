#ifndef APP_H
#define APP_H

#include <vector>
#include <set>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include "windows.h"
#include <tchar.h>

#include <opencv2/imgproc/imgproc.hpp>

#include <json/json.h>
#include <json/writer.h>

typedef HRESULT (CALLBACK* LPFNDLLFUNC1)(DWORD,UINT*);

class App
{
public:        
    App(const int argc, const char *argv[]);

    ~App();
    
    int run();

private:
    
    HINSTANCE m_instanceDll;
    LPCSTR faceDetection = "faceDet";
    std::string m_appPath;
    std::string m_inputPath;
    std::vector<std::string> m_filePaths;
    std::map<std::string, std::vector<cv::Rect>> m_result;
    
    int scanDir();
    int dirExists(const char *path);
    void findFacesResult();
    void saveAndBlurImages();
    void saveJsonResult();
    void blurFaces(cv::Mat &p_img,  std::vector<cv::Rect> &p_resRects);
    void saveImage(cv::Mat &p_img, std::string p_path);        
};

#endif