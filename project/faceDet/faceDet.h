#ifndef FACEDET_H
#define FACEDET_H

#include <opencv2/imgproc/imgproc.hpp>

#define FACEDET_API __declspec(dllexport)

struct TRectsFace
{
    int count;
    cv::Rect* rects;
    ~TRectsFace()
    {
        clear();        
    };
    void clear()
    {
        if(rects)
        {
            delete [] rects;
            rects = nullptr;
        }
    }
};

extern "C" FACEDET_API int faceDet(const char* p_path, TRectsFace &p_res);

extern "C" FACEDET_API void initLibrary();

extern "C" FACEDET_API void freeLibrary();

#endif