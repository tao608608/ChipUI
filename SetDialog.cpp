// SetDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "chipUI.h"
#include "SetDialog.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "resource.h"
// CSetDialog 对话框

IMPLEMENT_DYNAMIC(CSetDialog, CDialogEx)

CSetDialog::CSetDialog(Mat &image_,int start_,int end_,CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetDialog::IDD, pParent)
{
	image = image_.clone();
	m_edit = "";
	start = start_;
	end = end_;
}

CSetDialog::~CSetDialog()
{
}

void CSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_Set, &CSetDialog::OnEnChangeEditSet)
//	ON_WM_PAINT()
//ON_WM_PAINT()
ON_WM_PAINT()

ON_BN_CLICKED(IDOK, &CSetDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetDialog 消息处理程序


void CSetDialog::OnEnChangeEditSet()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	 GetDlgItem(IDC_EDIT_Set)->GetWindowTextW(m_edit);
}

int CSetDialog::DrawPicture()
{
	CRect rect;
	IplImage src_ = image;
	CvvImage cv_src;
	cv_src.CopyOf(&src_);
	CDC* pDC = GetDlgItem(IDC_ChipImage)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	GetDlgItem(IDC_ChipImage)->GetClientRect(&rect);
	cv_src.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
	
	UpdateData(false);
	return 0;
}








//void CSetDialog::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO:  在此处添加消息处理程序代码
//	// 不为绘图消息调用 CDialogEx::OnPaint()
//	DrawPicture();
//}


void CSetDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	DrawPicture();
}





BOOL CSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CWnd* pWnd = GetDlgItem(IDC_EDIT_Set);

	pWnd->SetFocus();
	return false;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CSetDialog::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	int temp = _ttoi(m_edit);

	if (temp > end || temp < start)
	{
		CString strs;
		strs = "请输入";
		CString a;
		
		a.Format(_T("%d"), start);
		strs += a;
		strs += "-";
		CString b;
		b.Format(_T("%d"), end);
		strs += b;
		strs += "的编号!";
		MessageBox(strs);
		return;
	}

	CDialogEx::OnOK();
}
