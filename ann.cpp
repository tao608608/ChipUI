#include "stdafx.h"

#include "ann.h"
#include "io.h"
#include <fstream>
#include <string>
#define HORIZONTAL 1
#define VERTICAL 0
void getFilesAll(string path, vector<string>& files)
{

	long hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;


	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
					getFilesAll(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void getJustCurrentDir(string path, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;        
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(path + fileinfo.name);
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
				}

			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void getAllFiles(string path, vector<string>& files)
{
	//文件句柄  
	long   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


AnnTrain::AnnTrain()
{
	ann = ml::ANN_MLP::create();
}
Mat features(Mat in, int sizeData);
Ptr<ml::TrainData> AnnTrain::tdata()
{

	Mat samples;
	vector<int> labels;
	vector<string>filenames;
	getJustCurrentDir("ann_char/", filenames);
	for (int i = 0; i < 10; ++i)
	{
		vector<string>files;
		getAllFiles(filenames[i], files);
		for (auto &c : files)
		{
			auto img = cv::imread(c, 0);
			auto fps = features(img, 10);
			samples.push_back(fps);
			labels.push_back(i);
		}
	}
	Mat samples_;
	samples.convertTo(samples_, CV_32F);
	Mat train_classes =
		cv::Mat::zeros((int)labels.size(), 10, CV_32F);
	for (int i = 0; i < train_classes.rows; ++i)
	{
		train_classes.at<float>(i, labels[i]) = 1.f;
	}
	return ml::TrainData::create(samples_, cv::ml::SampleTypes::ROW_SAMPLE,
		train_classes);
}

Rect GetCenterRect(Mat &in) {
	Rect _rect;

	int top = 0;
	int bottom = in.rows - 1;

	//上下

	for (int i = 0; i < in.rows; ++i) {
		bool bFind = false;
		for (int j = 0; j < in.cols; ++j) {
			if (in.data[i * in.step[0] + j] > 20) {
				top = i;
				bFind = true;
				break;
			}
		}
		if (bFind) {
			break;
		}

		//统计这一行或一列中，非零元素的个数

	}
	for (int i = in.rows - 1;
		i >= 0;
		--i) {
		bool bFind = false;
		for (int j = 0; j < in.cols; ++j) {
			if (in.data[i * in.step[0] + j] > 20) {
				bottom = i;
				bFind = true;
				break;
			}
		}
		if (bFind) {
			break;
		}

		//统计这一行或一列中，非零元素的个数

	}

	//左右

	int left = 0;
	int right = in.cols - 1;
	for (int j = 0; j < in.cols; ++j) {
		bool bFind = false;
		for (int i = 0; i < in.rows; ++i) {
			if (in.data[i * in.step[0] + j] > 20) {
				left = j;
				bFind = true;
				break;
			}
		}
		if (bFind) {
			break;
		}

		//统计这一行或一列中，非零元素的个数

	}
	for (int j = in.cols - 1;
		j >= 0;
		--j) {
		bool bFind = false;
		for (int i = 0; i < in.rows; ++i) {
			if (in.data[i * in.step[0] + j] > 20) {
				right = j;
				bFind = true;

				break;
			}
		}
		if (bFind) {
			break;
		}

		//统计这一行或一列中，非零元素的个数

	}

	_rect.x = left;
	_rect.y = top;
	_rect.width = right - left + 1;
	_rect.height = bottom - top + 1;

	return _rect;
}

Mat CutTheRect(Mat &in, Rect &rect) {
	int size = in.cols;  // (rect.width>rect.height)?rect.width:rect.height;
	Mat dstMat(size, size, CV_8UC1);
	dstMat.setTo(Scalar(0, 0, 0));

	int x = (int)floor((float)(size - rect.width) / 2.0f);
	int y = (int)floor((float)(size - rect.height) / 2.0f);

	//把rect中的数据 考取到dstMat的中间

	for (int i = 0; i < rect.height; ++i) {

		//宽

		for (int j = 0; j < rect.width; ++j) {
			dstMat.data[dstMat.step[0] * (i + y) + j + x] =
				in.data[in.step[0] * (i + rect.y) + j + rect.x];
		}
	}

	//
	return dstMat;
}

float countOfBigValue1(Mat &mat, int iValue) {
	float iCount = 0.0;
	if (mat.rows > 1) {
		for (int i = 0; i < mat.rows; ++i) {
			if (mat.data[i * mat.step[0]] > iValue) {
				iCount += 1.0;
			}
		}
		return iCount;

	}
	else {
		for (int i = 0; i < mat.cols; ++i) {
			if (mat.data[i] > iValue) {
				iCount += 1.0;
			}
		}

		return iCount;
	}
}

Mat ProjectedHistogram1(Mat img, int t) {
	int sz = (t) ? img.rows : img.cols;
	Mat mhist = Mat::zeros(1, sz, CV_32F);

	for (int j = 0; j < sz; j++) {
		Mat data = (t) ? img.row(j) : img.col(j);

		//统计这一行或一列中，非零元素的个数，并保存到mhist中

		mhist.at<float>(j) = countOfBigValue1(data, 20);
	}

	// Normalize histogram
	double min, max;
	minMaxLoc(mhist, &min, &max);

	//用mhist直方图中的最大值，归一化直方图

	if (max > 0)
		mhist.convertTo(mhist, -1, 1.0f / max, 0);

	return mhist;
}

Mat features(Mat in, int sizeData) {

	//抠取中间区域

	Rect _rect = GetCenterRect(in);

	Mat tmpIn = CutTheRect(in, _rect);
	
	Mat lowData;
	resize(tmpIn, lowData, Size(sizeData, sizeData));

	
	Mat vhist = ProjectedHistogram1(lowData, VERTICAL);
	Mat hhist = ProjectedHistogram1(lowData, HORIZONTAL);

	
	int numCols = vhist.cols + hhist.cols + lowData.cols * lowData.cols;

	Mat out = Mat::zeros(1, numCols, CV_32F);
	

	int j = 0;
	for (int i = 0; i < vhist.cols; i++) {
		out.at<float>(j) = vhist.at<float>(i);
		j++;
	}
	for (int i = 0; i < hhist.cols; i++) {
		out.at<float>(j) = hhist.at<float>(i);
		j++;
	}
	for (int x = 0; x < lowData.cols; x++) {
		for (int y = 0; y < lowData.rows; y++) {
			out.at<float>(j) += (float)lowData.at < unsigned
				char >(x, y);
			j++;
		}
	}
	return out;
}



void AnnTrain::annTrain()
{
	cv::Mat layers(1, 3, CV_32SC1);
	layers.at<int>(0) = 120;                // the input layer
	layers.at<int>(1) = 40;           // the neurons
	layers.at<int>(2) = 10;  // the output layer

	ann->setLayerSizes(layers);
	ann->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM, 1, 1);
	ann->setTrainMethod(cv::ml::ANN_MLP::TrainingMethods::BACKPROP);
	ann->setBackpropWeightScale(0.1);
	ann->setBackpropMomentumScale(0.1);
	auto traindata = tdata();
	ann->train(traindata);
	ann->save("ann_new.xml");

}
string AnnTrain::recogData(Mat &image)
{
	

	Mat feature = features(image, 10);
	
	auto index = static_cast<int>(ann->predict(feature));

	string s = kAllChars[index];
	return s;

}