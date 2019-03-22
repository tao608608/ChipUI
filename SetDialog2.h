#pragma once
#include "resource.h"
#include "opencv2/opencv.hpp"

using namespace cv;
// CSetDialog2 �Ի���

class CSetDialog2 : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDialog2)

public:
	CSetDialog2(Mat &src,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetDialog2();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditSetrange();

	CString str;
	int DrawPicture();
	Mat img;
	afx_msg void OnPaint();
};
