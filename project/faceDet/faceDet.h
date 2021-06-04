#ifndef FACEDET_H
#define FACEDET_H

#include <opencv2/imgproc/imgproc.hpp>

#ifdef FACEDET_EXPORTS
#define FACEDET_API __declspec(dllexport)
#else
#define FACEDET_API __declspec(dllimport)
#endif

extern "C" FACEDET_API cv::Rect* faceDet(const char* p_path);

extern "C" FACEDET_API void initLibrary();

extern "C" FACEDET_API void freeLibrary();

#endif