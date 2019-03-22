// SetDialog2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chipUI.h"
#include "SetDialog2.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include "CvvImage.h"
using namespace cv;

// CSetDialog2 �Ի���

IMPLEMENT_DYNAMIC(CSetDialog2, CDialogEx)

CSetDialog2::CSetDialog2(Mat &src,CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetDialog2::IDD, pParent)
{
	img = src.clone();
	str = "16";
}

CSetDialog2::~CSetDialog2()
{
}

void CSetDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetDialog2, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_SetRange, &CSetDialog2::OnEnChangeEditSetrange)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSetDialog2 ��Ϣ�������


void CSetDialog2::OnEnChangeEditSetrange()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	GetDlgItem(IDC_EDIT_SetRange)->GetWindowTextW(str);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
int CSetDialog2::DrawPicture()
{
	CRect rect;
	IplImage src_ = img;
	CvvImage cv_src;
	cv_src.CopyOf(&src_);
	CDC* pDC = GetDlgItem(IDC_rangeImage)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	GetDlgItem(IDC_rangeImage)->GetClientRect(&rect);
	cv_src.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);

	UpdateData(false);
	return 0;
}

void CSetDialog2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	DrawPicture();
}
