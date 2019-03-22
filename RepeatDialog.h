#pragma once

#include "resource.h"
#include "opencv2/opencv.hpp"
#include "CvvImage.h"
using namespace cv;
using namespace std;
// CRepeatDialog 对话框

class CRepeatDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CRepeatDialog)

public:
	CRepeatDialog(Mat &im1, Mat &im2, int start_, int end_, int index_, CString path_, CString str,const vector<CString>&Allstring_,
		CString ipathpic,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRepeatDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int DrawPicture1();
	int DrawPicture2();
	afx_msg void OnBnClickedOk();
	CString edit_str1;
	CString edit_str2;
	Mat image1;
	Mat image2;
	afx_msg void OnPaint();
	afx_msg void OnEnChangeEditre1();

	int start;
	int end;
	afx_msg void OnEnChangeEditre2();
	CString out;
	CString  ipathpicture;
	int index;
	CString path;
	vector<CString>Allstring;
public:
	bool readExcel(int &index,CString );
	virtual BOOL OnInitDialog();
	

};
