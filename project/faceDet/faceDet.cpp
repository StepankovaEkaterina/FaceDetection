#include "faceDet.h"

std::vector<cv::Rect> faceDetection::faceDet(const std::string &p_path, cv::CascadeClassifier &p_faceCascade)
{
    std::vector<cv::Rect> findRects;
    cv::Mat img = cv::imread(p_path);
    if(img.data)
    {
        std::cout << "Loaded image: " << p_path << std::endl;
        p_faceCascade.detectMultiScale(img, findRects, 1.1, 3, 0, cv::Size(30, 30));
        if(findRects.size())
        {
            for(cv::Rect &rect : findRects)
            {
                cv::blur(img(rect), img(rect), cv::Size(3,3));
            }
            cv::Size newSize = cv::Size(img.size().width / 2, img.size().height / 2);
            cv::resize(img, img, newSize);
            std::string savePath = p_path;
            size_t pos = p_path.find_last_of('.');
            if(pos != std::string::npos)
            {
                savePath.insert(pos, "saved");
                cv::imwrite(savePath, img);
            }
        }
    }
    else
    {
        std::cout << "File: " << p_path << "doesn't open" << std::endl;
    }

    return findRects;
}