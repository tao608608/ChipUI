#pragma once

#include "opencv2/opencv.hpp"
// CSetDialog 对话框
using namespace cv;
class CSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDialog)

public:
	CSetDialog(Mat &image,int start_,int end_,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_edit;
	afx_msg void OnEnChangeEditSet();
	int writeExcel(CString &);
	int DrawPicture();
	Mat image;
//	afx_msg void OnPaint();
//	afx_msg void OnPaint();
	afx_msg void OnPaint();
	afx_msg void OnStnClickedChipimage();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int start;
	int end;
};
