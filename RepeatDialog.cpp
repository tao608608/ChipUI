// RepeatDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chipUI.h"
#include "RepeatDialog.h"
#include "afxdialogex.h"




// CRepeatDialog �Ի���

IMPLEMENT_DYNAMIC(CRepeatDialog, CDialogEx)

CRepeatDialog::CRepeatDialog(Mat &im1,Mat &im2,int start_,int end_,int index_,CString path_,CString str,
const vector<CString>&Allstring_,CString ipathpic,CWnd* pParent /*=NULL*/)
	: CDialogEx(CRepeatDialog::IDD, pParent)
{
	image1 = im1.clone();
	image2 = im2.clone();
	start = start_;
	end = end_;
	index = index_;
	path = path_;
	edit_str1 = str;
	Allstring = Allstring_;
	ipathpicture = ipathpic;
}

CRepeatDialog::~CRepeatDialog()
{
}

void CRepeatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRepeatDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRepeatDialog::OnBnClickedOk)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_re1, &CRepeatDialog::OnEnChangeEditre1)
	ON_EN_CHANGE(IDC_EDIT_re2, &CRepeatDialog::OnEnChangeEditre2)
END_MESSAGE_MAP()


// CRepeatDialog ��Ϣ�������

int CRepeatDialog::DrawPicture1()
{
	CRect rect;
	IplImage src_ = image1;
	CvvImage cv_src;
	cv_src.CopyOf(&src_);
	CDC* pDC = GetDlgItem(IDC_Picpic1)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	GetDlgItem(IDC_Picpic1)->GetClientRect(&rect);
	cv_src.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);

	UpdateData(false);
	return 0;
}
int CRepeatDialog::DrawPicture2()
{
	CRect rect;
	IplImage src_ = image2;
	CvvImage cv_src;
	cv_src.CopyOf(&src_);
	CDC* pDC = GetDlgItem(IDC_PicPic2)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	GetDlgItem(IDC_PicPic2)->GetClientRect(&rect);
	cv_src.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);

	UpdateData(false);
	return 0;
}


//bool CRepeatDialog::repeat(CString str, int &index)
//{
//	for (int i = 0; i < Allstring.size(); ++i)
//	{
//		if (str == Allstring[i])
//		{
//			index = i;
//			return true;
//		}
//	}
//	return false;
//}

void CRepeatDialog::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	int temp1 = _ttoi(edit_str1);

	if (temp1 > end || temp1 < start)
	{
		CString strs;
		strs = "��߿�������";
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


	//if (repeat(edit_str2,index))
	//{
	//	string intToString(int);
	//	string picPath = ipathpicture + "/" + intToString(index + 1) + ".jpg";
	//	Mat tempimg = imread(picPath);//�ظ���ͼƬ
	//	
	//	CRepeatDialog myrepeat2(tempimg, image2, start, end, index, path, Allstring[index], Allstring, ipathpicture);
	//
	//		if (myrepeat2.DoModal() == IDOK)
	//		{
	//			writeExcelPos(index, path, myrepeat2.edit_str1);
	//			tempedit_str2 = myrepeat2.edit_str2;
	//			UpdateData(false);
	//		}
	//}
	int temp2 = _ttoi(edit_str2);

	if (temp2 > end || temp2 < start)
	{
		CString strs;
		strs = "�ұ߿�������";
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


void CRepeatDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	DrawPicture1();
	DrawPicture2();
}


void CRepeatDialog::OnEnChangeEditre1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	GetDlgItem(IDC_EDIT_re1)->GetWindowText(edit_str1);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CRepeatDialog::OnEnChangeEditre2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	GetDlgItem(IDC_EDIT_re2)->GetWindowText(edit_str2);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


bool CRepeatDialog::readExcel(int &index,CString str)
{


	return true;
}

BOOL CRepeatDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//readExcel(index, path);
	GetDlgItem(IDC_EDIT_re1)->SetWindowText(edit_str1);

	CWnd* pWnd = GetDlgItem(IDC_EDIT_re2);

	pWnd->SetFocus();
	OnPaint();
	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
