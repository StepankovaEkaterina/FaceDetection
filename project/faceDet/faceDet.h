#pragma once

#ifndef FACEDET_H
#define FACEDET_H

#include <iostream>
#include <thread>
#include <vector>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef FACEDET_EXPORTS
#define FACEDET_API __declspec(dllexport)
#else
#define FACEDET_API __declspec(dllimport)
#endif

extern "C" FACEDET_API std::vector<cv::Rect> faceDet(const std::string &p_path);

extern "C" FACEDET_API void initLibrary();

extern "C" FACEDET_API void freeLibrary();

#endif