#ifndef APP_H
#define APP_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>

#include "faceDet/faceDet.h"
#include <opencv2/core/utility.hpp>

#include <json/json.h>
#include <json/writer.h>

namespace facedetection
{
    class App
    {
    public:        
        App(const int argc, const char *argv[]);

        ~App();
        
        int run();

    private:
        
        std::string m_appPath;
        std::string m_inputPath;
        std::vector<std::string> m_filePaths;

        int scanDir();
        void findFases();
        void blurFaces(cv::Mat &p_img,  std::vector<cv::Rect> &p_resRects);
        void saveImage(cv::Mat &p_img, std::string p_path);        
    };
}

#endif