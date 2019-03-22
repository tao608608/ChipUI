#ifndef _CHIP_RECOG_H_
#define _CHIP_RECOG_H_
#include "opencv2/opencv.hpp"
#include <iostream>
#include "ann.h"
#include "svm.h"
#include <memory>
#include "tiny_dnn/tiny_dnn.h"
#include "opencv2/opencv.hpp"
#include "io.h"
#include "getseven.h"


using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;

using namespace std;
using namespace cv;





struct Chip
{
	cv::Mat image;
	cv::Mat chipImage;
	cv::Mat objectArea;
	cv::Rect chipSize;
	vector<cv::Mat>candidates;
};

class  ChipRecog
{
public:
	ChipRecog();
	int chipLocate(cv::Mat &image, cv::Mat &out);
	int chipLocate2(cv::Mat &image, cv::Mat &out);
	int FindSpecialChar(string path, cv::Mat &image, cv::Rect &specialRect, SvmTrain &mySvm, int &lengthP, int type);
	int FindNumberOne(string path, cv::Mat &image, cv::Rect &specialRect, network<sequential> &nn, int &lengthP, int type);
	bool getImageOne(Rect &rect, Mat &image);
	bool getNextRect1(Mat &image_, Rect &rect, Mat &compareImage_, Rect specialrect);
	bool getNextRect(Mat &image_, Rect &rect, Mat &compareImage_, int width);
	bool getNextRect2(Mat &image_, Rect &rect, Mat &compareImage_, Rect specialrect, int &adhesion);
	bool getImage(cv::Rect &rect, cv::Mat &image,int type);
	int chipRecogObject(cv::Mat &in, cv::Mat &chipImage, string &s, string &special, AnnTrain &at, cv::Rect &specialRect, int number);
	

	cv::Mat preprocessChar(cv::Mat in);
	bool setRectScale(cv::Size a, cv::Size b);
public:
	cv::Size minRect;
	cv::Size maxRect;
public:
	int  chipRecogSegment(Mat &input, Rect &specialRect, string &s,/* AnnTrain &at*/network<sequential> &nn, int number,int value);
	int  dealBoundary(Rect &rect, Mat &image,int value);

	int  chipRecogSegmentOne(Mat &input, Rect &specialRect, string &s, network<sequential> &nn ,int number);




public:
	int FindSpecialFirstNumber(Mat &input, Rect &specialRect);
	int FindSpecialNumberSeven(Mat &input, Rect &specialRect, string path ,int find7ways);
};
#endif