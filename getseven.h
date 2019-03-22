#pragma once
//export.h是DLL接口函数的声明，需要所有包含目录和命名空间。 
#define DllExport _declspec(dllexport)
#include <opencv2\opencv.hpp>
#include <iostream>



using namespace std;
using namespace cv;
using namespace cv::ml;



extern "C" int   _stdcall getSevenRect(const Mat &img, Rect &resultRect, Ptr<ml::SVM> &svm, bool haveresize, int find7way);

//extern "C" int  DllExport _stdcall  getSevenRect(const Mat &img, Rect &resultRect, Ptr<ml::SVM> &svm, bool haveresize, int find7way);
//函数返回0表示正常，1表示图为空，2表示svm的只能指针有问题，3表示没有找到7