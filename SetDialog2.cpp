// SetDialog2.cpp : 实现文件
//

#include "stdafx.h"
#include "chipUI.h"
#include "SetDialog2.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include "CvvImage.h"
using namespace cv;

// CSetDialog2 对话框

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


// CSetDialog2 消息处理程序


void CSetDialog2::OnEnChangeEditSetrange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	GetDlgItem(IDC_EDIT_SetRange)->GetWindowTextW(str);
	// TODO:  在此添加控件通知处理程序代码
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
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	DrawPicture();
}
