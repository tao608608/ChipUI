#pragma once
#include "resource.h"
#include "opencv2/opencv.hpp"

using namespace cv;
// CSetDialog2 对话框

class CSetDialog2 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDialog2)

public:
	CSetDialog2(Mat &src,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDialog2();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditSetrange();

	CString str;
	int DrawPicture();
	Mat img;
	afx_msg void OnPaint();
};
