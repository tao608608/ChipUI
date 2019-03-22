#include "stdafx.h"
#include "chipRecog.h"
#include "ann.h"


using namespace cv;

ChipRecog::ChipRecog()
{
	minRect = Size(20,20);
	maxRect = Size(200, 200);
	
}

string intTString(int a)
{
	stringstream ss;
	ss << a;
	string b;
	ss >> b;
	return b;
}



int chipLocate(Mat &input, Mat &out)
{
	Mat image = input.clone();

	if (image.channels() != 1)
		cvtColor(image, image, CV_BGR2GRAY);
	Mat src = image.clone();
	Mat im_threshold;
	threshold(image, im_threshold, 0, 255, THRESH_OTSU + THRESH_BINARY);



	Mat copy = im_threshold.clone();
	vector<vector<Point>>contours;


	cv::findContours(copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<Rect>rects;

	auto it = contours.begin();

	while (it != contours.end())
	{
		Rect rect = boundingRect(*it);
		rects.push_back(rect);
		cv::rectangle(image, rect, Scalar(255), 3);
		it++;
	}
	if (rects.empty())
	{
		return 1;
	}
	Rect maxErea = rects[0];
	int index = 0;

	for (int i = 1; i < (int)rects.size(); ++i)
	{
		if (rects[i].area()>maxErea.area() && rects[i].width>rects[i].height)
		{
			maxErea = rects[i];
			index = i;
		}
	}

	out = src(maxErea).clone();

	return 0;
}

bool isRect(Rect &rect, const Mat &input)
{
	/*Mat image = input.clone();
	int rectRows = (rect.y+rect.y + rect.height) / 2;
	int start = 0;
	if (rect.x - rect.height > 0)
	{
	start = rect.x - rect.height;
	}
	uchar *data = image.ptr<uchar>(rectRows);
	int count = 0;
	for (int i = start; i < rect.x; ++i)
	{
	if (data[i] != 0)
	{
	count++;
	}
	}
	double ratio = (double)count / ((double)rect.height );
	if (ratio>0.6)
	return true;
	return false;*/
	Mat image = input.clone();

	int count0 = 0;

	for (int i = rect.y; i < rect.y + rect.height; ++i)
	{
		uchar *data = image.ptr<uchar>(i);
		for (int j = rect.x; j < rect.x + rect.width; ++j)
		{
			if (data[j] != 0)
			{
				count0++;
			}
		}
	}



	int start1 = rect.x - rect.width;
	if (start1 < 0)
		return false;
	int count1 = 0;
	for (int i = rect.y; i < rect.y + rect.height; ++i)
	{
		uchar *data = image.ptr<uchar>(i);
		for (int j = start1; j < rect.x; ++j)
		{
			if (data[j] != 0)
			{
				count1++;
			}
		}
	}
	int start2 = start1 - rect.width;
	if (start2 < 0)
		start2 = 0;
	int count2 = 0;
	for (int i = rect.y; i < rect.y + rect.height; ++i)
	{
		uchar *data = image.ptr<uchar>(i);
		for (int j = start2; j < start1; ++j)
		{
			if (data[j] != 0)
			{
				count2++;
			}
		}
	}

	int start3 = start2 - rect.width;
	if (start3 < 0)
		start3 = 0;
	int count3 = 0;
	for (int i = rect.y; i < rect.y + rect.height; ++i)
	{
		uchar *data = image.ptr<uchar>(i);
		for (int j = start3; j < start2; ++j)
		{
			if (data[j] != 0)
			{
				count3++;
			}
		}
	}
	double ratio1 = (double)abs((count1 - count2)) / (double)count1;
	double ratio2 = (double)abs((count1 - count3)) / (double)count1;
	double ratio3 = (double)abs((count2 - count3)) / (double)count2;
	double area_ratio0 = (double)count0 / (double)rect.area();
	double area_ratio1 = (double)count1 / (double)rect.area();
	double area_ratio2 = (double)count2 / (double)rect.area();
	double area_ratio3 = (double)count3 / (double)rect.area();
	if (area_ratio1 > 0.9&&ratio1<0.1&&area_ratio0>0.4/*area_ratio2 > 0.7&&area_ratio3 > 0.7&&ratio1 < 0.2&&ratio2 < 0.2&&ratio3 < 0.2*/)
		return true;
	return false;
}

bool sweepBorder(Rect &rect)
{
	if (rect.y>5)
		return false;
	if (rect.height > 160)
		return true;
	return false;

}

bool checkRight(Rect &rect, const Mat &input)
{
	Mat image = input.clone();
	Rect newRect = Rect(rect.x + rect.width, (rect.y + rect.height + rect.y) / 2, (int)rect.height*1.5, rect.width);

	if (!(rect.x >= 0 && rect.y >= 0 && rect.width > 1 && rect.height > 1 && rect.x + rect.width < input.cols&&rect.y + rect.height < input.rows))
	{
		return true;
	}


	int count = 0;
	for (int i = newRect.y; i < newRect.y + newRect.height; ++i)
	{
		uchar *data = image.ptr<uchar>(i);
		for (int j = newRect.x; j < newRect.x + newRect.width; ++j)
		{
			if (data[j] != 0)
			{
				count++;
			}
		}
	}
	if (count == 0)
		return true;
	return false;

}

bool checkBook(Rect &rect, const Mat &input)
{
	Mat image = input.clone();
	Rect temp1 = Rect(rect.x + rect.width, rect.y, rect.width, rect.height);
	uchar *data = image.data;
	int acount = 0;
	for (int i = temp1.x; i < temp1.x + 4; ++i)
	{
		int count = 0;
		for (int j = temp1.y; j < temp1.y + temp1.height * 3 / 4; ++j)
		{
			if (data[j*image.cols + i] != 0)
				count++;
		}
		if (count == 0)
			acount++;
	}
	if (acount == 4)
		return false;
	return true;
}

vector<Rect>eraseByRect(vector<Rect>&rects,Rect &judgeRect)
{
	vector<Rect>out;
	for (int i = 0; i < rects.size(); ++i)
	{
		if (rects[i].x > judgeRect.x)
		{
			out.push_back(rects[i]);
		}
	}
	return out;
}
vector<Rect> adjustRects(vector<Rect>&rects)
{
	vector<Rect>out;
	vector<int>indexs(rects.size(), 0);
	if (rects.size() == 1)
	{
		return rects;
	}
	std::sort(rects.begin(), rects.end(), [](const Rect&a, const Rect &b){return a.y < b.y; });
	for (int i = 0; i < rects.size() - 1; ++i)
	{
		for (int j = i + 1; j < rects.size(); ++j)
		{
			if (rects[j].y - rects[i].y - rects[i].height * 3 / 4>0 /*&& abs(rects[j].height - rects[i].height)<(rects[i].height/ 2*/)
			{
				indexs[i] = 1;
				break;
			}
		}
	}
	for (int i = 0; i < rects.size(); ++i)
	{
		if (indexs[i] == 0)
			out.push_back(rects[i]);
	}
	std::sort(out.begin(), out.end(), [](const Rect&a, const Rect &b){return a.x < b.x; });
	return out;
}

int ChipRecog::FindSpecialFirstNumber(Mat &input, Rect &specialRect)
{
	Mat image = input.clone();     //复制一个新MAT

	Mat pic;
cvtColor(image, pic, CV_GRAY2BGR);  //从灰度图像模式变为彩色 主要为了显示方便改图仅用于显示

Mat im_threshold;
threshold(image, im_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY_INV);      //二值化图像  注意INV是取反 

Mat objectArea = im_threshold.rowRange(im_threshold.rows / 2, im_threshold.rows);   //这两行是取芯片图的左下角4分之一图
objectArea = objectArea.colRange(0, im_threshold.cols * 1 / 2);

Mat show = Mat::zeros(objectArea.rows, objectArea.cols, CV_8UC1);                   //新建一个objectArea尺寸的全黑图

for (int i = 0; i < objectArea.rows; ++i)                                         //遍历每个像素点，若当前像素和上一个像素不为0且下一个像素为0时，
	//在新建的show中画出该点,即令该点颜色为白，即像素值为255
{
	uchar *data = objectArea.ptr<uchar>(i);
	uchar *data1 = show.ptr<uchar>(i);
	for (int j = 1; j < objectArea.cols - 1; ++j)
	{
		if (data[j] != 0 && data[j - 1] != 0 && data[j + 1] == 0)
		{
			data1[j] = 255;
		}
	}
}


Mat findCont = show.clone();                         //复制show图，用来查找轮廓，因为查找轮廓会损坏原图，故新建
Mat ColorPic = show.clone();                         //复制用于显示
cvtColor(ColorPic, ColorPic, CV_GRAY2BGR);           //灰度图变为彩色图
vector<vector<Point>>contours;                       //轮廓表示的基本模式，即一个点集的集合。
blur(findCont, findCont, Size(3, 3));                //3*3的核进行线性平滑滤波
findContours(findCont, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);         //查找轮廓

auto it = contours.begin();                          //取集合中的第一个点集,为遍历做准备，为C++的迭代器

vector<Rect>candidates;
Rect borderRect = Rect(0, 0, 0, 0);
while (it != contours.end())                        //遍历轮廓中的所有点集
{

	if ((*it).size() < 10)                         //如果当前点集的像素点个数小于10则continue
	{
		it++;
		continue;
	}
	Rect temp = boundingRect(*it);                 //取当前矩形的外接正矩形
	if (temp.height < 50)
	{
		it++;
		continue;
	}
	if (temp.x < 10)
	{
		it++;
		continue;
	}
	if (temp.width < 5 || temp.width>35)
	{
		it++;
		continue;
	}
	if (isRect(temp, objectArea))//？
	{
		it++;
		continue;
	}
	if (sweepBorder(temp))
	{
		if (temp.x > borderRect.x)
			borderRect = temp;
		it++;
		continue;
	}
	if (checkRight(temp, objectArea))
	{
		it++;
		continue;
	}
	if (checkBook(temp, show))
	{
		it++;
		continue;
	}
	/*rectangle(ColorPic, temp, Scalar(255, 0, 0), 1);*/      //在colorPic上画该矩形
	candidates.push_back(temp);
	it++;
}
   if (candidates.empty())
   return 1;

   candidates = eraseByRect(candidates, borderRect);

   candidates = adjustRects(candidates);

   if (candidates.empty())
   return 1;
   

   if (candidates.size() == 1)
   specialRect = candidates[0];
   else
   {
	   if (abs(candidates[0].height - candidates[1].height) > 10)
	   {
		  specialRect = candidates[0].height>candidates[1].height ? candidates[0] : candidates[1];
       }
	   else
	   {
		   specialRect = candidates[0];
	   }
    }
	
	cv::rectangle(ColorPic, specialRect, Scalar(255, 0, 0), 1);
	specialRect.y += im_threshold.rows / 2;
	int top_sp = specialRect.y - specialRect.height / 3 > 0 ? specialRect.y - specialRect.height / 3 : 0;

	Rect tempR = Rect(specialRect.x, top_sp,
		specialRect.width, specialRect.y + specialRect.height - 1 + specialRect.height / 3 < im_threshold.rows ? 5 * specialRect.height / 3 : im_threshold.rows - top_sp);
	Mat tempM = im_threshold.rowRange(top_sp, top_sp + tempR.height);
	int top_new = specialRect.y - top_sp;

	int bottom = top_new + specialRect.height;
	Mat object = tempM.rowRange(top_new, bottom);
	object = object.colRange(specialRect.x + specialRect.width, object.cols);
	Mat image1 = object.clone();
	Mat showshow = tempM.clone();
	Rect charRect = Rect(specialRect.x, top_new, specialRect.width, specialRect.height);
	if (getNextRect(image1, charRect, showshow, specialRect.height*0.6))
	{
		charRect.y += top_sp;
		specialRect = charRect;
		cv::rectangle(pic, charRect, Scalar(0, 255, 0), 2);
	}
	else
	{
		image1 = tempM.rowRange(charRect.y, charRect.y + charRect.height);
		image1 = image1.colRange(charRect.x + charRect.width, tempM.cols);
		getNextRect(image1, charRect, showshow, specialRect.height*0.6);
		charRect.y += top_sp;
		specialRect = charRect;
	}


	Mat lalala = im_threshold.clone();
	/*
	("objectArea", objectArea);
	imshow("show", ColorPic);*/

	//cv::rectangle(lalala, specialRect, Scalar(255, 0, 0), 3);
	//imshow("im_threshold", lalala);
	
	
	return 0;
}


int convertImage(Mat &input, vec_t &d)
{
	cv::Mat_<uint8_t> resized;//uint8_t：一字节
	cv::resize(input, resized, cv::Size(32, 32));

	std::transform(resized.begin(), resized.end(), std::back_inserter(d),
		[=](uint8_t c) { return c * (1.0 / 128.0) - 1; });
	return 0;
}

int recogNumber(network<sequential>&nn, vec_t input, int &index)
{

	nn.set_netphase(net_phase::test);
	label_t predicted = nn.predict_label(input);
	index = predicted;
	//cout << "net" << nn.in_data_size() << endl;
	return 0;
}

bool ChipRecog:: setRectScale(Size a, Size b)
{
	minRect =Size(a.width,a.height);
	maxRect = Size(b.width,b.height);
	return true;
}
int ChipRecog::chipLocate(Mat &image, Mat &out)
{
	if (image.channels() != 1)
		cvtColor(image, image, CV_BGR2GRAY);
	Mat src = image.clone();
	Mat im_threshold;
	threshold(image, im_threshold, 0, 255,    THRESH_OTSU+THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(25, 25));
	erode(im_threshold, im_threshold, element);
	Mat copy = im_threshold.clone();
	vector<vector<Point>>contours;


	cv::findContours(copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<Rect>rects;

	auto it = contours.begin();

	while (it != contours.end())
	{
		Rect rect = boundingRect(*it);
		rects.push_back(rect);
		//rectangle(image, rect, Scalar(255), 3);
		it++;
	}
	if (rects.empty())
		return 2;
	Rect maxErea = rects[0];
	int index = 0;

	for (int i = 1; i < rects.size(); ++i)
	{
		if (rects[i].area()>maxErea.area() && rects[i].width*1.5>rects[i].height)
		{
			maxErea = rects[i];
			index = i;
		}
	}

	out = src(maxErea).clone();
	
	return 0;
}
int ChipRecog::chipLocate2(Mat &image, Mat &out)
{
	if (image.channels() != 1)
		cvtColor(image, image, CV_BGR2GRAY);
	Mat src = image.clone();
	Mat im_threshold;
	threshold(image, im_threshold, 0, 255, THRESH_OTSU + THRESH_BINARY);
	Mat element = getStructuringElement(MORPH_RECT, Size(25, 25));
	erode(im_threshold, im_threshold, element);


	Mat copy = im_threshold.clone();
	vector<vector<Point>>contours;


	cv::findContours(copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	vector<Rect>rects;

	auto it = contours.begin();

	while (it != contours.end())
	{
		Rect rect = boundingRect(*it);
		rects.push_back(rect);
		//rectangle(image, rect, Scalar(255), 3);
		it++;
	}
	if (rects.empty())
		return 2;
	Rect maxErea = rects[0];
	int index = 0;

	for (int i = 1; i < rects.size(); ++i)
	{
		if (rects[i].area()>maxErea.area())
		{
			maxErea = rects[i];
			index = i;
		}
	}

	out = src(maxErea).clone();

	return 0;
}

vector<Rect>  ajustSpecialRects(vector<Rect>&rects)
{
	vector<int>indexs(rects.size(), 0);
	vector<Rect>new_rect;
	for (int i = 0; i < rects.size() - 1; ++i)
	{
		for (int j = i + 1; j < rects.size(); ++j)
		{
			if (rects[j].y - rects[i].y - rects[i].height * 3 / 4>0&&abs(rects[j].area()-rects[i].area())<(rects[i].area()/2))
			{
				indexs[i] = 1;
				break;
			}
		}
	}

	for (int i = 0; i < rects.size(); ++i)
	{
		if (indexs[i] == 0)
			new_rect.push_back(rects[i]);
	}

	return new_rect;
}


bool ChipRecog::getImage(Rect &rect, Mat &in,int type)
{
	Mat image_ = in(rect);
	Mat image = image_.clone();
	
	uchar *data = image.data;
	int start = 0;
	int end = image.cols;

	vector<int>Xshadows(image.cols, 0);
	for (int i = 0; i < image.cols; ++i)
	{
		for (int j = 0; j < image.rows; ++j)
		{
			if (data[j*image.cols + i] != 0)
				Xshadows[i]++;
		}
	}
	for (int i = 0; i < Xshadows.size() - 2; ++i)
	{
		if (Xshadows[i]>(0.1*image.rows) && Xshadows[i + 1]>(0.1*image.rows) && Xshadows[i + 2]>(0.1*image.rows))
		{
			start = i;
			break;
		}
	}

	for (int i = Xshadows.size() - 1; i >= 0; --i)
	{
		if (Xshadows[i]>(0.1*image.rows))
		{
			end = i;
			break;
		}
	}

	if (start >= end)
		return false;
	rect.x = rect.x + start;
	rect.width = end - start + 1;

	Mat src = in.clone();
	uchar *srcData = src.data;
	int Ystart = rect.y;
	int Yend = rect.y+rect.height;

	if (type == 0)
	{
		for (int i = Ystart; i >= (Ystart - rect.height >= 0 ? Ystart - rect.height : 0); --i)
		{
			int count = 0;
			for (int j = rect.x; j < rect.x + rect.width; ++j)
			{
				if (srcData[i*src.cols + j] != 0)
				{
					count++;
				}
			}
			if (count < 3)
			{
				Ystart = i;
				break;
			}
		}
	}
	else
	{
		for (int i = Ystart + 2; i >= (Ystart - rect.height >= 0 ? Ystart - rect.height : 0); --i)
		{
			int count = 0;
			for (int j = rect.x; j < rect.x + rect.width; ++j)
			{
				if (srcData[i*src.cols + j] != 0 || srcData[(i - 1)*src.cols + j] != 0 || srcData[(i - 2)*src.cols + j] != 0)
				{
					count++;
				}
			}
			if (count < 3)
			{
				Ystart = i;
				break;
			}
		}
	}
	
	
	rect.height = rect.y + rect.height - Ystart;
	rect.y = Ystart;

	int newYbottmo = 0;
	newYbottmo=rect.y + rect.height - 1;

	int end_end = newYbottmo;
	bool change = false;
	for (int i = newYbottmo; i < src.rows - 1 ; ++i)
	{
		int count = 0;
		for (int j = rect.x; j < rect.x + rect.width; ++j)
		{
			if (srcData[i*src.cols + j] != 0)
			{
				count++;
			}
		}
		if (count < 3)
		{
			change = true;
			newYbottmo = i;
			break;
		}
	}

	if (newYbottmo == end_end&&change == false)
	{
		newYbottmo += 5;
	}
	rect.height = newYbottmo - rect.y + 1;


	

	return true;
}





int ChipRecog::FindSpecialChar(string path, Mat &image, Rect &specialRect, SvmTrain &mySvm, int &lengthP, int type)

{
	if (image.channels() == 3)
		cvtColor(image, image, CV_BGR2GRAY);

	

	Mat objectArea = image.rowRange(image.rows / 3, image.rows);
	objectArea = objectArea.colRange(0, image.cols * 2 / 3);


	Mat pic = image.clone();
	cvtColor(pic, pic, CV_GRAY2BGR);
	Mat pic_ = pic.clone();

	Mat image_threshold;


	threshold(image, image_threshold, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);

	if (image.cols < 50 && image.rows < 50)
		return 1;
	
	vector<Rect>cascadeRects;
	vector<Rect>candidates;
	Ptr<cv::CascadeClassifier> cascade;
		path += "\\scu1.xml";

		cascade = makePtr<cv::CascadeClassifier>(path);


		
		cascade->detectMultiScale(objectArea, cascadeRects, 1.1, lengthP, 0, minRect,maxRect);

		Mat image_show = image_threshold.clone();
		Mat image_show_ = image_threshold.clone();
		
		
			for (int i = 0; i < cascadeRects.size(); ++i)
			{

				    Mat temp;
				
					cascadeRects[i].y += image.rows / 3;
					if (!getImage(cascadeRects[i], image_threshold,type))
							continue;
					//rectangle(pic, cascadeRects[i], Scalar(255, 0, 0), 2);
					//rectangle(image_show, cascadeRects[i], Scalar(255, 0, 0), 2);
				
					try
					{
						temp = image(cascadeRects[i]);
					}
					catch (Exception &ex)
					{
						continue;
				    }
					
					
					if (temp.cols >= 20 || temp.rows >= 40)
						resize(temp, temp, Size(20, 40), 0, 0, INTER_AREA);
					else
						resize(temp, temp, Size(20, 40), 0, 0, INTER_CUBIC);
					
					
					
				
					
				if (mySvm.recogData(temp))
				{
					candidates.push_back(cascadeRects[i]);
				    cv::rectangle(pic, cascadeRects[i], Scalar(255, 0, 0), 3);
					//rectangle(image_show_, cascadeRects[i], Scalar(255, 0, 0), 3);

				}

			}
		
			
			
		
		
	
			
	/*
			namedWindow("image_show", WINDOW_NORMAL);
			imshow("image_show", image_show);
			namedWindow("image_show_", WINDOW_NORMAL);
			imshow("image_show_", image_show_);*/
			
	if (candidates.empty())
		return 1;
	if (candidates.size() == 1)
	{
		specialRect = candidates[0];
	}
	else
	{
		sort(candidates.begin(), candidates.end(), [](const Rect &a, const Rect&b){return a.y < b.y; });
		candidates = ajustSpecialRects(candidates);
		sort(candidates.begin(), candidates.end(), [](const Rect &a, const Rect&b){return a.x < b.x; });
		specialRect = candidates[0];
	}

	
	
	/*imshow("image_show_", pic);*/
	
	return 0;

}

bool ChipRecog::getImageOne(Rect &rect, Mat &input)
{
	Mat image = input.clone();
	uchar *data = image.data;
	int start = rect.x;
	if (rect.y > image.rows)
		return false;

	int end = rect.x + rect.width - 1;



	vector<int>Xshadows_first(rect.width, 0);
	for (int i = start; i <= end; ++i)
	{
		for (int j = rect.y; j < (rect.y + rect.height <image.rows - 1 ? rect.y + rect.height : image.rows - 1); ++j)
		{
			if (data[j*image.cols + i] != 0)
			{
				Xshadows_first[i - start]++;
			}
		}
	}

	int max_first = Xshadows_first[0];
	int max_index_first = 0;
	for (int i = 1; i < Xshadows_first.size(); ++i)
	{
		if (Xshadows_first[i] > max_first)
		{
			max_first = Xshadows_first[i];
			max_index_first = i;
		}
	}

	for (int i = max_index_first; i <Xshadows_first.size(); ++i)
	{
		if (Xshadows_first[i] < 3)
		{
			end = i + start;
			break;
		}
	}


	for (int i = max_index_first; i >= 0; --i)
	{
		if (Xshadows_first[i] < 3)
		{
			start = i + start;
			break;
		}
	}





	if (start >= end)
		return false;
	rect.x = start;
	rect.width = end - start + 1;

	vector<int>Yshadow(2 * rect.height, end);
	for (int i = (rect.y - rect.height>0 ? rect.y - rect.height : 0); i < (rect.y + rect.height<image.rows ? rect.y + rect.height : image.rows); ++i)
	{
		for (int j = start; j <= end; ++j)
		{
			if (data[i*image.cols + j] != 0)
			{
				Yshadow[i - rect.y + rect.height] = j;
				break;
			}
		}
	}
	int max = 0;
	int maxVal = Yshadow[0];
	for (int i = 0; i < Yshadow.size() * 3 / 5; ++i)
	{
		if (Yshadow[i] >= maxVal)
		{
			max = i;
			maxVal = Yshadow[i];
		}
	}

	int Ystart = max + rect.y - rect.height;

	rect.height = rect.y + rect.height - Ystart;
	rect.y = Ystart;
	rect.x = start;
	rect.width = end - start + 1;




	int bottomValue = rect.y + rect.height - 1;

	for (int i = bottomValue; i < (rect.y + rect.height * 3 / 2>image.rows ? image.rows : rect.y + rect.height * 3 / 2); ++i)
	{
		int count1 = 0;
		for (int j = rect.x + rect.width; j >= rect.x; j--)
		{
			if (data[i*image.cols + j] != 0)
			{
				count1++;
			}
		}
		if (count1 <3)
		{
			rect.height = i - rect.y;
			break;
		}
	}




	return true;
}

bool judgeRectHaar(Rect &rect)
{
	if (rect.x - rect.width - 4 < 0)
		return false;
	if (rect.area() < 1000)
		return false;
	if (rect.width >rect.height)
		return false;
	double ratio = (double)rect.height / (double)rect.width;
	if (ratio < 1.8)
		return false;
	return true;

}
bool judgeSingleChars(Rect rect, Mat &image,int width)
{
	if (rect.x < 0||rect.x>image.cols)
		return false;
	if (rect.y < 0||rect.y>image.rows)
		return false;
	if (rect.width <=width/4||rect.width>image.cols)
		return false;
	if (rect.height <=1 || rect.height>image.rows)
		return false;
	
	Mat audix = image(rect);
	Mat temp = audix.clone();
	int area = rect.width*rect.height;
	int count = 0;//所有行总共的白色像素点
	int number = 0;//有白色像素点的行数
	vector<int>Yshadow(audix.rows, 0);
	for (int i = 0; i < audix.rows; ++i)
	{
		uchar *data = temp.ptr<uchar>(i);
		for (int j = 0; j < audix.cols; ++j)
		{
			if (data[j] != 0)
			{
				count++;
				Yshadow[i]++;//每行白色像素点
			}
		}
		if (Yshadow[i]>1)
		{
			number++;
		}
	}
	double ratix = (double)count / (double)area;//区域的平均白色像素点
	if (ratix >= 0.1&&number>rect.height / 3)
		return true;
	return false;
}

bool findStart(vector<int>&arrays, int index)//arrays：存放着每列白色点数的矩阵；index：第几列；
{
	if (arrays[index] < 2)
		return false;
	if (index >= 4 && index <= arrays.size() - 5)//在第四列到最后之间
	{
		int countR = 0;
		for (int i = index + 1; i <= index + 4; i++)
		{
			if (arrays[i] >= 2)
				countR++;//第i列右边四列中白色（可能为数字）的列数
		}
		int countL = 0;
		for (int i = index - 1; i >= index - 4; i--)
		{
			if (arrays[i] < 2)
				countL++;//第i列左边四列黑色的（不为数字）的列数
		}
		if (countR >= 3 && countL >= 1)
		{
			return true;
		}
		return false;
	}
	else if (index < 4)
	{
		int countR = 0;
		for (int i = index + 1; i <= index + 4; i++)
		{
			if (arrays[i] >= 2)
				countR++;
		}

		int countL = 0;
		for (int i = 0; i <= index - 1; i++)
		{
			if (arrays[i] < 2)
				countL++;
		}

		if (countR >= 3 && countL >= index - 1)
		{
			return true;
		}
		return false;
	}



}

bool findEnd(vector<int>&arrays, int &index)
{
	if (index <= arrays.size() - 5 && arrays[index] >= 2)
		return false;
	if (index >= 4 && index <= arrays.size() - 5)
	{
		int countR = 0;
		for (int i = index + 1; i <= index + 4; i++)
		{
			if (arrays[i] < 2)//右边4列为黑色
				countR++;
		}
		int countL = 0;
		for (int i = index - 1; i >= index - 4; i--)
		{
			if (arrays[i] >= 2)//左边四列为白色
				countL++;
		}
		if (countR >= 1 && countL >= 3)
		{
			return true;
		}
		return false;
	}
	else if (index < 4)//遇不到 index=i+4 >=4
	{
		int countR = 0;
		for (int i = index + 1; i <= index + 4; i++)
		{
			if (arrays[i] < 2)
				countR++;
		}

		int countL = 0;
		for (int i = 0; i <= index - 1; i++)
		{
			if (arrays[i] >= 2)
				countL++;
		}

		if (countR >= 3 && countL >= index - 1)
		{
			return true;
		}
		return false;
	}
	else if (index > arrays.size() - 5)//基本不会遇到？ arrays.size()1000左右，数字出现的位置在500之前；
	{
		int countR = 0;
		for (int i = index + 1; i <arrays.size(); ++i)
		{
			if (arrays[i] < 2)
				countR++;
		}
		int countL = 0;
		for (int i = index - 1; i >= index - 4; i--)
		{
			if (arrays[i] >= 2)
				countL++;
		}
		if (countR >= arrays.size() - 1 - index - 1 && countL >= 3)
		{
			return true;
		}
		return false;
	}
}
bool ChipRecog::getNextRect(Mat &image_, Rect &rect, Mat &compareImage_, int width)
//image后面还未截出数字的图片 在循环里每次跟新；rect：标准的数字矩形框；compareImage_:所有数字所在行图片；width：标准矩形框宽度
{
	Mat image = image_.clone();
	Mat compareImage = compareImage_.clone();
	//imwrite("1.jpg", image);
	//imwrite("2.jpg", compareImage);

	vector<int>Xshadow(image.cols, 0);
	uchar *data = image.data;
	for (int i = 0; i < image.cols; ++i)
	{

		for (int j = 0; j < image.rows * 3 / 4; ++j)
		{
			if (data[j*image.cols + i] != 0)
				Xshadow[i]++;//每一列白色点数
		}
	}

	Point pt;
	bool stop = false;
	for (int i = 0; i < Xshadow.size(); ++i)
	{
		if (findStart(Xshadow, i))//i列为数字的起点
		{
			pt.x = i;
			i = i + 4;
			for (int j = i; j < Xshadow.size(); ++j)
			{
				if (findEnd(Xshadow, j))//j列为数字的终点
				{
					pt.y = j;
					j = j + 4;
					i = j;//？
					stop = true;
					break;
				}
			}

		}

		if (stop == true)
			break;
	}

	Rect temp = Rect(pt.x + rect.x + rect.width, rect.y, pt.y - pt.x + 1, rect.height);//截出只有数字的框
	Mat candidate = compareImage(temp);
	//candidate = preprocessChar(candidate);
	//imwrite("1111.jpg", candidate);

	rect = temp;
	if (!judgeSingleChars(temp, compareImage, 1))//是否为单个数字
	{
		return false;
	}
	int countNumber = 0;
	countNumber = (double)temp.width / (double)width + 0.5;

	if (countNumber <= 1)//框出来的准确数字小于标准框0.5
	{
		uchar *dataC = compareImage.data;
		int top = temp.y;
		int bottom = temp.y + temp.height;
		int length = temp.height / 3;
		int left = temp.x;

		vector<int>Yborder(compareImage.rows, left + temp.width);
		for (int i = 0; i < compareImage.rows; ++i)
		{
			for (int j = left; j < left + temp.width; ++j)
			{
				if (dataC[i*compareImage.cols + j] != 0)
				{
					Yborder[i] = j;//每行白色点数
					break;
				}
			}
		}



		for (int i = top; i < bottom; ++i)
		{
			int count = 0;
			uchar *ddata = compareImage.ptr<uchar>(i);
			for (int j = left; j < left + temp.width; ++j)
			{
				if (ddata[j] != 0)
				{
					count++;
				}
			}
			if (count >= 2)
			{
				top = i;//刚刚有白色点最上面的行数
				break;
			}
		}

		for (int i = bottom - 1; i >= top; --i)
		{
			int count = 0;
			uchar *ddata = compareImage.ptr<uchar>(i);
			for (int j = left; j <left + temp.width; ++j)
			{
				if (ddata[j] != 0)
					count++;
			}
			if (count >= 2)
			{
				bottom = i + 1;//刚刚有白色点最下面的行数
				break;
			}
		}

		int maxIndex = top;
		int maxValue = Yborder[top];
		for (int i = 0; i < top + 5; ++i)
		{
			if (Yborder[i] >= maxValue)
			{
				maxIndex = i;
				maxValue = Yborder[i];
			}
		}
		top = maxIndex;//上方白色点最多的一行（数字上界）

		maxIndex = bottom - 1;
		maxValue = Yborder[maxIndex];
		for (int i = bottom - 1; i < compareImage.rows; ++i)
		{
			if (Yborder[i] > maxValue)
			{
				maxIndex = i;
				maxValue = Yborder[i];
			}
		}
		bottom = maxIndex;//下方白色点最多的一行（数字下界）


		rect.x = temp.x;
		rect.y = top;
		rect.height = bottom - top;
		rect.width = temp.width;
	}
	else//没有用？
	{
		uchar *dataC = compareImage.data;
		int top = temp.y;
		int bottom = temp.y + temp.height;
		int left = temp.x;
		vector<int>Xborder(temp.width, temp.y + temp.height - 1);
		for (int i = left; i < left + temp.width; ++i)
		{
			for (int j = top; j < bottom; ++j)
			{
				if (dataC[j*compareImage.cols + i] != 0)
				{
					Xborder[i - temp.x] = j;
					break;
				}
			}
		}

		int divide = 0;
		int max = 0;

		for (int j = width*0.6; j < width*1.4; ++j)
		{
			if (Xborder[j] > max)
			{
				max = Xborder[j];
				divide = j;
			}
		}

		Rect rect1;
		if (divide == 0)
		{
			rect1 = Rect(temp.x, temp.y, temp.width / countNumber, temp.height);
		}
		else
		{
			rect1 = Rect(temp.x, temp.y, divide + 1, temp.height);
		}





		top = rect1.y;
		bottom = rect1.y + rect1.height;
		left = rect1.x;

		vector<int>Yborder(compareImage.rows, left + rect1.width);
		for (int i = 0; i < compareImage.rows; ++i)
		{
			for (int j = left; j < left + rect1.width; ++j)
			{
				if (dataC[i*compareImage.cols + j] != 0)
				{
					Yborder[i] = j;
					break;
				}
			}
		}



		for (int i = top; i < bottom; ++i)
		{
			int count = 0;
			uchar *ddata = compareImage.ptr<uchar>(i);
			for (int j = left; j < left + rect1.width; ++j)
			{
				if (ddata[j] != 0)
				{
					count++;
				}
			}
			if (count >= 2)
			{
				top = i;
				break;
			}
		}

		for (int i = bottom - 1; i >= top; --i)
		{
			int count = 0;
			uchar *ddata = compareImage.ptr<uchar>(i);
			for (int j = left; j <left + rect1.width; ++j)
			{
				if (ddata[j] != 0)
					count++;
			}
			if (count >= 2)
			{
				bottom = i + 1;
				break;
			}
		}

		int maxIndex = top;
		int maxValue = Yborder[top];
		for (int i = 0; i < top + 5; ++i)
		{
			if (Yborder[i] >= maxValue)
			{
				maxIndex = i;
				maxValue = Yborder[i];
			}
		}
		top = maxIndex;

		maxIndex = bottom - 1;
		maxValue = Yborder[maxIndex];
		for (int i = bottom - 1; i < compareImage.rows; ++i)
		{
			if (Yborder[i] > maxValue)
			{
				maxIndex = i;
				maxValue = Yborder[i];
			}
		}
		bottom = maxIndex;


		rect.x = rect1.x;
		rect.y = top;
		rect.height = bottom - top;
		rect.width = rect1.width;



	}



	return true;
}

vector<Rect> adjustRect(vector<Rect>&rects)
{
	vector<int>indexs(rects.size(), 0);
	vector<Rect>new_rect;
	for (int i = 0; i < rects.size() - 1; ++i)
	{
		for (int j = i + 1; j < rects.size(); ++j)
		{
			if (rects[j].y - rects[i].y - rects[i].height * 3 / 4 >= 0)
			{
				indexs[i] = 1;
				break;
			}
		}
	}

	for (int i = 0; i < rects.size(); ++i)
	{
		if (indexs[i] == 0)
			new_rect.push_back(rects[i]);
	}

	return new_rect;
}
vector<Rect> adjustRectMid(vector<Rect>&rects)
{
	vector<Rect>new_rect;
	for (int i = 0; i < rects.size(); ++i)
	{
		double ratio = (double)rects[i].height / (double)rects[i].width;
		if (ratio < 1.8)
			continue;
		new_rect.push_back(rects[i]);
	}
	sort(new_rect.begin(), new_rect.end(), [](const Rect &a, const Rect&b){return a.height > b.height; });
	return new_rect;
}
int ChipRecog:: FindNumberOne(string path, cv::Mat &image, cv::Rect &specialRect, network<sequential> &nn, int &lengthP, int type)
{
	if (image.channels() == 3)
		cvtColor(image, image, CV_BGR2GRAY);

	Mat objectArea = image.rowRange(image.rows / 2, image.rows);
	objectArea = objectArea.colRange(0, image.cols * 1 / 2);

	Mat pic = image.clone();
	cvtColor(pic, pic, CV_GRAY2BGR);
	Mat pic_ = pic.clone();

	Mat image_threshold;


	threshold(image, image_threshold, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);

	vector<Rect>cascadeRects;
	vector<Rect>candidates;

	Ptr<cv::CascadeClassifier> cascade;

	cascade = makePtr<cv::CascadeClassifier>(path);
	 if (type==0)
	cascade->detectMultiScale(objectArea, cascadeRects, 1.2, 5, 0, Size(50, 50), Size(55, 55));
	if (type == 1)
		cascade->detectMultiScale(objectArea, cascadeRects, 1.2, 10, 0, Size(60, 60), Size(65, 65));
	if (type == 2)
		cascade->detectMultiScale(objectArea, cascadeRects, 1.2, 10, 0, Size(110, 110), Size(130, 130));
	for (int i = 0; i < cascadeRects.size(); ++i)
	{
		cascadeRects[i].y = cascadeRects[i].y + image.rows / 2;
		if (!getImageOne(cascadeRects[i], image_threshold))
			continue;
		if (cascadeRects[i].x < 0 || cascadeRects[i].x >= image.rows || cascadeRects[i].y < 0 || cascadeRects[i].y >= image.rows
			|| cascadeRects[i].width < 0 || cascadeRects[i].height < 0)
			continue;
		candidates.push_back(cascadeRects[i]);
	}

	vector<Rect>candidates_2;

	Mat image_th_copy = image_threshold.clone();

	Mat objectArea1 = image_th_copy.rowRange(image.rows / 2, image.rows);
	objectArea1 = objectArea1.colRange(0, image.cols * 1 / 2);
	vector<vector<Point>>contours;
	findContours(objectArea1, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	auto it = contours.begin();
	while (it != contours.end())
	{
		Rect temp = boundingRect(*it);
		temp.y += image.rows / 2;
		candidates.push_back(temp);
		it++;
	}
	for (int i = 0; i < candidates.size(); ++i)
	{
		//rectangle(pic, candidates[i], Scalar(255, 0, 0), 2);
		if (judgeRectHaar(candidates[i]))
		{
			//rectangle(pic, candidates[i], Scalar(0, 255, 0), 2);
			Mat tempIm = image_threshold(candidates[i]);
			vec_t input;
			if (tempIm.cols >= 32 || tempIm.rows >= 32)
				resize(tempIm, tempIm, Size(32, 32), 0, 0, INTER_AREA);
			else
				resize(tempIm, tempIm, Size(32, 32), 0, 0, INTER_CUBIC);

			convertImage(tempIm, input);
			int ooo = -1;
			recogNumber(nn, input, ooo);
			if (ooo == 1)
			{
				//rectangle(pic, candidates[i], Scalar(0, 0, 255), 2);
				candidates_2.push_back(candidates[i]);
			}
		}
	}


	if (candidates_2.empty())
		return 1;
	if (candidates_2.size() == 1)
	{
		specialRect = candidates_2[0];
		//rectangle(pic, candidates[0], Scalar(0, 255, 0), 2);
		int top_sp = specialRect.y - specialRect.height / 3 > 0 ? specialRect.y - specialRect.height / 3 : 0;

		Rect tempR = Rect(specialRect.x, top_sp,
			specialRect.width, specialRect.y + specialRect.height - 1 + specialRect.height / 3 < image_threshold.rows ? 5 * specialRect.height / 3 : image_threshold.rows - top_sp);
		Mat tempM = image_threshold.rowRange(top_sp, top_sp + tempR.height);
		int top_new = specialRect.y - top_sp;

		int bottom = top_new + specialRect.height;
		Mat object = tempM.rowRange(top_new, bottom);
		object = object.colRange(specialRect.x + specialRect.width, object.cols);
		Mat image1 = object.clone();
		Mat showshow = tempM.clone();
		Rect charRect = Rect(specialRect.x, top_new, specialRect.width, specialRect.height);
		if (getNextRect(image1, charRect, showshow, specialRect.width * 2))
		{
			charRect.y += top_sp;
			specialRect = charRect;
			//rectangle(pic, charRect, Scalar(0, 255, 0), 2);
		}
		else
		{
			image1 = tempM.rowRange(charRect.y, charRect.y + charRect.height);
			image1 = image1.colRange(charRect.x + charRect.width, tempM.cols);
			getNextRect(image1, charRect, showshow, specialRect.width * 2);
			charRect.y += top_sp;
			specialRect = charRect;
		}
		//rectangle(pic, charRect, Scalar(0, 255, 0), 2);
	}
	else
	{
		sort(candidates_2.begin(), candidates_2.end(), [](const Rect &a, const Rect&b){return a.y < b.y; });
		candidates_2 = adjustRect(candidates_2);
		sort(candidates_2.begin(), candidates_2.end(), [](const Rect &a, const Rect&b){return a.x < b.x; });
		candidates_2 = adjustRectMid(candidates_2);
		if (candidates_2.empty())
			return 1;
		specialRect = candidates_2[0];
		//rectangle(pic, candidates[0], Scalar(0, 255, 0), 2);
		int top_sp = specialRect.y - specialRect.height / 3 > 0 ? specialRect.y - specialRect.height / 3 : 0;

		Rect tempR = Rect(specialRect.x, top_sp,
			specialRect.width, specialRect.y + specialRect.height - 1 + specialRect.height / 3 < image_threshold.rows ? 5 * specialRect.height / 3 : image_threshold.rows - top_sp);
		Mat tempM = image_threshold.rowRange(top_sp, top_sp + tempR.height);
		int top_new = specialRect.y - top_sp;

		int bottom = top_new + specialRect.height;
		Mat object = tempM.rowRange(top_new, bottom);
		object = object.colRange(specialRect.x + specialRect.width, object.cols);
		Mat image1 = object.clone();
		Mat showshow = tempM.clone();
		Rect charRect = Rect(specialRect.x, top_new, specialRect.width, specialRect.height);
		if (getNextRect(image1, charRect, showshow, specialRect.width * 2))
		{
			charRect.y += top_sp;
			specialRect = charRect;
			//rectangle(pic, charRect, Scalar(0, 255, 0), 2);
		}
		else
		{
			image1 = tempM.rowRange(charRect.y, charRect.y + charRect.height);
			image1 = image1.colRange(charRect.x + charRect.width, tempM.cols);
			getNextRect(image1, charRect, showshow, specialRect.width * 2);
			charRect.y += top_sp;
			specialRect = charRect;
		}

	}
	/*rectangle(pic, specialRect, Scalar(0, 255, 0), 2);
	namedWindow("111", WINDOW_NORMAL);
	imshow("111", pic);*/
	return 0;

}




bool dealAllChars(vector<Rect>&rects, Rect &specialRect, Mat &image_)
{
	Mat image = image_.clone();

	int length = specialRect.height / 3;
	for (int i = 0; i < rects.size(); ++i)
	{
		uchar *data = image.data;

		int top = specialRect.y;
		int left = rects[i].x + specialRect.x + specialRect.width;
		int bottom = specialRect.y + rects[i].height;

		for (int m = top; m >= (top - length>0 ? top - length : 0); --m)
		{
			int count = 0;
			for (int n = left; n < left + rects[i].width; ++n)
			{
				if (data[m*image.cols + n] != 0)
					count++;
			}
			if ((double)count / (double)rects[i].width < 0.1)
			{
				top = m;
				break;
			}
		}

		for (int m = bottom; m < (bottom + length<image.rows ? bottom + length : image.rows); ++m)
		{
			int count = 0;
			for (int n = left; n < left + rects[i].width; ++n)
			{
				if (data[m*image.cols + n] != 0)
					count++;
			}
			if ((double)count / (double)rects[i].width < 0.1)
			{
				bottom = m;
				break;
			}
		}

		for (int m = top; m <= bottom; ++m)
		{
			int count = 0;
			for (int n = left; n < left + rects[i].width; ++n)
			{
				if (data[m*image.cols + n] != 0)
					count++;
			}
			if ((double)count / (double)rects[i].width > 0.1)
			{
				top = m;
				break;
			}
		}


		for (int m = bottom; m >= top; --m)
		{
			int count = 0;
			for (int n = left; n < left + rects[i].width; ++n)
			{
				if (data[m*image.cols + n] != 0)
					count++;
			}
			if ((double)count / (double)rects[i].width > 0.1)
			{
				bottom = m;
				break;
			}
		}

		rects[i].x = left;
		rects[i].y = top;
		rects[i].height = bottom - top + 1;

	}


	return true;
}


int preprocessChar2(Mat &in)
{

	threshold(in, in, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);
	int h = in.rows;
	int w = in.cols;

	//统一每个字符的大小

	int charSize = 32;


	if (in.cols >= 32 || in.rows >= 32)
		resize(in, in, Size(charSize, charSize), 0, 0, INTER_AREA);
	else
		resize(in, in, Size(charSize, charSize), 0, 0, INTER_CUBIC);

	return 0;

}


Mat ChipRecog::preprocessChar(Mat in)
{
	int h = in.rows;
	int w = in.cols;

	//统一每个字符的大小

	int charSize = 20;
	double ratio = (double)h / (double)w;
	if (ratio > 1.5)
	{
		Mat transformMat = Mat::eye(2, 3, CV_32F);
		int m = max(w, h);
		transformMat.at<float>(0, 2) = float(m / 2 - w / 2);
		transformMat.at<float>(1, 2) = float(m / 2 - h / 2);

		Mat warpImage(m, m, in.type());

		warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR,
			BORDER_CONSTANT, Scalar(0));

		//！ 将所有的字符调整成统一的尺寸

		Mat out;
		resize(warpImage, out, Size(charSize, charSize));

		return out;
	}
	else
	{
		Mat transformMat = Mat::eye(2, 3, CV_32F);
		int m = max(w, h);
		transformMat.at<float>(0, 2) = float(m - w / 2);
		transformMat.at<float>(1, 2) = float(m / 2 - h / 2);

		Mat warpImage(m, 2 * m, in.type());

		warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR,
			BORDER_CONSTANT, Scalar(0));

		//！ 将所有的字符调整成统一的尺寸

		Mat out;
		resize(warpImage, out, Size(charSize, charSize));

		return out;
	}
}

int ChipRecog::chipRecogObject(Mat &in, Mat &chipImage, string &s, string &special, AnnTrain &at, Rect &specialRect, int number)
{
	Mat image = in.clone();


	vector<Point>border;
	vector<Rect>charRects;
	vector<int>Xshadow(image.cols, 0);
	uchar *data = image.data;
	for (int i = 0; i < image.cols; ++i)
	{

		for (int j = 0; j < image.rows; ++j)
		{
			if (data[j*image.cols + i] != 0)
				Xshadow[i]++;
		}
	}

	for (int i = 0; i < Xshadow.size(); ++i)
	{
		Point pt;
		if (findStart(Xshadow, i))
		{
			pt.x = i;
			i = i + 4;
			for (int j = i; j < Xshadow.size(); ++j)
			{
				if (findEnd(Xshadow, j))
				{
					pt.y = j;
					j = j + 4;
					i = j;
					border.push_back(pt);
					break;
				}
			}
		}
	}

	for (int i = 0; i < border.size(); ++i)
	{
		Rect rect;
		rect.x = border[i].x;
		rect.y = 0;
		rect.height = image.rows;
		rect.width = border[i].y - border[i].x + 1;
		if (judgeSingleChars(rect, image,1)/*&&rect.x>specialRect.width/2*/)
		{
			charRects.push_back(rect);
		}

	}


	Mat numberSpecial = chipImage(specialRect);
	Mat temp = numberSpecial.clone();
	int start = 0;
	int end = 0;
	uchar *dataT = temp.data;
	for (int j = 0; j < numberSpecial.cols; ++j)
	{
		int count = 0;
		for (int i = 0; i < numberSpecial.rows; ++i)
		{
			if (dataT[i*numberSpecial.cols + j] != 0)
				count++;
		}
		if (count>5)
		{
			start = j;
			break;
		}
	}

	for (int j = numberSpecial.cols; j >= 0; --j)
	{
		int count = 0;
		for (int i = 0; i < numberSpecial.rows; ++i)
		{
			if (dataT[i*numberSpecial.cols + j] != 0)
				count++;
		}
		if (count>5)
		{
			end = j;
			break;
		}
	}
	if (start<end)
		temp = temp.colRange(start, end);

	int width = temp.cols;


	std::sort(charRects.begin(), charRects.end(), [](const Rect& r1, const Rect& r2) { return r1.x < r2.x; });




	vector<Rect>FinalRects;
	for (int i = 0; i < charRects.size(); ++i)
	{
		if (charRects[i].width < 1.5*width)
		{


			FinalRects.push_back(charRects[i]);
			if (FinalRects.size() >= number - 2)
				break;
		}
		else if (charRects[i].width < 2.5*width&&charRects[i].width >= 1.5*width)
		{
			Rect tRect1, tRect2;
			tRect1 = Rect(charRects[i].x, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect1);
			if (FinalRects.size() >= number - 2)
				break;
			tRect2 = Rect(charRects[i].x + width, charRects[i].y, charRects[i].width - width, charRects[i].height);
			FinalRects.push_back(tRect2);
			if (FinalRects.size() >= number - 2)
				break;

		}
		else if (charRects[i].width < 3.5*width&&charRects[i].width >= 2.5*width)
		{
			Rect tRect1, tRect2, tRect3;
			tRect1 = Rect(charRects[i].x, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect1);
			if (FinalRects.size() >= number - 2)
				break;
			tRect2 = Rect(charRects[i].x + width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect2);
			if (FinalRects.size() >= number - 2)
				break;
			tRect3 = Rect(charRects[i].x + 2 * width, charRects[i].y, charRects[i].width - 2 * width, charRects[i].height);
			FinalRects.push_back(tRect3);
			if (FinalRects.size() >= number - 2)
				break;
		}
		else if (charRects[i].width < 4.5*width&&charRects[i].width >= 3.5*width)
		{
			Rect tRect1, tRect2, tRect3, tRect4;
			tRect1 = Rect(charRects[i].x, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect1);
			if (FinalRects.size() >= number - 2)
				break;
			tRect2 = Rect(charRects[i].x + width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect2);
			if (FinalRects.size() >= number - 2)
				break;
			tRect3 = Rect(charRects[i].x + 2 * width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect3);
			if (FinalRects.size() >= number - 2)
				break;
			tRect4 = Rect(charRects[i].x + 3 * width, charRects[i].y, charRects[i].width - 3 * width, charRects[i].height);
			FinalRects.push_back(tRect4);
			if (FinalRects.size() >= number - 2)
				break;
		}
		else if (charRects[i].width < 5.5*width&&charRects[i].width >= 4.5*width)
		{
			Rect tRect1, tRect2, tRect3, tRect4, tRect5;
			tRect1 = Rect(charRects[i].x, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect1);
			if (FinalRects.size() >= number - 2)
				break;
			tRect2 = Rect(charRects[i].x + width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect2);
			if (FinalRects.size() >= number - 2)
				break;
			tRect3 = Rect(charRects[i].x + 2 * width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect3);
			if (FinalRects.size() >= number - 2)
				break;
			tRect4 = Rect(charRects[i].x + 3 * width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect4);
			if (FinalRects.size() >= number - 2)
				break;
			tRect5 = Rect(charRects[i].x + 4 * width, charRects[i].y, charRects[i].width - 4 * width, charRects[i].height);
			FinalRects.push_back(tRect5);
			if (FinalRects.size() >= number - 2)
				break;
		}
		else if (charRects[i].width < 6.5*width&&charRects[i].width >= 5.5*width)
		{
			Rect tRect1, tRect2, tRect3, tRect4, tRect5, tRect6;
			tRect1 = Rect(charRects[i].x, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect1);
			if (FinalRects.size() >= number - 2)
				break;
			tRect2 = Rect(charRects[i].x + width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect2);
			if (FinalRects.size() >= number - 2)
				break;
			tRect3 = Rect(charRects[i].x + 2 * width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect3);
			if (FinalRects.size() >= number - 2)
				break;
			tRect4 = Rect(charRects[i].x + 3 * width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect4);
			if (FinalRects.size() >= number - 2)
				break;
			tRect5 = Rect(charRects[i].x + 4 * width, charRects[i].y, charRects[i].width, charRects[i].height);
			FinalRects.push_back(tRect5);
			if (FinalRects.size() >= number - 2)
				break;
			tRect6 = Rect(charRects[i].x + 5 * width, charRects[i].y, charRects[i].width - 5 * width, charRects[i].height);
			FinalRects.push_back(tRect6);
			if (FinalRects.size() >= number - 2)
				break;
		}
		else if (charRects[i].width < 7.5*width&&charRects[i].width >= 6.5*width)
		{
			Rect tRect1, tRect2, tRect3, tRect4, tRect5, tRect6, tRect7;
			tRect1 = Rect(charRects[i].x, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect1);
			if (FinalRects.size() >= number - 2)
				break;
			tRect2 = Rect(charRects[i].x + width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect2);
			if (FinalRects.size() >= number - 2)
				break;
			tRect3 = Rect(charRects[i].x + 2 * width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect3);
			if (FinalRects.size() >= number - 2)
				break;
			tRect4 = Rect(charRects[i].x + 3 * width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect4);
			if (FinalRects.size() >= number - 2)
				break;
			tRect5 = Rect(charRects[i].x + 4 * width, charRects[i].y, charRects[i].width, charRects[i].height);
			FinalRects.push_back(tRect5);
			if (FinalRects.size() >= number - 2)
				break;
			tRect6 = Rect(charRects[i].x + 5 * width, charRects[i].y, charRects[i].width, charRects[i].height);
			FinalRects.push_back(tRect6);
			if (FinalRects.size() >= number - 2)
				break;
			tRect7 = Rect(charRects[i].x + 6 * width, charRects[i].y, charRects[i].width - 6 * width, charRects[i].height);
			FinalRects.push_back(tRect7);
			if (FinalRects.size() >= number - 2)
				break;
		}
		else if (charRects[i].width < 8.5*width&&charRects[i].width >= 7.5*width)
		{
			Rect tRect1, tRect2, tRect3, tRect4, tRect5, tRect6, tRect7, tRect8;
		  	tRect1 = Rect(charRects[i].x, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect1);
			if (FinalRects.size() >= number - 2)
				break;
			tRect2 = Rect(charRects[i].x + width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect2);
			if (FinalRects.size() >= number - 2)
				break;
			tRect3 = Rect(charRects[i].x + 2 * width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect3);
			if (FinalRects.size() >= number - 2)
				break;
			tRect4 = Rect(charRects[i].x + 3 * width, charRects[i].y, width, charRects[i].height);
			FinalRects.push_back(tRect4);
			if (FinalRects.size() >= number - 2)
				break;
			tRect5 = Rect(charRects[i].x + 4 * width, charRects[i].y, charRects[i].width, charRects[i].height);
			FinalRects.push_back(tRect5);
			if (FinalRects.size() >= number - 2)
				break;
			tRect6 = Rect(charRects[i].x + 5 * width, charRects[i].y, charRects[i].width, charRects[i].height);
			FinalRects.push_back(tRect6);
			if (FinalRects.size() >= number - 2)
				break;
			tRect7 = Rect(charRects[i].x + 6 * width, charRects[i].y, charRects[i].width, charRects[i].height);
			FinalRects.push_back(tRect7);
			if (FinalRects.size() >= number - 2)
				break;
			tRect8 = Rect(charRects[i].x + 7 * width, charRects[i].y, charRects[i].width - 7 * width, charRects[i].height);
			FinalRects.push_back(tRect8);
			if (FinalRects.size() >= number - 2)
				break;
		}
	}


	dealAllChars(FinalRects, specialRect, chipImage);





	s = s + special;




	for (int i = 0; i < FinalRects.size(); ++i)
	{
		Mat candidateChar = chipImage(FinalRects[i]);

		candidateChar = preprocessChar(candidateChar);
		s += at.recogData(candidateChar);
	}


	return 0;
}

int ChipRecog::chipRecogSegment(Mat &input, Rect &specialRect, string &s,/* AnnTrain &at,*/network<sequential> &nn, int number,int value)
{
	int top_sp = specialRect.y - specialRect.height / 3 > 0 ? specialRect.y - specialRect.height / 3 : 0;

	Rect tempR = Rect(specialRect.x, top_sp,
		specialRect.width, specialRect.y + specialRect.height - 1 + specialRect.height / 3 < input.rows ? 5 * specialRect.height / 3 : input.rows - top_sp);
	Mat tempM = input.rowRange(top_sp, top_sp + tempR.height);

	int top_new = specialRect.y - top_sp;

	int bottom = top_new + specialRect.height;

	Mat object = tempM.rowRange(top_new, bottom);
	object = object.colRange(specialRect.x + specialRect.width, object.cols);

	threshold(tempM, tempM, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);
	Mat showshow = tempM.clone();
	Mat image = object.clone();


	vector<Point>border;
	vector<Rect>charRects;
	vector<int>Xshadow(image.cols, 0);
	uchar *data = image.data;
	for (int i = 0; i < image.cols; ++i)
	{

		for (int j = 0; j < image.rows; ++j)
		{
			if (data[j*image.cols + i] != 0)
				Xshadow[i]++;
		}
	}

	for (int i = 0; i < Xshadow.size(); ++i)
	{
		Point pt;
		if (findStart(Xshadow, i))
		{
			pt.x = i;
			i = i + 4;
			for (int j = i; j < Xshadow.size(); ++j)
			{
				if (findEnd(Xshadow, j))
				{
					pt.y = j;
					j = j + 4;
					i = j;
					border.push_back(pt);
					break;
				}
			}
		}
	}

	for (int i = 0; i < border.size(); ++i)
	{
		Rect rect;

		rect.x = border[i].x;
		rect.y = 0;
		rect.height = image.rows;
		rect.width = border[i].y - border[i].x + 1;
		if (judgeSingleChars(rect, image,1))
		{
			charRects.push_back(rect);
		}

	}




	Mat new_contours = image.clone();
	uchar *data_new = new_contours.data;
	vector<int>X_contours(image.cols, image.cols - 1);
	for (int i = 0; i < new_contours.cols; ++i)
	{
		for (int j = 0; j < new_contours.rows; ++j)
		{
			if (data[j*new_contours.cols + i] != 0)
			{
				//data_new[j*new_contours.cols + i] = 255;
				X_contours[i] = j;
				break;
			}
		}
	}
	/*for (int i = 0; i < new_contours.cols; ++i)
	{
	for (int j = new_contours.rows-1; j >= 0; --j)
	{
	if (data[j*new_contours.cols + i] != 0)
	{
	data_new[j*new_contours.cols + i] = 255;
	break;
	}
	}
	}*/
	vector<Rect>FinalRects;
	sort(charRects.begin(), charRects.end(), [](const Rect &a, const Rect&b){return a.x < b.x; });




	for (int i = 0; i < charRects.size(); ++i)
	{
		int countNumber = 0;
		countNumber = (double)charRects[i].width / (double)specialRect.width + 0.5;
		if (countNumber <= 1)
		{
			FinalRects.push_back(charRects[i]);
			if (FinalRects.size() >= number - 2)
			{
				break;
			}
		}
		else if (countNumber == 2)
		{
			int divide = charRects[i].x+charRects[i].width / 2;
			int max = 0;
			for (int j = charRects[i].x + specialRect.width*0.6; j < charRects[i].x + specialRect.width*1.4; ++j)
			{
				if (X_contours[j] > max)
				{
					max = X_contours[j];
					divide = j;
				}
			}			
		}
		else if (countNumber == 3)
		{
			int divide1 = charRects[i].x + charRects[i].width / 3, divide2 = charRects[i].x + charRects[i].width * 2 / 3;
			int max1 = 0, max2 = 0;
			for (int j = charRects[i].x + specialRect.width*0.6; j < charRects[i].x + specialRect.width*1.4; ++j)
			{
				if (X_contours[j]>max1)
				{
					max1 = X_contours[j];
					divide1 = j;
				}
			}
			
			for (int j = charRects[i].x + specialRect.width*1.6; j < charRects[i].x + specialRect.width*2.4; ++j)
			{
				if (X_contours[j] > max2)
				{
					max2 = X_contours[j];
					divide2 = j;
				}
			}

			
			Rect rect3 = Rect(divide2, charRects[i].y, charRects[i].x + charRects[i].width - divide2, charRects[i].height);
			FinalRects.push_back(rect3);
			if (FinalRects.size() >= number - 2)
				break;
		}
		else if (countNumber == 4)
		{
			int divide1 = charRects[i].x + charRects[i].width / 4, divide2 = charRects[i].x + charRects[i].width / 2, divide3 = charRects[i].x + charRects[i].width*3 / 4;
			int max1 = 0, max2 = 0, max3 = 0;
			for (int j = charRects[i].x + specialRect.width*0.6; j < charRects[i].x + specialRect.width*1.4; ++j)
			{
				if (X_contours[j] > max1)
				{
					divide1 = j;
					max1 = X_contours[j];
				}
			}
			
			
			for (int j = charRects[i].x + specialRect.width*1.6; j < charRects[i].x + specialRect.width*2.4; ++j)
			{
				if (X_contours[j] > max2)
				{
					divide2 = j;
					max2 = X_contours[j];
				}
			}

			

			for (int j = charRects[i].x + specialRect.width*2.6; j < charRects[i].x + specialRect.width*3.4; ++j)
			{
				if (X_contours[j] > max3)
				{
					divide3 = j;
					max3 = X_contours[j];
				}

			}
			Rect rect3 = Rect(divide2 + 1, charRects[i].y, divide3 - divide2, charRects[i].height);
			FinalRects.push_back(rect3);
			if (FinalRects.size() >= number - 2)
				break;
			Rect rect4 = Rect(divide3, charRects[i].y, charRects[i].x + charRects[i].width - divide3, charRects[i].height);
			FinalRects.push_back(rect4);
			if (FinalRects.size() >= number - 2)
				break;
		}
		else
		{
			int divide1 = charRects[i].x + charRects[i].width / 5, 
				divide2 = charRects[i].x + charRects[i].width*2 / 5,
				divide3 = charRects[i].x + charRects[i].width * 3 / 5,
				divide4 = charRects[i].x + charRects[i].width * 4 / 5;
			int max1 = 0, max2 = 0, max3 = 0, max4 = 0;
			for (int j = charRects[i].x + specialRect.width*0.6; j < charRects[i].x + specialRect.width*1.4; ++j)
			{
				if (X_contours[j]>max1)
				{
					max1 = X_contours[j];
					divide1 = j;
				}
			}
			Rect rect1 = Rect(charRects[i].x, charRects[i].y, divide1 - charRects[i].x + 1, charRects[i].height);
			FinalRects.push_back(rect1);
			if (FinalRects.size() >= number - 2)
				break;
			for (int j = charRects[i].x + specialRect.width*1.6; j < charRects[i].x + specialRect.width*2.4; ++j)
			{
				if (X_contours[j] > max2)
				{
					max2 = X_contours[j];
					divide2 = j;
				}

			}

			Rect rect2 = Rect(divide1 + 1, charRects[i].y, divide2 - divide1, charRects[i].height);
			FinalRects.push_back(rect2);
			if (FinalRects.size() >= number - 2)
				break;

			for (int j = charRects[i].x + specialRect.width*2.6; j < charRects[i].x + specialRect.width*3.4; ++j)
			{
				if (X_contours[j] > max3)
				{
					max3 = X_contours[j];
					divide3 = j;
				}
			}
			Rect rect3 = Rect(divide2 + 1, charRects[i].y, divide3 - divide2, charRects[i].height);
			FinalRects.push_back(rect3);
			if (FinalRects.size() >= number - 2)
				break;

			for (int j = charRects[i].x + specialRect.width*3.6; j < charRects[i].x + specialRect.width*4.4; ++j)
			{
				if (X_contours[j] > max4)
				{
					max4 = X_contours[j];
					divide4 = j;
				}
			}

			Rect rect4 = Rect(divide3 + 1, charRects[i].y, divide4 - divide3, charRects[i].height);
			FinalRects.push_back(rect4);
			if (FinalRects.size() >= number - 2)
				break;

			Rect rect5 = Rect(divide4, charRects[i].y, charRects[i].x + charRects[i].width - divide4, charRects[i].height);
			FinalRects.push_back(rect5);
			if (FinalRects.size() >= number - 2)
				break;
		}
	}

	
	for (int i = 0; i < FinalRects.size(); ++i)
	{
		//rectangle(object, FinalRects[i], Scalar(255), 1);

		Rect adRect = Rect(specialRect.x + specialRect.width + FinalRects[i].x,
			FinalRects[i].y + specialRect.height / 3, FinalRects[i].width, FinalRects[i].height);
		try
		{
			dealBoundary(adRect, showshow, value);
		}
		catch (Exception& ex)
		{
			return 1;
		}
		cv::rectangle(showshow, adRect, Scalar(255), 1);
		Mat candidate = showshow(adRect);
		//candidate = preprocessChar(candidate);
		preprocessChar2(candidate);
		//imwrite("lalala" + intTString(i) + ".jpg", candidate);
		Mat new_candidate = candidate.clone();
		vec_t dd;
		convertImage(new_candidate, dd);
		int index1 = 0;
		recogNumber(nn, dd,index1);
		string temp = intTString(index1);
		s += temp;
	}




	
	/*imshow("芯片局部图", tempM);
	imshow("show", showshow);*/
	return 0;
}

int ChipRecog::dealBoundary(Rect &rect, Mat &image,int value)
{
	uchar *data = image.data;
	int top = rect.y;
	int bottom = rect.y + rect.height;
	int length = rect.height / 3;
	int left = rect.x;

	vector<int>Yshadow(rect.height, 0);

	for (int i = top; i < bottom; ++i)
	{
		uchar *ddata = image.ptr<uchar>(i);
		for (int j = left; j < left + rect.width; ++j)
		{
			if (ddata[j] != 0)
				Yshadow[i - top]++;
		}
	}


	for (int i = top; i < bottom; ++i)
	{
		int count = 0;
		uchar *ddata = image.ptr<uchar>(i);
		for (int j = left; j < left + rect.width; ++j)
		{
			if (ddata[j] != 0)
				count++;
		}
		if (count >= 2)
		{
			top = i;
			break;
		}

	}

	for (int i = bottom - 1; i >= top; --i)
	{
		int count = 0;
		uchar *ddata = image.ptr<uchar>(i);
		for (int j = left; j <left + rect.width; ++j)
		{
			if (ddata[j] != 0)
				count++;
		}
		if (count >= 2)
		{
			bottom = i + 1;
			break;
		}
	}

	for (int m = top; m >= (top - length>0 ? top - length : 0); --m)
	{
		int count = 0;
		uchar *ddata = image.ptr<uchar>(m);
		for (int n = left; n < left + rect.width; ++n)
		{
			if (ddata[n] != 0)
				count++;
		}
		if (count<2)
		{
			top = m;
			break;
		}
	}
	for (int m = bottom - 1; m < (bottom + length<image.rows ? bottom + length : image.rows); ++m)
	{
		int count = 0;
		uchar *ddata = image.ptr<uchar>(m);
		for (int n = left; n < left + rect.width; ++n)
		{
			if (ddata[n] != 0)
				count++;
		}
		if (count<2)
		{
			bottom = m + 1;
			break;
		}
	}

	rect.y = top;
	rect.height = bottom - top;

	bool change = true;
	vector<int>Ycount;
	for (int i = top; i < top + rect.height; ++i)
	{
		uchar *data_change = image.ptr<uchar>(i);
		int count = 0;
		for (int j = left; j < left + rect.width; ++j)
		{
			if (data_change[j] != 0)
				count++;
		}

		Ycount.push_back(count);
	}

	int numbers = 0;
	int count_index = 0;
	for (int i = 0; i < Ycount.size()/2; ++i)
	{
		if (Ycount[i] < 2)
		{
			numbers++;
		}
		else
		{
			numbers = 0;
		}

		if (numbers>value)
		{
			while (Ycount[i] < 2 && i < Ycount.size())
			{
				i++;
			}
			count_index = i - 1;
			break;
		}

	}

	if (count_index != 0)
	{
		rect.height = rect.height - count_index;
		rect.y = rect.y + count_index;

	}

	return 0;
}
bool equalImage(Mat &a, Mat b)
{
	if (a.rows != b.rows&&a.cols != b.cols)
	{
		return false;
	}
	Mat aa = a.clone();
	Mat bb = b.clone();
	for (int i = 0; i < a.rows; ++i)
	{
		uchar *dataAA = aa.ptr<uchar>(i);
		uchar *dataBB = bb.ptr<uchar>(i);
		for (int j = 0; j < a.cols; ++j)
		{
			if (dataAA[i] != dataBB[j])
			{
				return false;
			}
		}
	}
	return true;
}


void erzhihua(Mat&src1)
{
	int x_num = 6;
	int y_num = 2;
	if (src1.channels() != 1)
	{
		cvtColor(src1, src1, CV_BGR2GRAY);
	}
	const int height = src1.rows;
	const int width = src1.cols;
	const int spilt_width = width / x_num;
	const int spilt_height = height / y_num;
	int i = 0, j = 0;
	Mat src12;
	Mat src22;
	for (j = 0; j < x_num; j++)
	{
		for (i = 0; i < y_num; i++)
		{
			if (i == y_num - 1 && j == x_num - 1)
			{
				src12 = src1(Range(i*spilt_height, src1.rows), Range(j*spilt_width, src1.cols));
			}
			else if (i == y_num - 1 && j != x_num - 1)
			{
				src12 = src1(Range(i*spilt_height, src1.rows), Range(j*spilt_width, (j + 1)*spilt_width));
			}
			else if (i != y_num - 1 && j == x_num - 1)
			{
				src12 = src1(Range(i*spilt_height, (i + 1)*spilt_height), Range(j*spilt_width, src1.cols));

			}
			else if (i != y_num - 1 && j != x_num - 1)
			{
				src12 = src1(Range(i*spilt_height, (i + 1)*spilt_height), Range(j*spilt_width, (j + 1)*spilt_width));
			}
			threshold(src12, src22, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);
			Mat mask = src12.clone();
			src22.copyTo(src12, mask);
		}
		
	}
	//imwrite("erzhihuaquan.jpg", src1);
}

void cutcol(Mat &src)
{
	vector<int>Xshadow(src.cols, 0);
	for (int i = 0; i < src.cols; ++i)
	{
		for (int j = 0; j < src.rows; ++j)
		{
			if (src.at<uchar>(j,i) != 0)
				Xshadow[i]++;//每一列白色点数
		}
	}
	int left = 0;
	int right = Xshadow.size()-1;
	for (int i = 0; i<Xshadow.size()/2; i++)
	{
		if (Xshadow[i + 1] > 0 && Xshadow[i + 2] > 0 && Xshadow[i] == 0)
		{
			left = i;
		}
	}
	for (int i = Xshadow.size() - 1; i>Xshadow.size() / 2; i--)
	{
		if (Xshadow[i - 1] > 0 && Xshadow[i - 2] > 0 && Xshadow[i] == 0)
		{
			right = i;
		}
	}
	src=src.colRange(left,right);
}
int  ChipRecog::chipRecogSegmentOne(Mat &input, Rect &specialRect, string &s, network<sequential> &nn, int number)
{
	float b = float(input.rows) / float(input.cols);
	if (b>0.66&&input.cols<900)//小芯片粘连
	{
		int top_sp = specialRect.y - specialRect.height / 7 > 0 ? specialRect.y - specialRect.height / 7 : 0;
		Rect tempR = Rect(specialRect.x, top_sp,specialRect.width, specialRect.y + specialRect.height - 1 + specialRect.height / 3 < input.rows ? 4 * specialRect.height / 3 : input.rows - top_sp);
		Mat tempM = input.rowRange(top_sp, top_sp + tempR.height);//根据第一个数字找到所有数字所在的行
		threshold(tempM, tempM, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);
		Mat showshow = tempM.clone();
		Mat ss = showshow.clone();
		/*Mat element = getStructuringElement(MORPH_RECT, Size(9, 9));
		erode(tempM, tempM, element);*/
		
		//imwrite("03数字所在行.jpg", tempM);
		int top_new = specialRect.y - top_sp;
		int bottom = top_new + specialRect.height/* + specialRect.height / 3>input.rows ? input.rows : top_new + specialRect.height + specialRect.height / 3*/;

		Mat object = tempM.rowRange(top_new, bottom);//缩小高度
		//imwrite("04数字所在行精准化.jpg", object);
		Mat object1 = object.colRange(specialRect.x + specialRect.width, object.cols*7/8);//去除第一个数字，截出后四位数字的位置
		//imwrite("05后四位数字.jpg", object1);
		Mat image = object1.clone();
		

		Rect charRect = specialRect;
		int modelheigth = specialRect.height;
		int modelWidth = specialRect.width;
		vector<Rect>charRects;
		Mat im2;
		int adhesion = 0;//用于判断前面数字是否有粘连，粘连的话，起始位置没有从黑到白，既p.x=0
		while (charRects.size() < number - 2 && !equalImage(image, im2))//剩下部分是否相等（没有数字只剩黑色像素则停止循环）
			//while：找出7或6之后的所有数字的边框
		{
			//imwrite("06剩下部分.jpg", image);
			//if (getNextRect(image, charRect, showshow, modelWidth))//showshow数字所在行
			//找到之前数字的下一个数字（除去前两个数字，第一次找7或6之后的一位数字），
			//将找到的数的框存入charRect，将截掉找到的数字后的图片赋给image，使用image（剩下部分）继续找下一个数

			if (getNextRect2(image, charRect, ss, specialRect, adhesion))
			{
				charRects.push_back(charRect);//将找到的数字放入charRects
				/*rectangle(as, charRect, Scalar(255), 2);*/

			}
			//image = image.colRange(charRect.x + charRect.width, image.cols);
			if (charRect.x + charRect.width > ss.cols || image.cols <= specialRect.width)
				break;
		}
		if (charRects.size() != number - 2)//未找到指定数量的数字，7的位置未找准确
		{
			return 1;
		}
		for (int i = 0; i < charRects.size(); ++i)//处理并识别后面的所有数字
		{
			//Mat candidate = showshow(charRects[i]);
			Mat candidate = ss.colRange(charRects[i].x, charRects[i].x + charRects[i].width);
			//imwrite("截出的数字" + intTString(i) + ".jpg", candidate);

			vector<int>Yshadow(candidate.rows, 0);
			uchar* p;
			for (int i = 0; i < candidate.rows; ++i)
			{
				p = candidate.ptr<uchar>(i);
				for (int j = 0; j < candidate.cols; j += 1)
				{
					int H = int(p[j]);
					if (H>0)
					{
						Yshadow[i]++;
					}

				}
			}
			int top;
			int bottom;
			for (int i = 0; i <= Yshadow.size() / 2; ++i)
			{
				if (i == int(Yshadow.size() / 2 - 1))
				{
					top = 0;
					break;
				}

				else if (Yshadow[i + 1] == 0 && Yshadow[i] == 0 && Yshadow[i + 2] == 0 && Yshadow[i + 3] > 0 && Yshadow[i + 4] >0 && Yshadow[i + 5] > 0)
				{
					top = i + 3;
					break;
				}
			}


			for (int j = Yshadow.size(); j >= top; --j)
			{

				if (j == top + Yshadow.size() / 2)
				{
					bottom = Yshadow.size();
					break;
				}
				else if (Yshadow[j - 1] == 0 && Yshadow[j - 2] == 0 && Yshadow[j - 3] == 0 && Yshadow[j - 4] > 0 && Yshadow[j - 5] > 0 && Yshadow[j - 6] > 0)
				{
					bottom = j - 3;
					break;
				}
			}
			if (top == 0)
				top = bottom - specialRect.height>0 ? bottom - specialRect.height : 0;

			Rect temp1 = Rect(0, top, charRects[i].width, bottom - top);//截出只有数字的框
			candidate = candidate(temp1);
			//imwrite("截出的数字" + intTString(i) + "1.jpg", candidate);
			cutcol(candidate);
			//imwrite("截出的数字" + intTString(i) + "2.jpg", candidate);
			Mat show_show = showshow.clone();
			preprocessChar2(candidate);
			Mat new_candidate = candidate.clone();
			vec_t dd;
			convertImage(new_candidate, dd);
			int index1 = 0;
			recogNumber(nn, dd, index1);//识别数字
			string temp = intTString(index1);
			s += temp;
		}
		/*namedWindow("showshow", WINDOW_NORMAL);
		imshow("showshow", show_show);*/
		return 0;
	}
	else
	{
		int top_sp = specialRect.y - specialRect.height / 7 > 0 ? specialRect.y - specialRect.height / 7 : 0;

		Rect tempR = Rect(specialRect.x, top_sp,
			specialRect.width, specialRect.y + specialRect.height - 1 + specialRect.height / 3 < input.rows ? 4 * specialRect.height / 3 : input.rows - top_sp);
		Mat tempM = input.rowRange(top_sp, top_sp + tempR.height);//根据第一个数字找到所有数字所在的行
		//threshold(tempM, tempM, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);
		erzhihua(tempM);
		Mat showshow = tempM.clone();
		Mat ss = showshow.clone();
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(tempM, tempM, element);
		//imwrite("03数字所在行.jpg", tempM);
		
		int top_new = specialRect.y - top_sp;

		int bottom = top_new + specialRect.height/* + specialRect.height / 3>input.rows ? input.rows : top_new + specialRect.height + specialRect.height / 3*/;

		Mat object = tempM.rowRange(top_new, bottom);//缩小高度
		//imwrite("04数字所在行精准化.jpg", object);

		Mat object1 = object.colRange(specialRect.x + specialRect.width, object.cols);//去除第一个数字，截出后四位数字的位置
		//imwrite("05后四位数字.jpg", object1);
		Mat image = object1.clone();
		

		Rect charRect = specialRect;

		int modelheigth = specialRect.height;
		int modelWidth = specialRect.width;
		vector<Rect>charRects;

		Mat im2;
		while (charRects.size() < number - 2 && !equalImage(image, im2))//剩下部分是否相等（没有数字只剩黑色像素则停止循环）
			//while：找出7或6之后的所有数字的边框
		{
			//imwrite("06剩下部分.jpg", image);
			//if (getNextRect(image, charRect, showshow, modelWidth))//showshow数字所在行
			//找到之前数字的下一个数字（除去前两个数字，第一次找7或6之后的一位数字），
			//将找到的数的框存入charRect，将截掉找到的数字后的图片赋给image，使用image（剩下部分）继续找下一个数
			if (getNextRect1(image, charRect, ss, specialRect))
			{
				charRects.push_back(charRect);//将找到的数字放入charRects
				/*rectangle(as, charRect, Scalar(255), 2);*/

			}
			//image = image.colRange(charRect.x + charRect.width, image.cols);
			if (charRect.x + charRect.width > ss.cols||image.cols<=specialRect.width)
				break;
		}
		if (charRects.size() != number - 2)//未找到指定数量的数字，7的位置未找准确
		{
			return 1;
		}
		for (int i = 0; i < charRects.size(); ++i)//处理并识别后面的所有数字
		{
			//Mat candidate = showshow(charRects[i]);
			Mat candidate = ss.colRange(charRects[i].x, charRects[i].x + charRects[i].width);
			//imwrite("截出的数字" + intTString(i) + ".jpg", candidate);

			vector<int>Yshadow(candidate.rows, 0);
			uchar* p;
			for (int i = 0; i < candidate.rows; ++i)
			{
				p = candidate.ptr<uchar>(i);
				for (int j = 0; j < candidate.cols; j += 1)
				{
					int H = int(p[j]);
					if (H>0)
					{
						Yshadow[i]++;
					}
				}
			}
			int top;
			int bottom;
			for (int i = 0; i <= Yshadow.size() / 2; ++i)
			{
				if (i == int(Yshadow.size() / 2 - 1))
				{
					top = 0;
					break;
				}

				else if (Yshadow[i + 1] == 0 && Yshadow[i] == 0 && Yshadow[i + 2] == 0 && Yshadow[i + 3] > 0 && Yshadow[i + 4] >0 && Yshadow[i + 5] > 0)
				{
					top = i + 3;
					break;
				}
			}


			for (int j = Yshadow.size(); j >= top; --j)
			{

				if (j == top + Yshadow.size() / 2)
				{
					bottom = Yshadow.size();
					break;
				}
				else if (Yshadow[j - 1] == 0 && Yshadow[j - 2] == 0 && Yshadow[j - 3] == 0 && Yshadow[j - 4] > 0 && Yshadow[j - 5] > 0 && Yshadow[j - 6] > 0)
				{
					bottom = j - 3;
					break;
				}
			}
			if (top == 0)
				top = bottom - specialRect.height>0 ? bottom - specialRect.height : 0;

			Rect temp1 = Rect(0, top, charRects[i].width, bottom - top);//截出只有数字的框
			candidate = candidate(temp1);
			//imwrite("截出的数字" + intTString(i) + "1.jpg", candidate);
			Mat show_show = showshow.clone();
			preprocessChar2(candidate);
			Mat new_candidate = candidate.clone();
			vec_t dd;
			convertImage(new_candidate, dd);
			int index1 = 0;
			recogNumber(nn, dd, index1);//识别数字
			string temp = intTString(index1);
			s += temp;
		}
		/*namedWindow("showshow", WINDOW_NORMAL);
		imshow("showshow", show_show);*/
		return 0;
	}
	
}


bool getNewRect(Rect& rect, const Mat &image)
{
	if (image.empty())
		return false;
	if (rect.area() == 0)
		return false;
	Mat input = image.clone();
	uchar *data = image.data;
	vector<int>Yshadow(rect.height/8,rect.x+rect.width);
	for (int i = rect.y; i < rect.y+rect.height/8; ++i)
	{

		for (int j = rect.x; j < rect.x+rect.width; ++j)
		{
			if (data[i*input.cols + j] != 0)
			{
				Yshadow[i - rect.y]=j;
				break;
			}
		}
	}
	int maxIndex = 0, maxValue =0;
	for (int i = 0; i < Yshadow.size(); ++i)
	{
		if (Yshadow[i] >= maxValue)
		{
			maxIndex = i;
			maxValue = Yshadow[i];
		}
	}
	if (maxIndex)
	{
		rect.height = rect.height - maxIndex;
		rect.y = rect.y+maxIndex;
	}
	return true;
}


int getInterestingRect(const Mat &in, Rect &finalrect)
{
	if (in.empty())
	{
		return 1;
	}
	if (finalrect.area() == 0)
	{
		return 2;
	}
	Mat image_ = in(finalrect).clone();//粗略框的7
	if (image_.channels() != 1)
	{
		cvtColor(image_, image_, CV_RGB2GRAY);
	}
	Mat  image = image_.clone();//初略图；

	Rect  rect2 = Rect(finalrect.x - finalrect.width / 6, finalrect.y - finalrect.height / 6, finalrect.width + finalrect.width / 3, finalrect.height + finalrect.height / 3);
	Mat   imageaddright = in(rect2);

	threshold(image, image, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);

	threshold(imageaddright, imageaddright, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);



	vector<int>Yshadow(imageaddright.rows, 0);
	uchar* p;
	for (int i = 0; i < imageaddright.rows; ++i)
	{
		p = imageaddright.ptr<uchar>(i);
		for (int j = 0; j < imageaddright.cols; j += 1)
		{
			int H = int(p[j]);
			if (H>0)
			{
				Yshadow[i]++;
			}

		}
	}
	int top;
	int bottom;
	for (int i = 0; i <= Yshadow.size() / 2; ++i)
	{
		if (i == int(Yshadow.size() / 2-1))
		{
			top = 0;
			break;
		}

		else if (Yshadow[i + 1] <6 && Yshadow[i] <6 && Yshadow[i + 2]<6 && Yshadow[i + 3] > 0 && Yshadow[i + 4] > 0 && Yshadow[i + 5] > 0)
		{
			top = i + 3;
			break;
		}
	}

	for (int j = top; j <= Yshadow.size(); ++j)
	{

		if (j == Yshadow.size() - 7)
		{
			bottom = Yshadow.size();
			break;
		}
		else if (Yshadow[j + 1] > 0 && Yshadow[j + 2] > 0 && Yshadow[j + 3] > 0 && Yshadow[j + 4] == 0 && Yshadow[j + 5] == 0 && Yshadow[j + 6] == 0)
		{
			bottom = j + 3;
			break;
		}
	}

	imageaddright = imageaddright.rowRange(top, bottom);
	Rect temp1 = Rect(rect2.x, rect2.y + top, rect2.width, bottom - top + 1);
	Mat   imageaddright12 = in(temp1);

	vector<int>Xshadow(imageaddright.cols, 0);
	for (int i = 0; i < imageaddright.cols; ++i)
	{

		for (int j = 0; j < imageaddright.rows; ++j)
		{
			int a = imageaddright.at<uchar>(j, i);
			if (a == 255)
				Xshadow[i]++;//每一列白色点数
		}
	}
	Point pt;

	for (int i = 0; i <= Xshadow.size() / 2; ++i)
	{
		if (i == int(Xshadow.size() / 2-1))
		{
			pt.x = 0;
			break;
		}

		else if (Xshadow[i + 1] == 0 && Xshadow[i] == 0 && Xshadow[i + 2] > 0 && Xshadow[i + 3] > 0 && Xshadow[i + 5] > 0 && Xshadow[i + 4] > 0 && Xshadow[i + 5] > 0)
		{
			pt.x = i + 1;
			break;
		}
	}

	for (int j = pt.x; j <= Xshadow.size() - 5; ++j)
	{

		if (j == Xshadow.size() - 6)
		{
			pt.y = Xshadow.size();
			break;
		}
		else if (Xshadow[j + 1] > 0 && Xshadow[j + 2] > 0 && Xshadow[j + 3] == 0 && Xshadow[j + 4] == 0)
		{
			pt.y = j + 2;
			break;
		}
	}
	imageaddright = imageaddright.colRange(pt.x, pt.y);
	Rect temp = Rect(temp1.x + pt.x, temp1.y, pt.y - pt.x + 1, temp1.height);
	Mat   imageaddright1 = in(temp);
	finalrect = temp;
	return 0;
}


int ChipRecog::FindSpecialNumberSeven(Mat &image_, Rect &specialRect, string path,int find7way)
{

	int hight = image_.rows;
	int weigh = image_.cols;
	bool haveresize=false;
	/*if (hight >= 1000 && weigh >= 1500)
	{
		resize(image_, image_, Size(2 * weigh / 3, 2 * hight / 3));
		haveresize = true;
	}*/
	Mat image = image_.clone();

	if (image.channels() == 3)
		cvtColor(image, image, CV_BGR2GRAY);

	if (image.cols < 50 && image.rows < 50)
		return 1;
	Ptr<ml::SVM>svm = ml::SVM::create();
	svm = ml::SVM::load<ml::SVM>(path+"/scu4.xml");
	
	/*Mat objectArea = image.rowRange(image.rows / 3, image.rows);
	objectArea = objectArea.colRange(0, image.cols * 2 / 3).clone();*/

	Mat pic = image.clone();
	cvtColor(pic, pic, CV_GRAY2BGR);
	Mat pic_ = pic.clone();

	//Mat image_threshold;


	//threshold(image, image_threshold, 0, 255, THRESH_OTSU + THRESH_BINARY_INV);
	
	Rect tempRect; 
	Mat obj_copy = image.clone();
	//imwrite("01芯片.jpg", obj_copy);

	int aaa=getSevenRect(obj_copy, tempRect, svm,haveresize,find7way);//粗略的给出7的框
	if (aaa!=0)
	return 1;
	
	if (tempRect.area() == 0)
		return 1;
	specialRect = tempRect;
	cv::rectangle(obj_copy, specialRect, Scalar(0, 255, 0), 2);
	//imwrite("01精细的框.jpg", obj_copy);
	return 0;
}

bool ChipRecog::getNextRect1(Mat &image_, Rect &rect, Mat &compareImage_, Rect specialRect)//大芯片
//image后面还未截出数字的图片 在循环里每次跟新；rect：标准的数字矩形框；compareImage_:所有数字所在行图片缩小；width：标准矩形框宽度
{
	Mat image = image_.clone();
	Mat as = compareImage_.clone();
	//imwrite("shuru.jpg", image);
	//imwrite("jiequ.jpg", as);
	vector<int>Xshadow(image.cols, 0);
	uchar *data = image.data;
	for (int i = 0; i < image.cols; ++i)
	{

		for (int j = 0; j < image.rows; ++j)
		{
			if (data[j*image.cols + i] != 0)
				Xshadow[i]++;//每一列白色点数
		}
	}

	Point pt=(0,0);
	int max = specialRect.width  <= Xshadow.size()? specialRect.width: Xshadow.size();
	for (int i = 0; i <= max; ++i)
	{
		 if (i == max-1)
		{
			 pt.x = 0;
			 for (int i = 0; i <= specialRect.width*4/5; ++i)
			{
				if (Xshadow[i]!=0)
				{
					pt.x = i;
					break;
				}
			}
			int max = pt.x+specialRect.width * 5 / 4 <= Xshadow.size()-5 ?pt.x+ specialRect.width * 5 / 4 : Xshadow.size()-5;
			for (int j = 0; j <= max; ++j)
			{			
				if (Xshadow[j + 1] > 0 && Xshadow[j + 2] > 0 && Xshadow[j + 3] == 0 && Xshadow[j + 4] == 0)
				{
					pt.y = j + 2;					
					break;
				}
				continue;
			}
			if (pt.y ==0)
			{
				vector<int>sum;
				int k = specialRect.width / 4;
				int miny = Xshadow[pt.x + specialRect.width - k>Xshadow.size() - 1 ? Xshadow.size() - 1 : pt.x + specialRect.width - k];
				int max = pt.x + specialRect.width + k <= image.cols ? pt.x + specialRect.width + k : image.cols;
				for (int i = pt.x + specialRect.width - k; i <= max-1; ++i)
				{
					if (Xshadow[i] < miny)
					{
						miny = Xshadow[i];
					}
				}
				for (int i = pt.x + specialRect.width - k; i <= max-1; ++i)
				{
					if (Xshadow[i] == miny)
					{
						sum.push_back(i);
					}
				}
				int sumy = 0;
				for (int i = 0; i < sum.size(); i++)
				{
					sumy = sumy + sum[i];
				}
				pt.y = sumy / sum.size();
				break;
			}
			break;
		}
		
		else if (Xshadow[i + 1] == 0 && Xshadow[i] == 0 && Xshadow[i + 2]>0 && Xshadow[i + 3]>0 && Xshadow[i + 5] > 0 && Xshadow[i + 4] > 0 )
		{		
			
			pt.x = i + 1;//start
			int max = pt.x + specialRect.width * 5 / 4 <= Xshadow.size() - 5 ? pt.x+specialRect.width * 5 / 4 : Xshadow.size() - 5;
			for (int j = i+5; j <= max; ++j)
			{			
				if (Xshadow[j + 1] > 0 && Xshadow[j + 2] > 0 && Xshadow[j + 3] == 0 && Xshadow[j + 4] == 0)
				{
					pt.y = j + 2;				
					break;
				}
				continue;
			}
			if (pt.y ==0)
			{
				vector<int>sum;
				int k = specialRect.width / 4;
				int miny = Xshadow[pt.x + specialRect.width - k>Xshadow.size() - 1 ? Xshadow.size() - 1 : pt.x + specialRect.width - k];
				int max = pt.x + specialRect.width + k <= image.cols ? pt.x + specialRect.width + k : image.cols;
				for (int i = pt.x + specialRect.width - k; i <= max-1; ++i)
				{
					if (Xshadow[i] < miny)
					{
						miny = Xshadow[i];
					}
				}
				for (int i = pt.x + specialRect.width - k; i <= max-1; ++i)
				{
					if (Xshadow[i] == miny)
					{
						sum.push_back(i);
					}
				}
				int sumy = 0;
				for (int i = 0; i < sum.size(); i++)
				{
					sumy = sumy + sum[i];
				}
				pt.y = sumy / sum.size();
				break;
			}
			break;
		}
		
		continue;
	}

	

	Rect temp = Rect(pt.x + rect.x + rect.width, 0, pt.y - pt.x + 1, as.rows);//截出只有数字的框
	Mat candidate = as(temp);
	//imwrite("1111.jpg", candidate);

	rect = temp;
	if (!judgeSingleChars(temp, as, specialRect.width))//是否为单个数字
	{
		image_ = image_.colRange(pt.x + rect.width, image_.cols);

		return false;
	}

	/*if (pt.y = pt.x + width)
	{
		image = as.colRange(rect.x + rect.width, as.cols);
		imwrite("image.jpg", image);
	}*/
		
	image_ = image_.colRange(pt.x + rect.width, image_.cols);

	return true;
}

bool ChipRecog::getNextRect2(Mat &image_, Rect &rect, Mat &compareImage_, Rect specialRect,int &adhesion)//小芯片
//image后面还未截出数字的图片 在循环里每次跟新；rect：标准的数字矩形框；compareImage_:所有数字所在行图片缩小；width：标准矩形框宽度
{
	Mat image = image_.clone();
	Mat as = compareImage_.clone();
	//imwrite("shuru.jpg", image);
	//imwrite("jiequ.jpg", as);
	vector<int>Xshadow(image.cols, 0);
	uchar *data = image.data;
	for (int i = 0; i < image.cols; ++i)
	{

		for (int j = 0; j < image.rows; ++j)
		{
			if (data[j*image.cols + i] != 0)
				Xshadow[i]++;//每一列白色点数
		}
	}

	Point pt;
	for (int i = 0; i < Xshadow.size(); ++i)
	{
		if (i >= specialRect.width * 4 / 5 || adhesion==1)
		{
			pt.x = 0;
			for (int i = 0; i <= specialRect.width * 4 / 5; ++i)
			{
				if (Xshadow[i] != 0)
				{
					pt.x = i;
					break;
				}
			}
			for (int j = 0; j <= Xshadow.size() - 5; ++j)
			{
				if (Xshadow[j + 1] > 0 && Xshadow[j + 2] > 0 && Xshadow[j + 3] == 0 && Xshadow[j + 4] == 0 || j ==Xshadow.size()-5)
				{
					pt.y = j + 2;//end
					if (pt.y >= pt.x + specialRect.width * 5 / 4)
					{
						vector<int>sum;
						adhesion = 1;
						int k = specialRect.width / 4;
						int miny = Xshadow[specialRect.width - k];
						int max = specialRect.width + k <= image.cols ? specialRect.width + k : image.cols;
						for (int i = specialRect.width - k; i <= max; ++i)
						{
							if (Xshadow[i] < miny)
							{
								miny = Xshadow[i];
							}
						}
						for (int i = specialRect.width - k; i <= max; ++i)
						{
							if (Xshadow[i] == miny)
							{
								sum.push_back(i);
							}
						}
						int sumy = 0;
						for (int i = 0; i < sum.size(); i++)
						{
							sumy = sumy + sum[i];
						}
						pt.y = sumy / sum.size();
						break;
					}
					break;
				}
				continue;
			}
			break;
		}

		else if (Xshadow[i + 1] == 0 && Xshadow[i] == 0 && Xshadow[i + 2]>0 && Xshadow[i + 3]>0 && Xshadow[i + 5] > 0 && Xshadow[i + 4] > 0 && Xshadow[i + 5] > 0)
		{

			pt.x = i + 1;//start
			for (int j = i + 5; j <= Xshadow.size() - 5; ++j)
			{

				if (Xshadow[j + 1] > 0 && Xshadow[j + 2] > 0 && Xshadow[j + 3] == 0 && Xshadow[j + 4] == 0||j==Xshadow.size()-5)
				{
					pt.y = j + 2;//end
					if (pt.y >= pt.x + specialRect.width * 5 / 4)					
					{
						adhesion = 1;
						vector<int>sum;
						int k = specialRect.width / 4;
						int miny = Xshadow[pt.x+specialRect.width - k];
						int max =pt.x+ specialRect.width + k <= image.cols ? pt.x+specialRect.width + k : image.cols;
						for (int i = pt.x+specialRect.width - k; i <=max; ++i)
						{
							if (Xshadow[i] < miny)
							{
								miny = Xshadow[i];
							}
						}
						for (int i = pt.x + specialRect.width - k; i <= max; ++i)
						{
							if (Xshadow[i] == miny)
							{
								sum.push_back(i);
							}
						}
						int sumy=0;
						for (int i = 0; i < sum.size(); i++)
						{
							sumy = sumy + sum[i];
						}
						pt.y = sumy / sum.size();
						break;

					}
					break;
				}
				continue;
			}
			break;
		}

		continue;
	}



	Rect temp = Rect(pt.x + rect.x + rect.width, 0, pt.y - pt.x + 1, as.rows);//截出只有数字的框
	Mat candidate = as(temp);
	//imwrite("1111.jpg", candidate);

	rect = temp;
	if (!judgeSingleChars(temp, as, specialRect.width))//是否为单个数字
	{
		image_ = image_.colRange(pt.x + rect.width, image_.cols);

		return false;
	}

	/*if (pt.y = pt.x + width)
	{
	image = as.colRange(rect.x + rect.width, as.cols);
	imwrite("image.jpg", image);
	}*/

	image_ = image_.colRange(pt.x + rect.width, image_.cols);

	return true;
}