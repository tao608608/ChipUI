#ifndef _SVM_H
#define _SVM_H

#include "ann.h"
using namespace std;
using namespace cv;
class SvmTrain
{
public:
	SvmTrain();
	void trainData();
	bool recogData(Mat &image);
	Ptr<cv::ml::TrainData> tdata();
public:

	Ptr<ml::SVM> svm;
};
#endif