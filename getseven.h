#pragma once
//export.h��DLL�ӿں�������������Ҫ���а���Ŀ¼�������ռ䡣 
#define DllExport _declspec(dllexport)
#include <opencv2\opencv.hpp>
#include <iostream>



using namespace std;
using namespace cv;
using namespace cv::ml;



extern "C" int   _stdcall getSevenRect(const Mat &img, Rect &resultRect, Ptr<ml::SVM> &svm, bool haveresize, int find7way);

//extern "C" int  DllExport _stdcall  getSevenRect(const Mat &img, Rect &resultRect, Ptr<ml::SVM> &svm, bool haveresize, int find7way);
//��������0��ʾ������1��ʾͼΪ�գ�2��ʾsvm��ֻ��ָ�������⣬3��ʾû���ҵ�7