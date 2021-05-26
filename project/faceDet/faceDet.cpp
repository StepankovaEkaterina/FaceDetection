#include "faceDet.h"

std::vector<cv::Rect> facedetection::faceDet(const std::string &p_path)
{
    std::vector<cv::Rect> findRects;
    cv::Mat img = cv::imread(p_path);
    if(img.data)
    {
        std::cout << "load image: " << p_path << std::endl;
        cv::CascadeClassifier faceCascade = init();
        faceCascade.detectMultiScale(img, findRects, 1.1, 3, 0, cv::Size(30, 30));
        free(faceCascade);        
    }
    else
    {
        std::cout << "file: " << p_path << " doesn't open" << std::endl;
    }

    return findRects;
}

cv::CascadeClassifier facedetection::init()
{
    cv::CascadeClassifier faceCascade;
    std::string cascadePath = "haarcascade_frontalface_alt2.xml";
    faceCascade.load(cascadePath);
    return faceCascade;
}

void facedetection::free(cv::CascadeClassifier &p_faceCascade)
{
    p_faceCascade.empty();
}