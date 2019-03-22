// SetDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chipUI.h"
#include "SetDialog.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "resource.h"
// CSetDialog �Ի���

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


// CSetDialog ��Ϣ�������


void CSetDialog::OnEnChangeEditSet()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
//	// TODO:  �ڴ˴������Ϣ����������
//	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
//	DrawPicture();
//}


void CSetDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	DrawPicture();
}





BOOL CSetDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CWnd* pWnd = GetDlgItem(IDC_EDIT_Set);

	pWnd->SetFocus();
	return false;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CSetDialog::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int temp = _ttoi(m_edit);

	if (temp > end || temp < start)
	{
		CString strs;
		strs = "������";
		CString a;
		
		a.Format(_T("%d"), start);
		strs += a;
		strs += "-";
		CString b;
		b.Format(_T("%d"), end);
		strs += b;
		strs += "�ı��!";
		MessageBox(strs);
		return;
	}

	CDialogEx::OnOK();
}
