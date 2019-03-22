#pragma once

#include "opencv2/opencv.hpp"
// CSetDialog �Ի���
using namespace cv;
class CSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDialog)

public:
	CSetDialog(Mat &image,int start_,int end_,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
