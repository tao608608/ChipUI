#include "stdafx.h"
#include "svm.h"
#include "io.h"
SvmTrain::SvmTrain()
{
	svm = cv::ml::SVM::create();
}
void getFilesAll1(string path, vector<string>& files)
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
					getFilesAll1(p.assign(path).append("\\").append(fileinfo.name), files);
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
void getJustCurrentDir1(string path, vector<string>& files)
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

void getAllFiles1(string path, vector<string>& files)
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
					getAllFiles1(p.assign(path).append("\\").append(fileinfo.name), files);
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
float countOfBigValue2(Mat &mat, int iValue)
{
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

Mat ProjectedHistogram(Mat img, int t) {
	int sz = (t) ? img.rows : img.cols;
	Mat mhist = Mat::zeros(1, sz, CV_32F);

	for (int j = 0; j < sz; j++) {
		Mat data = (t) ? img.row(j) : img.col(j);

		//统计这一行或一列中，非零元素的个数，并保存到mhist中

		mhist.at<float>(j) = countOfBigValue2(data, 20);
	}

	// Normalize histogram
	double min, max;
	minMaxLoc(mhist, &min, &max);

	//用mhist直方图中的最大值，归一化直方图

	if (max > 0)
		mhist.convertTo(mhist, -1, 1.0f / max, 0);

	return mhist;
}

Mat getTheFeatures(Mat in)
{
	const int VERTICAL = 0;
	const int HORIZONTAL = 1;

	// Histogram features
	Mat vhist = ProjectedHistogram(in, VERTICAL);
	Mat hhist = ProjectedHistogram(in, HORIZONTAL);

	// Last 10 is the number of moments components
	int numCols = vhist.cols + hhist.cols;

	Mat out = Mat::zeros(1, numCols, CV_32F);

	// Asign values to feature,样本特征为水平、垂直直方图

	int j = 0;
	for (int i = 0; i < vhist.cols; i++) {
		out.at<float>(j) = vhist.at<float>(i);
		j++;
	}
	for (int i = 0; i < hhist.cols; i++) {
		out.at<float>(j) = hhist.at<float>(i);
		j++;
	}

	return out;
}

void getHistogramFeatures(const Mat& image, Mat& features)
{
	Mat grayImage;
	if (image.channels() == 3)
		cvtColor(image, grayImage, CV_RGB2GRAY);
	else
		grayImage = image.clone();

	//grayImage = histeq(grayImage);

	Mat img_threshold;
	threshold(grayImage, img_threshold, 0, 255,
		CV_THRESH_OTSU + CV_THRESH_BINARY_INV);
	features = getTheFeatures(img_threshold);
}
void SvmTrain::trainData()
{

	svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::RBF);
	svm->setDegree(0.1);
	// 1.4 bug fix: old 1.4 ver gamma is 1
	svm->setGamma(0.1);
	svm->setCoef0(0.1);
	svm->setC(1);
	svm->setNu(0.1);
	svm->setP(0.1);
	svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 100000, 0.00001));
	auto train_data = tdata();
	svm->train(train_data);
	svm->save("svm.xml");
}

Ptr<cv::ml::TrainData> SvmTrain::tdata()
{
	vector<string>filenamesIs;
	vector<string>filenamesNot;
	getAllFiles1("svm_char/is_", filenamesIs);
	getAllFiles1("svm_char/not_", filenamesNot);

	Mat samples;
	vector<int>labors;
	for (int i = 0; i < 2; ++i)
	{
		if (i == 0)
		{
			for (auto &c : filenamesNot)
			{
				auto image = imread(c);
				if (!image.data)
					continue;
				Mat feature;
				getHistogramFeatures(image, feature);
				feature = feature.reshape(1, 1);
				samples.push_back(feature);
				labors.push_back(i);
			}
		}
		else
		{
			for (auto &d : filenamesIs)
			{
				auto image = imread(d);
				if (!image.data)
					continue;
				Mat feature;
				getHistogramFeatures(image, feature);
				feature = feature.reshape(1, 1);
				samples.push_back(feature);
				labors.push_back(i);
			}
		}
	}

	cv::Mat samples_, responses_;
	samples.convertTo(samples_, CV_32FC1);
	cv::Mat(labors).copyTo(responses_);

	return cv::ml::TrainData::create(samples_, cv::ml::SampleTypes::ROW_SAMPLE,
		responses_);
}
bool SvmTrain::recogData(Mat &image)
{
	Mat feature;
	getHistogramFeatures(image, feature);
	feature = feature.reshape(1, 1);
	float response = svm->predict(feature);
	if (response == 1)
		return true;
	else
		return false;
}