#ifndef _ANN_H_
#define _ANN_H_
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;

static const char *kAllChars[] = 
{
	"0", "1", "2",
	"3", "4", "5",
	"6", "7", "8",
	"9",
};



class AnnTrain
{
public:
	AnnTrain();
	void annTrain();
	string recogData(Mat &image);
	Ptr<ml::TrainData> tdata();
public:
	Ptr<ml::ANN_MLP> ann;
	const char* chars_folder_;

};
#endif