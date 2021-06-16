#include "faceDet.h"

#include <iostream>
#include <thread>
#include <vector>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::CascadeClassifier m_faceCascade;
std::string m_cascadePath = "haarcascade_frontalface_alt2.xml";

int faceDet(const char* p_path, TRectsFace &p_res)
{
    cv::Rect *rects = nullptr;
    std::vector<cv::Rect> findRects;
    cv::Mat img = cv::imread(p_path);
    if(img.data)
    {
        std::cout << "load image: " << p_path << std::endl;        
        m_faceCascade.detectMultiScale(img, findRects, 1.1, 3, 0, cv::Size(30, 30));               
    }
    else
    {
        std::cout << "file: " << p_path << " doesn't open" << std::endl;
        return -1;
    }
    
    if(findRects.size())
    {        
        rects = new cv::Rect[findRects.size()];
        for(size_t i = 0; i < findRects.size(); i++)
        {
            rects[i] = findRects[i];
        }      
    }

    p_res.count = findRects.size();
    p_res.rects = rects;    

    return 0;
}

void initLibrary()
{
    m_faceCascade.load(m_cascadePath);    
}

void freeLibrary()
{
    m_faceCascade.empty();
}