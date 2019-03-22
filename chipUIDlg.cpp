
// chipUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "chipUI.h"
#include "chipUIDlg.h"
#include "afxdialogex.h"
#include "chipRecog.h"
#include "ann.h"
#include "CvvImage.h"
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <conio.h>
#include  <direct.h>
#include "SetDialog2.h"
#include "Dbghelp.h"
#include "libxl.h"

using namespace libxl;


using namespace std;
using namespace cv;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	
// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


int stringToINT(string s)
{
	stringstream ss;
	ss << s;
	int a=0;
	ss >> a;
	return a;
}

// CchipUIDlg �Ի���



CchipUIDlg::CchipUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CchipUIDlg::IDD, pParent)
	, m_result_new(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	checkButton = true;
	errorCount = 0;
	hot_key_check = false;
}
string intToString(int a)
{
	stringstream ss;
	ss << a;
	string b;
	ss >> b;
	return b;
}
void CchipUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Result_Edit, m_result_new);
	DDV_MaxChars(pDX, m_result_new, 9);
	DDX_Control(pDX, IDC_Instance, m_button_instance);
	DDX_Control(pDX, IDC_GetImage, m_button_getImage);
	DDX_Control(pDX, IDC_Close, m_button_close);
	DDX_Control(pDX, IDC_Recog, m_button_recog);

	DDX_Control(pDX, IDC_COMBO6, m_cb6);

	DDX_Control(pDX, IDC_Result_Edit, m_editPlace);
	DDX_Control(pDX, IDC_BUTTON_Debug, m_debug);
	DDX_Control(pDX, IDC_BUTTON3, m_stopDebug);
	DDX_Control(pDX, IDC_OpenPicture, m_openPicture);
	DDX_Control(pDX, IDC_Save, m_SavePicture);
	DDX_Control(pDX, IDC_checkButton, m_check_Button);
}

BEGIN_MESSAGE_MAP(CchipUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
//	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_Instance, &CchipUIDlg::OnBnClickedInstance)
	
	ON_BN_CLICKED(IDC_GetImage, &CchipUIDlg::OnBnClickedGetimage)
	ON_BN_CLICKED(IDC_Recog, &CchipUIDlg::OnBnClickedRecog)
	ON_BN_CLICKED(IDC_OpenPicture, &CchipUIDlg::OnBnClickedOpenpicture)
	ON_BN_CLICKED(IDC_Close, &CchipUIDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_Save, &CchipUIDlg::OnBnClickedSave)
	ON_WM_TIMER()
	

	ON_CBN_SELCHANGE(IDC_COMBO6, &CchipUIDlg::OnCbnSelchangeCombo6)
	
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Debug, &CchipUIDlg::OnBnClickedButtonDebug)
	ON_BN_CLICKED(IDC_BUTTON3, &CchipUIDlg::OnBnClickedButton3)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_BUTTON_repair, &CchipUIDlg::OnBnClickedButtonrepair)
	ON_BN_CLICKED(IDC_Caculate, &CchipUIDlg::OnBnClickedCaculate)
	ON_BN_CLICKED(IDC_touch, &CchipUIDlg::OnBnClickedtouch)
	ON_BN_CLICKED(IDC_cancelTouch, &CchipUIDlg::OnBnClickedcanceltouch)
	ON_BN_CLICKED(IDC_checkButton, &CchipUIDlg::OnBnClickedcheckbutton)
	ON_BN_CLICKED(IDCANCEL, &CchipUIDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CchipUIDlg ��Ϣ�������
CchipUIDlg *dlg;
BOOL CchipUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	countRecog = 0;
	SetWindowText(_T("���ʶ��ϵͳ"));
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	
	/*OnBnClickedInstance();
	SetTimer(1, 200, NULL);*/
	GetDlgItem(IDC_touch)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_cancelTouch)->ShowWindow(SW_HIDE);
	m_check_Button.SetCheck(0);
	m_button_instance.EnableWindow(true);
	m_button_getImage.EnableWindow(false);
	m_button_close.EnableWindow(false);
	m_button_recog.EnableWindow(false);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	str1 = "��";
	
	m_cb6.SetCurSel(0);
	CTime ct;
	ct=CTime::GetTickCount();
	current_time = ct.Format("%m��%d��%Hʱ%M��%S��result.xls");
	timeForSave = ct.Format("%m��%d��%Hʱ%M��%S��-img");
    
	
	
	_getcwd(ppbuffer, MAX_PATH);
	string cwdBuf  = ppbuffer;

	cwdBuf +="/ini.xml";
	FileStorage  fs;
	fs.open(cwdBuf, FileStorage::READ);
	inumber = fs["iNumber"];
	itype = fs["iType"];
	string start_ = fs["iStart"];
	string end_ = fs["iEnd"];
	istart = stringToINT(start_);
	iend = stringToINT(end_);
	if (!start_.empty())
	{
		specialChar1.push_back(start_[0]);
		specialChar2.push_back(start_[1]);
	}
	pauseSeconds = fs["iTime"];
	iPathExcel_ = fs["iExcel"];
	iPathPicture_ = fs["iPicture"];
	pBuffer_path = fs["Buffer"];
	irotate = fs["Rotate"];
	lostPathExcel_ = fs["lostExcel"];
	m_cb6.SetCurSel(irotate);
	iPathExcel = iPathExcel_.c_str();
	iPathPicture = iPathPicture_.c_str();
	lostPathExcel = lostPathExcel_.c_str();

	firstReturn = false;


	m_editFont.CreatePointFont(500, _T("����"));
	m_editPlace.SetFont(&m_editFont);



#define O true
#define X false
	static const bool connection[] = {
		O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
		O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
		O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
		X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
		X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
		X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
	};
#undef O
#undef X

	nn << convolutional_layer<tan_h>(32, 32, 5, 1, 6) // 32x32 in, 5x5 kernel, 1-6 fmaps conv
		<< average_pooling_layer<tan_h>(28, 28, 6, 2) // 28x28 in, 6 fmaps, 2x2 subsampling
		<< convolutional_layer<tan_h>(14, 14, 5, 6, 16,
		connection_table(connection, 6, 16)) // with connection-table
		<< average_pooling_layer<tan_h>(10, 10, 16, 2)
		<< convolutional_layer<tan_h>(5, 5, 5, 16, 120)
		<< fully_connected_layer<tan_h>(120, 10);

	string ss = pBuffer_path + "\\scu3";
	
	
	
	
	ifs.open(ss);
	nn.load(ifs);


	if (specialChar2.empty())
		MessageBox(_T("��ʼ��xml����"));
	path_cas1 = pBuffer_path + "/scu_h_xml/scu_h_" + specialChar2 + ".xml";
	
	
	

	dlg = this;
	OnBnClickedInstance();
	
	CString csSubString;
	CString path = iPathPicture;
	while (-1 != path.Find('\\', 0))
	{
		int nPos = path.Find('\\', 0);
		csSubString += path.Left(nPos + 1);
		if (!PathIsDirectory(csSubString))
		{
			if (!CreateDirectory(csSubString, NULL))
			{
				MessageBox(_T("����·��ʧ��"));
			}
		}
		path = path.Right(path.GetLength() - nPos - 1);
	}

	csSubString += path;
	CreateDirectory(csSubString, NULL);
	/*savePathPicture += iPathPicture;*/
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CchipUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

//void CchipUIDlg::OnPaint()
//{
//	if (IsIconic())
//	{
//		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
//
//		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
//
//		// ʹͼ���ڹ����������о���
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;
//
//		// ����ͼ��
//		dc.DrawIcon(x, y, m_hIcon);
//	}
//	else
//	{
//		CDialogEx::OnPaint();
//	}
//}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CchipUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void DrawResult(string s)
{
	Mat image = Mat::zeros(200, 1024, CV_8UC3);
	putText(image, s.c_str(), Point(50, 200), 2, 8, Scalar(255, 255, 255));
	
	//imshow("ʶ����", image);	
}



void CchipUIDlg::OnBnClickedInstance()
{
	//COUNTING AVAILABLE SYSTEMS (TL producers) 

	
	GetLostNumber(lostPathExcel);
	readExcel();
	OnBnClickedClose();

	//string str_ann = pBuffer_path + "/charnew1.xml";
	
	string str_svm = pBuffer_path + "/scu2.xml";
	/*string str_haar = pBuffer_path + "/cascade_haar.xml";
*/  


	//at.ann = ml::ANN_MLP::load<ml::ANN_MLP>(str_ann);

	mySvm.svm = cv::ml::SVM::load<cv::ml::SVM>(str_svm);
	

	//cascade = makePtr<cv::CascadeClassifier>(str_haar);
	try
	{
		systemList = SystemList::GetInstance();
		systemList->Refresh();

		//SYSTEM DEVICE INFORMATION
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("ϵͳ��ʼ��ʧ��"));
		return;
	}

	//OPEN THE FIRST SYSTEM IN THE LIST WITH A CAMERA CONNECTED
	try
	{
		for (SystemList::iterator sysIterator = systemList->begin(); sysIterator != systemList->end(); sysIterator++)
		{
			try
			{
				sysIterator->second->Open();
				
				sSystemID = sysIterator->first;
				
				try
				{
					interfaceList = sysIterator->second->GetInterfaces();
					//COUNT AVAILABLE INTERFACES
					interfaceList->Refresh(100); // timeout of 100 msec					
					//INTERFACE INFORMATION					
				}
				catch (BGAPI2::Exceptions::IException& ex)
				{
					MessageBox(_T("δ��⵽����"));
					return;
				}


				

				//OPEN THE NEXT INTERFACE IN THE LIST
				try
				{
					for (InterfaceList::iterator ifIterator = interfaceList->begin(); ifIterator != interfaceList->end(); ifIterator++)
					{
						try
						{
							
							ifIterator->second->Open();
							//search for any camera is connetced to this interface
							deviceList = ifIterator->second->GetDevices();
							deviceList->Refresh(100);
							if (deviceList->size() == 0)
							{
								ifIterator->second->Close();
							}
							else
							{
								sInterfaceID = ifIterator->first;
								
								if (ifIterator->second->GetTLType() == "GEV")
								{
									bo_int64 iIpAddress = ifIterator->second->GetNode("GevInterfaceSubnetIPAddress")->GetInt();
									
									bo_int64 iSubnetMask = ifIterator->second->GetNode("GevInterfaceSubnetMask")->GetInt();
								
								}
								if (ifIterator->second->GetTLType() == "U3V")
								{
									
								}
								
								break;
							}
						}
						catch (BGAPI2::Exceptions::ResourceInUseException& ex)
						{
							MessageBox(_T("������Ϣ��ȡʧ��"));
						}
					}
				}
				catch (BGAPI2::Exceptions::IException& ex)
				{
					MessageBox(_T("�������ʧ��"));
					return;
				}


				//if a camera is connected to the system interface then leave the system loop
				if (sInterfaceID != "")
				{
					break;
				}
			}
			catch (BGAPI2::Exceptions::ResourceInUseException& ex)
			{
				MessageBox(_T("ϵͳ�Ѿ���"));
				return;
			}
		}
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("��������"));
		return;
	}

	if (sSystemID == "")
	{
		MessageBox(_T("δ�ҵ�ϵͳ"));
		BGAPI2::SystemList::ReleaseInstance();
		return ;
	}
	else
	{
		pSystem = (*systemList)[sSystemID];
	}


	if (sInterfaceID == "")
	{
		
		MessageBox(_T("δ�ҵ����"));
		pSystem->Close();
		BGAPI2::SystemList::ReleaseInstance();
		return ;
	}
	else
	{
		pInterface = (*interfaceList)[sInterfaceID];
	}


	try
	{
		//COUNTING AVAILABLE CAMERAS
		deviceList = pInterface->GetDevices();
		deviceList->Refresh(100);
		

		//DEVICE INFORMATION BEFORE OPENING
		
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("��ȡ���ʧ��"));
	}

	//OPEN THE FIRST CAMERA IN THE LIST
	try
	{
		for (DeviceList::iterator devIterator = deviceList->begin(); devIterator != deviceList->end(); devIterator++)
		{
			try
			{
				devIterator->second->Open();
				sDeviceID = devIterator->first;

				//SERIAL NUMBER
				
				break;
			}
			catch (BGAPI2::Exceptions::ResourceInUseException& ex)
			{
				MessageBox(_T("����Ѿ���"));
			}
			catch (BGAPI2::Exceptions::AccessDeniedException& ex)
			{
				MessageBox(_T("����Ѿ���"));
			}
		}
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("��������"));
	}

	if (sDeviceID == "")
	{
		MessageBox(_T("δ�ҵ���Ӧ�豸"));
		pInterface->Close();
		pSystem->Close();
		BGAPI2::SystemList::ReleaseInstance();
		return ;
	}
	else
	{
		pDevice = (*deviceList)[sDeviceID];
	}

	try
	{
		//SET TRIGGER MODE OFF (FreeRun)
		//pDevice->GetRemoteNode("TriggerMode")->SetString("Off");
		pDevice->GetRemoteNode("PixelFormat")->SetString("Mono8");
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("���ø�ʽ����"));
	}

	try
	{
		//COUNTING AVAILABLE DATASTREAMS
		datastreamList = pDevice->GetDataStreams();
		datastreamList->Refresh();
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("�õ�����������"));
	}

	//OPEN THE FIRST DATASTREAM IN THE LIST
	try
	{
		for (DataStreamList::iterator dstIterator = datastreamList->begin(); dstIterator != datastreamList->end(); dstIterator++)
		{
			sDataStreamID = dstIterator->first;
			
			dstIterator->second->Open();
			
		
			break;
		}
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("�򿪵�һ������������"));
	}

	if (sDataStreamID == "")
	{
		MessageBox(_T("��������ȡʧ��"));
		pDevice->Close();
		pInterface->Close();
		pSystem->Close();
		BGAPI2::SystemList::ReleaseInstance();
		return ;
	}
	else
	{
		pDataStream = (*datastreamList)[sDataStreamID];
	}

	try
	{
		//BufferList
		bufferList = pDataStream->GetBufferList();

		// 4 buffers using internal buffer mode
		for (int i = 0; i<4; i++)
		{
			pBuffer = new BGAPI2::Buffer();
			bufferList->Add(pBuffer);
		}
		
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("δ֪�Ĵ�������"));
		return;
	}

	try
	{
		for (BufferList::iterator bufIterator = bufferList->begin(); bufIterator != bufferList->end(); bufIterator++)
		{
			bufIterator->second->QueueBuffer();
		}
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		
	}

	try
	{
		pDataStream->StartAcquisitionContinuous();
		
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		
	}

	try
	{
		pDevice->GetRemoteNode("TriggerMode")->SetString("Off");
		pDevice->GetRemoteNode("AcquisitionStart")->Execute();
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("��������������"));
		return ;
	}


	
	MessageBox(_T("��ʼ����ϣ�"));
	

	m_button_instance.EnableWindow(false);
	m_button_getImage.EnableWindow(true);
	m_button_close.EnableWindow(true);
	m_button_recog.EnableWindow(false);
	
}//��ʼ��

int CchipUIDlg::iowithWindows(CString &s,Mat &image)//s��������ʶ���ַ���  ��ͼƬ
{
	int index = countRecog;//�Ѽ���Ҳ����excel��������
	int breaked = 0;//�Ƿ���break
	struct MyStruct
	{
		CString lstring;
		CString  rstring;
	};

	stack<MyStruct> p;
	if (repeat(s, index))//�����ظ����������ж��Ƿ��ظ�
	{
		string intToString(int);
		string picPath = iPathPicture + "/" + intToString(index+1) + ".jpg";
		Mat tempImg = imread(picPath);//�ظ���ͼƬ
		CRepeatDialog myrepeat(tempImg, image, istart, iend, index, iPathExcel, AllString[index], AllString, iPathPicture);
		if (myrepeat.DoModal() == IDOK)//tempImg����ͼimage����ͼ��
		{
			MyStruct sold = { myrepeat.edit_str1, myrepeat.edit_str2 };
			p.push(sold);
			
		}
		while (!p.empty())
		{
			MyStruct stemptop = p.top();
			p.pop();
			int label1 = 0;
			int label2 = 0;
			int index2 = index;
			while (repeat2(stemptop.lstring, label1,index2))//����ظ�
			{  
				string intToString(int);
				string picPathTemp1 = iPathPicture + "/" + intToString(label1 + 1) + ".jpg";
				Mat tempImgTemp1 = imread(picPathTemp1);//�ظ���ͼƬ
				string picPathTemp11 = iPathPicture + "/" + intToString(index2 + 1) + ".jpg";
				Mat tempImgTemp11 = imread(picPathTemp11);//�ظ���ͼƬ
				CRepeatDialog tempRepeat1(tempImgTemp1, tempImgTemp11, istart, iend, label1, iPathExcel, AllString[label1], AllString, iPathPicture);
				if (tempRepeat1.DoModal() == IDOK)
				{
					MyStruct snew1 = { tempRepeat1.edit_str1, tempRepeat1.edit_str2 };
					stemptop.lstring = tempRepeat1.edit_str2;
					writeExcelPos(label1, iPathExcel, tempRepeat1.edit_str1);
					AllString[label1] = tempRepeat1.edit_str1;
					index2 = label1;
					UpdateData(false);
				}
			}
					
		       writeExcelPos(index, iPathExcel, stemptop.lstring);
				AllString[index] = stemptop.lstring;
				m_result_new = stemptop.rstring;
			UpdateData(false);

			
			while (repeat(stemptop.rstring, label2))//�ұ��ظ�
			{
				string intToString(int);
				string picPathTemp2 = iPathPicture + "/" + intToString(label2+ 1) + ".jpg";
				Mat tempImgTemp2 = imread(picPathTemp2);//�ظ���ͼƬ
				CRepeatDialog tempRepeat2(tempImgTemp2, image, istart, iend, label2, iPathExcel, AllString[label2], AllString, iPathPicture);
				if (tempRepeat2.DoModal() == IDOK)
				{
					MyStruct snew2 = { tempRepeat2.edit_str1, tempRepeat2.edit_str2 };
					stemptop.rstring = tempRepeat2.edit_str2;
					writeExcelPos(label2, iPathExcel, tempRepeat2.edit_str1);
					AllString[label2] = tempRepeat2.edit_str1;
					m_result_new = stemptop.rstring;
					UpdateData(false);
				}
				else
				{
					breaked = 1;
					break;
				
				}
			}
			if (breaked == 0)
			{
			writeExcel(iPathExcel, m_result_new);
			SavePicture(iPathPicture, image);
			AllString.push_back(m_result_new);
			}
			UpdateData(false);
		}
		return index;
		
	}
	else
	{
		writeExcel(iPathExcel, s);	
		SavePicture(iPathPicture, image);
		m_result_new = s;
		AllString.push_back(s);
		UpdateData(false);
		return index;
	}
	
	
}


bool CchipUIDlg::outRangeCheck(CString &str)
{
	USES_CONVERSION;
	std::string temp(W2A(str));
	if (temp.size() < 2)
		return false;
	if (temp[0] != '1' || temp[1] != '6')
	{
		return false;
	}

	int n = stringToINT(temp);
	if (n >= istart&&n <= iend)
	{
		return true;
	}
	return false;
	
}


void CchipUIDlg::readExcel()
{
	Book *book = xlCreateBook();
	book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");
	if (book)
	{
		if (book->load(iPathExcel))
		{
			Sheet *sheet = book->getSheet(0);
			if (sheet)
			{
				countRecog=sheet->lastRow();
				for (int i = 0; i < countRecog; ++i)
				{
					CString  s_t;
					s_t = sheet->readStr(i, 0);
					AllString.push_back(s_t);
				}
			}
		}
	}
	book->release();
	

}

void CchipUIDlg::GetLostNumber(CString &path)
{
	Book *book = xlCreateBook();
	book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");
	if (book)
	{
		if (book->load(path))
		{
			Sheet *sheet = book->getSheet(0);
			if (sheet)
			{
				int n = sheet->lastRow();
				for (int i = 0; i < n; ++i)
				{
					CString  s_t;
					s_t = sheet->readStr(i, 0);
					lostNumbers.push_back(s_t);
				}
			}
		}
	}
	book->release();
}

bool CchipUIDlg::checkLost(string s)
{
	CString temp_s;
	temp_s = s.c_str();
	for (int i = 0; i < lostNumbers.size(); ++i)
	{
		if (temp_s == lostNumbers[i])
			return false;
	}
	return true;
}

void CchipUIDlg::OnBnClickedGetimage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	
	
	int width = 2592;
	int height = 1944;
	/*BGAPI2::Image * pTransformImage = NULL;*/
	unsigned char* transformBuffer = NULL;
	BGAPI2::Buffer * pBufferFilled = NULL;
	
	//pDataStream->GetBufferList()->FlushInputToOutputQueue();

	/*pDataStream->GetBufferList()->FlushAllToInputQueue();*/
	
	//pDataStream->GetBufferList()->FlushUnqueuedToInputQueue();
	/*pDataStream->GetBufferList()->DiscardOutputBuffers();*/
	/*int count1=pDataStream->GetBufferList()->GetStartedCount();
	int count2 = pDataStream->GetBufferList()->GetQueuedCount();
	int count5 = pDataStream->GetBufferList()->GetAnnouncedCount();
	int count3 = pDataStream->GetBufferList()->GetA
	DeliveryCount();
	int count4 = pDataStream->GetBufferList()->GetDeliveredCount();*/
	

	try
	{			
		for (int i = 0; i < 2; i++)
			{
				pBufferFilled = pDataStream->GetFilledBuffer(1000); //timeout 1000 msec

				//pBufferFilled=pDataStream->GetBufferByIndex(count);
				//pDataStream->GetBufferList()->FlushInputToOutputQueue();
				pDataStream->GetBufferList()->DiscardOutputBuffers();
				if (pBufferFilled == NULL)
				{
					MessageBox(_T("Error: Buffer Timeout after 1000 msec"));

				}
				else if (pBufferFilled->GetIsIncomplete() == true)
				{
					//MessageBox(_T("Error: Image is incomplete"));
					// queue buffer again
					pBufferFilled->QueueBuffer();
					i = -1;
					continue;
				}
				else
				{

					if (1 == i)
					{

						transformBuffer = (unsigned char *)pBufferFilled->GetMemPtr();

						width = pBufferFilled->GetWidth();
						height = pBufferFilled->GetHeight();
					}
					
						pBufferFilled->QueueBuffer();
					
					

				}
				
			}			
		 }	
		
	
	
	catch (BGAPI2::Exceptions::IException& ex)
	{
		KillTimer(1);
		return;
	}
	Mat src(height, width, CV_8UC1);


	for (int i = 0; i < height; ++i)
	{
		uchar *data = src.ptr<uchar>(i);
		for (int j = 0; j < width; ++j)
		{
			data[j] = transformBuffer[i*width + j];
		}
	}
	myImage = src.clone();
	//pBufferFilled->QueueBuffer();
	if (irotate == 1)
	{
		
		transpose(myImage, myImage);
		flip(myImage, myImage, 1);
		flip(myImage, myImage, -1);
	}
	else if (irotate == 2)
	{
		transpose(myImage, myImage);
		flip(myImage, myImage, 1);
	}

	else if (irotate == 3)
	{
		flip(myImage, myImage, -1);
	}
	IplImage src_ = myImage;
	CvvImage cv_src;
	cv_src.CopyOf(&src_);

	CRect rect;
	CDC* pDC = GetDlgItem(IDC_Picture)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	GetDlgItem(IDC_Picture)->GetClientRect(&rect);
	cv_src.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
	
	

	
	m_button_recog.EnableWindow(true);
	
	
	


     //pTransformImage->Release();
	
	
	UpdateData(false);
	



}

int CchipUIDlg::writeExcel(CString &path_,CString &str)
{
	Book *book = xlCreateBook();
	book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");
	if (book)
	{
		if (book->load(path_))
		{
			Sheet* sheet = book->getSheet(0);
			if(sheet)
			{
				int nn = sheet->lastRow();
				sheet->writeStr(nn, 0,str);
			}
		}
		else
		{
			Sheet* sheet = book->addSheet(L"Sheet1");
			if (sheet)
			{
				sheet->writeStr(0, 0, str);
				if (countRecog != 0)
					MessageBox(_T("excel�ļ���ȡʱ��������"));
			}
		}
	}
	book->save(path_);
	book->release();
	return 1;
}

int CchipUIDlg::writeExcelPos(int pos, CString &path_, CString &str)
{
	Book *book = xlCreateBook();
	book->setKey(L"Halil Kural", L"windows-2723210a07c4e90162b26966a8jcdboe");
	if (book)
	{
		if (book->load(path_))
		{
			Sheet* sheet = book->getSheet(0);
			if (sheet)
			{
				/*sheet->writeStr(countRecog++, 0, str);*/
				sheet->writeStr(pos++, 0, str);
			}
		}
		else
		{
			Sheet* sheet = book->addSheet(L"Sheet1");
			if (sheet)
			{
				int l = sheet->lastRow();
				/*sheet->writeStr(l, 0, str);*/
				sheet->writeStr(0, 0, str);
				if (countRecog != 0)
					MessageBox(_T("excel�ļ���ȡʱ��������"));
				countRecog++;
			}
		}
	}
	book->save(path_);
	book->release();

	return 1;
}

int CchipUIDlg::SavePicture(CString &path_,Mat &image)
{
	CString path = path_;
	USES_CONVERSION;
	
	CString csSubString;
	while (-1 != path.Find('\\', 0))
	{
		int nPos = path.Find('\\', 0);
		csSubString += path.Left(nPos + 1);
		if (!PathIsDirectory(csSubString))
		{
			if (!CreateDirectory(csSubString, NULL))
			{
				MessageBox(_T("����·��ʧ��"));
			}
		}
		path = path.Right(path.GetLength() - nPos - 1);
	}
	
	csSubString += path;
	CreateDirectory(csSubString, NULL);

	std::string sPath(W2A(path_));
	string intToString(int a);
	sPath = sPath + "/" + intToString(++countRecog)+".jpg";
	
	
	imwrite(sPath, image);

	return 0;
}


void CchipUIDlg::OnBnClickedRecog()
{
	
	Sleep(pauseSeconds*1000);
	if (checkButton)//��ťģʽ
	OnBnClickedGetimage();
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	double t = (double)getTickCount();
	/*AnnTrain at;
	at.ann = ml::ANN_MLP::load<ml::ANN_MLP>("charnew1.xml");*/
	Chip myChip;
	ChipRecog myRecog;
	/*
	namedWindow("MYIMAGE", WINDOW_NORMAL);
	
	("MYIMAGE", myImage);
	*/
	int lengthP = 0;
	int value = 5;
	int type = 0;
	if ((itype == "GH3205J") || (itype == "JGW4407A") || (itype == "GH3202Z") || (itype == "GH3202S")
		|| (itype == "GH3203J"))
	{
		type = 0;//С��
		myRecog.setRectScale(Size(20, 20), Size(100, 100));
		lengthP = 5;
		value = 5;
	}

	else if ((itype == "GH1146Z") || (itype == "GG4202Z-A") || (itype == "GG4202Z") || (itype == "HCD0201")
		|| (itype == "GW4201Z"))
	{
		type = 2;//����
		myRecog.setRectScale(Size(150, 150), Size(240, 240));
		value = 10;
		lengthP = 10;
	}
	else
	{
		type = 1;//����
		myRecog.setRectScale(Size(20, 20), Size(200, 200));
		lengthP = 10;
		value = 7;
	}
	int find7way = 0;//0Ϊ�Ȳ���SVM ��δ�ҵ����ٲ��ô�ͳ������1Ϊ��֪SVM�ҵ� ����λ���Ҵ�ֱ�Ӳ��ô�ͳ����
	int cyclenum = 0;//0Ϊ�Ѿ��ҹ�һ��7���ҹ�����7��++������������7��ѭ����
	find7:
	myChip.image = myImage;
	int countIndex = 0;
	if (itype != "GH0201J"&&itype != "GH3201J-2")
	{
		countIndex = myRecog.chipLocate(myChip.image, myChip.chipImage);//�����ο� �Ҿ��ο����ڸ�
		//imwrite("jietu.jpg", myChip.chipImage);
	}
	else
	{
		countIndex = myRecog.chipLocate2(myChip.image, myChip.chipImage);//�����ο� 
	}
	


	if (countIndex == 0x002)//����2 ���������ͼƬ��û�о��ο�
	{
		CSetDialog myset(myChip.image,istart,iend);
		if (myset.DoModal() == IDOK)
		{
			
			iowithWindows(myset.m_edit, myChip.image);
			/*while (checkRepeat())
			{
				int aaaaa = 0;
			}*/
			UpdateData(false);
		}
		else
		{
			string str1118 = "***" + intToString(countRecog);
			CString str1118_;
			str1118_ = str1118.c_str();
			iowithWindows(str1118_, myChip.image);
			/*while (checkRepeat())
			{
				int aaaaa = 0;
			}*/
			UpdateData(false);
		}
		return;		
	}
	
	
	int index = 0;
	try
	{
		/*if (specialChar2 == "6")
			index = myRecog.FindSpecialChar(pBuffer_path, myChip.chipImage, myChip.chipSize, mySvm, lengthP, itype);
		else
			index = myRecog.FindNumberOne(path_cas1, myChip.chipImage, myChip.chipSize, nn, lengthP, itype);*/
		if (specialChar2 == "6"&&itype!="GH3201J"&&itype!="GH3202S")   
			index = myRecog.FindSpecialChar(pBuffer_path, myChip.chipImage, myChip.chipSize, mySvm, lengthP, type);
		else if (specialChar2 == "7")
		{
		
			index = myRecog.FindSpecialNumberSeven(myChip.chipImage, myChip.chipSize, pBuffer_path,find7way);//����оƬͼƬ�����7�Ŀ�ߴ磻
		}
		else 
		{
			myChip.chipImage = myChip.chipImage.colRange(myChip.chipImage.cols / 30, myChip.chipImage.cols);
			index = myRecog.FindSpecialFirstNumber(myChip.chipImage, myChip.chipSize);
		}
	}
	catch (Exception& ex)
	{
		
		/*CString aaa;
		aaa = "unknow error";
		AfxMessageBox(aaa);
		return;*/
		//7��11���滻��Խ���ʱ�򵯿�ȥ�����µ�
		CSetDialog myset(myChip.image, istart, iend);
		if (myset.DoModal() == IDOK)
		{

			iowithWindows(myset.m_edit, myChip.image);
			/*while (checkRepeat())
			{
			int aaaaa = 0;
			}*/
			UpdateData(false);
		}
		else
		{
			string str1118 = "***" + intToString(countRecog);
			CString str1118_;
			str1118_ = str1118.c_str();
			iowithWindows(str1118_, myChip.image);
			/*while (checkRepeat())
			{
			int aaaaa = 0;
			}*/
			UpdateData(false);
		}
		return;

	}
	

	t = ((double)getTickCount() - t) / getTickFrequency();
	string s = specialChar1+specialChar2;
	if (index != 0)
	{
		CSetDialog myset(myChip.image, istart, iend);
		if (myset.DoModal() == IDOK)
		{
			iowithWindows(myset.m_edit, myChip.image);
			/*while (checkRepeat())
			{
				int aaaaa = 0;
			}*/
		}
		else
		{
			string str1118 = "***" + intToString(countRecog);
			CString str1118_;
			str1118_ = str1118.c_str();
			iowithWindows(str1118_, myChip.image);
			/*while (checkRepeat())
			{
				int aaaaa = 0;
			}*/
			UpdateData(false);
		}
		UpdateData(false);
		return;
	}
		

	
	int number = inumber;//оƬ��Ҫ��⵽�����ֵĸ���


	/*if (specialChar2 == "6"&&itype != "GH3201J"&&itype != "GH3202S")
		myRecog.chipRecogSegment(myChip.chipImage, myChip.chipSize, s, nn, number, value);
	else*/
	find7way=myRecog.chipRecogSegmentOne(myChip.chipImage, myChip.chipSize, s, nn, number);
		//���ݵ�һ�ҵ��������ҵ�������������
		//оƬͼƬ���ҵ��ĵ�һ�����ֿ�ĳߴ磻������ִ��������ѧϰ��������Ҫ��������ָ���
	if (find7way == 1 && cyclenum == 0)
	{
		cyclenum++;
		goto find7;
	}
	find7way = 0;//��������7�󣬽����ʼ����������һ��ͼƬֱ�Ӵӷ���1��ʼ
	if (s.size() == number)
	{

		CString ttt;
		ttt = s.c_str();
		int ttt2_int = stringToINT(s);
		if (ttt2_int >= istart&&ttt2_int <= iend)
		{	
			if (checkLost(s))
			{
				iowithWindows(ttt, myChip.image);
				DrawResult(s);
				return;
			}
			else
			{
				CSetDialog myset(myChip.image, istart, iend);
				if (myset.DoModal() == IDOK)
				{
					iowithWindows(myset.m_edit, myChip.image);
				}
				else
				{
					string str1118 = "***" + intToString(countRecog);
					CString str1118_;
					str1118_ = str1118.c_str();
					iowithWindows(str1118_, myChip.image);
					UpdateData(false);
				}
				UpdateData(false);
				return;
			}
		}
		else
		{
			CSetDialog myset(myChip.image, istart, iend);
			if (myset.DoModal() == IDOK)
			{
				iowithWindows(myset.m_edit, myChip.image);
				/*while (checkRepeat())
				{
					int aaaaa = 0;
				}*/
			}
			else
			{
				string str1118 = "***" + intToString(countRecog);
				CString str1118_;
				str1118_ = str1118.c_str();
				iowithWindows(str1118_, myChip.image);
				/*while (checkRepeat())
				{
					int aaaaa = 0;
				}*/
				UpdateData(false);
			}
			UpdateData(false);
			return;
		}
		/*while (checkRepeat())
		{
			int aaaaa = 0;
		}*/
	}
	else
	{
		CSetDialog myset(myChip.image, istart, iend);
		if (myset.DoModal() == IDOK)
		{
			iowithWindows(myset.m_edit, myChip.image);
			/*while (checkRepeat())
			{
				int aaaaa = 0;
			}*/
		}
		else
		{
			string str1118 = "***" + intToString(countRecog);
			CString str1118_;
			str1118_ = str1118.c_str();
			iowithWindows(str1118_, myChip.image);
			/*while (checkRepeat())
			{
				int aaaaa = 0;
			}*/
			UpdateData(false);
		}
		UpdateData(false);
		return;
	}
		

	
	
	
	

}


bool CchipUIDlg::repeat(CString str,int &index)
{
	for (int i = 0; i < AllString.size(); ++i)
	{
		if (str == AllString[i])
		{
			index = i;
			return true;
		}
	}
	return false;
}

bool CchipUIDlg::repeat2(CString str, int &index,int&not)
{
	for (int i = 0; i < AllString.size(); ++i)
	{  
		if (i==not)
		{
			i += 1;
		}
		if (i == AllString.size())
		{
			return false;
		}
		if (str == AllString[i])
		{
			index = i;
			return true;
		}
	}
	return false;
}

void CchipUIDlg::OnBnClickedOpenpicture()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog openDialog(true);
	if (openDialog.DoModal() == IDOK)
	{
		CDC* pDC = GetDlgItem(IDC_Picture)->GetDC();
		HDC hDC = pDC->GetSafeHdc();
		CString filename = openDialog.GetPathName();
		USES_CONVERSION;
		std::string path(W2A(filename));
		Mat src = imread(path);
		
		myImage = src.clone();
		if (irotate == 1)
		{
			transpose(myImage, myImage);
			flip(myImage, myImage, 1);
			flip(myImage, myImage, -1);
		}
		else if (irotate == 2)
		{
			transpose(myImage, myImage);
			flip(myImage, myImage, 1);
		}
		else if (irotate == 3)
		{

			flip(myImage, myImage, -1);
		}
		IplImage src_ = myImage;
		CvvImage cv_src;
		cv_src.CopyOf(&src_);
		CRect rect;

		GetDlgItem(IDC_Picture)->GetClientRect(&rect);
		cv_src.DrawToHDC(hDC, &rect);
		ReleaseDC(pDC);

		UpdateData(false);
				
							
		
		
		
		m_button_recog.EnableWindow(true);
	}
}


void CchipUIDlg::OnBnClickedClose()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	

	//STOP CAMERA
	try
	{
		//SEARCH FOR 'AcquisitionAbort'
		if (pDevice->GetRemoteNodeList()->GetNodePresent("AcquisitionAbort"))
		{
			pDevice->GetRemoteNode("AcquisitionAbort")->Execute();
		}
		pDevice->GetRemoteNode("AcquisitionStop")->Execute();
		
		if (pDevice->GetTLType() == "GEV")
		{
			if (pDevice->GetRemoteNodeList()->GetNodePresent("DeviceStreamChannelPacketSize"))
				std::cout << "         DeviceStreamChannelPacketSize:  " << pDevice->GetRemoteNode("DeviceStreamChannelPacketSize")->GetInt() << " [bytes]" << std::endl;
			else
				std::cout << "         GevSCPSPacketSize:              " << pDevice->GetRemoteNode("GevSCPSPacketSize")->GetInt() << " [bytes]" << std::endl;
			std::cout << "         GevSCPD (PacketDelay):          " << pDevice->GetRemoteNode("GevSCPD")->GetInt() << " [tics]" << std::endl;
		}
		std::cout << std::endl;
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		//MessageBox(_T("�ر�1����"));
	}

	//STOP DataStream acquisition
	try
	{
		if (pDataStream->GetTLType() == "GEV")
		{
			//DataStream Statistic
			std::cout << "         DataStream Statistics " << std::endl;
			std::cout << "           GoodFrames:            " << pDataStream->GetNodeList()->GetNode("GoodFrames")->GetInt() << std::endl;
			std::cout << "           CorruptedFrames:       " << pDataStream->GetNodeList()->GetNode("CorruptedFrames")->GetInt() << std::endl;
			std::cout << "           LostFrames:            " << pDataStream->GetNodeList()->GetNode("LostFrames")->GetInt() << std::endl;
			std::cout << "           ResendRequests:        " << pDataStream->GetNodeList()->GetNode("ResendRequests")->GetInt() << std::endl;
			std::cout << "           ResendPackets:         " << pDataStream->GetNodeList()->GetNode("ResendPackets")->GetInt() << std::endl;
			std::cout << "           LostPackets:           " << pDataStream->GetNodeList()->GetNode("LostPackets")->GetInt() << std::endl;
			std::cout << "           Bandwidth:             " << pDataStream->GetNodeList()->GetNode("Bandwidth")->GetInt() << std::endl;
			std::cout << std::endl;
		}
		if (pDataStream->GetTLType() == "U3V")
		{
			//DataStream Statistic
			std::cout << "         DataStream Statistics " << std::endl;
			std::cout << "           GoodFrames:            " << pDataStream->GetNodeList()->GetNode("GoodFrames")->GetInt() << std::endl;
			std::cout << "           CorruptedFrames:       " << pDataStream->GetNodeList()->GetNode("CorruptedFrames")->GetInt() << std::endl;
			std::cout << "           LostFrames:            " << pDataStream->GetNodeList()->GetNode("LostFrames")->GetInt() << std::endl;
			std::cout << std::endl;
		}

		//BufferList Information
		std::cout << "         BufferList Information " << std::endl;
		std::cout << "           DeliveredCount:        " << bufferList->GetDeliveredCount() << std::endl;
		std::cout << "           UnderrunCount:         " << bufferList->GetUnderrunCount() << std::endl;
		std::cout << std::endl;

		pDataStream->StopAcquisition();
		std::cout << "5.1.12   DataStream stopped " << std::endl;
		bufferList->DiscardAllBuffers();
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		//MessageBox(_T("�ر�2����"));
	}
	std::cout << std::endl;


	std::cout << "RELEASE" << std::endl;
	std::cout << "#######" << std::endl << std::endl;

	//Release buffers
	std::cout << "5.1.13   Releasing the resources " << std::endl;
	try
	{
		//MessageBox(_T("�ر�3-0"));
		while (bufferList->size() > 0)
		{
			pBuffer = bufferList->begin()->second;
			bufferList->RevokeBuffer(pBuffer);
			delete pBuffer;
		}
		std::cout << "         buffers after revoke:    " << bufferList->size() << std::endl;
		
		pDataStream->Close();
		
		
		pDevice->Close();
		
		
		pInterface->Close();
		
		pSystem->Close();
		
		BGAPI2::SystemList::ReleaseInstance();
		
		
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		
	}

	std::cout << std::endl;
	std::cout << "End" << std::endl << std::endl;

	std::cout << "Input any number to close the program:";
	
	m_button_instance.EnableWindow(true);
	m_button_getImage.EnableWindow(false);
	m_button_close.EnableWindow(true);
	m_button_recog.EnableWindow(false);
	return ;
}



void CchipUIDlg::OnBnClickedSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	if (myImage.empty())
	{				
		MessageBox(_T("ͼƬΪ��"));
	}
	else
	{
		CTime ct;
		ct=CTime::GetTickCount();
		int hour = ct.GetHour();
		int minnute = ct.GetMinute();
		int second = ct.GetSecond();
		int day = ct.GetDayOfWeek();
		imwrite("D:/2016_10_19оƬ/" +intToString(day)+ intToString(hour) + "_"+intToString(minnute) +"_"+ intToString(second) + ".jpg", myImage);
		MessageBox(_T("������ɣ�"));
	}
}

bool CchipUIDlg::checkRepeat()
{
	for (int i = 0; i < AllString.size()-1; ++i)
	{
		for (int j = i+1; j < AllString.size() ; ++j)
		{
			if (AllString[i] == AllString[j]&&AllString[i]!="delete")
			{
				string intToString(int);
				string picPath1 = iPathPicture + "/" + intToString(i+1) + ".jpg";
				string picPath2 = iPathPicture + "/" + intToString(j+1) + ".jpg";
				Mat tempImg = imread(picPath1);
				Mat tempImg2 = imread(picPath2);
				CRepeatDialog myrepeat(tempImg, tempImg2, istart, iend, i, iPathExcel, AllString[i], AllString, iPathPicture);
				if (myrepeat.DoModal() == IDOK)
				{
					writeExcelPos(i, iPathExcel, myrepeat.edit_str1);
					writeExcelPos(j,iPathExcel, myrepeat.edit_str2);
					//SavePicture(iPathPicture, image);
					m_result_new = myrepeat.edit_str2;
					AllString[i]=myrepeat.edit_str1;
					AllString[j] = myrepeat.edit_str2;
					UpdateData(false);
				}
				else
				{
					return false;
				}


				return true;
			}
		}
	}

	return false;
}


void CchipUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
		OnBnClickedGetimage();
	CDialogEx::OnTimer(nIDEvent);
}


//BOOL CchipUIDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO:  �ڴ����ר�ô����/����û���
//	if (pMsg->message == WM_KEYDOWN&&m_button_recog.IsWindowEnabled())
//	{
//		switch (pMsg->wParam)
//		{
//		case VK_NUMPAD0:    // ���λس�
//			OnBnClickedRecog();
//				return TRUE;
//		}
//	}
//	return CDialog::PreTranslateMessage(pMsg);
//
//	
//}

void CchipUIDlg::OnCbnSelchangeCombo6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_cb6.GetLBText(m_cb6.GetCurSel(), str1);
	if (str1=="˳ʱ����ת90��")
	{
		irotate = 2;
	}
	 else if (str1=="��ʱ����ת90��")
	{
		irotate = 1;
	}
	 else if (str1 == "��ת180��")
	 {
		 irotate = 3;
	 }
	 else
	 {
		 irotate = 0;
	 }

}

void CchipUIDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	OnBnClickedClose();
	CDialogEx::OnClose();
}


void CchipUIDlg::OnBnClickedButtonDebug()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SetTimer(1, 200, NULL);
}


void CchipUIDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
}


void CchipUIDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


int CchipUIDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	if (!RegisterHotKey(this->m_hWnd, 2009, NULL, VK_DECIMAL))
	{
		MessageBox(_T("Hotkeys already registered by other program"));
		return false;
	}
	return 0;
}


void CchipUIDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	UnregisterHotKey(this->m_hWnd, 2009);
	// TODO:  �ڴ˴������Ϣ����������
}


void CchipUIDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (hot_key_check)
		return;
	hot_key_check = true;
	 OnBnClickedRecog();
	 hot_key_check = false;
     return;
	
	

	
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CchipUIDlg::OnBnClickedButtonrepair()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CSetDialog myset(myImage, istart, iend);
	if (myset.DoModal() == IDOK)
	{	//����
		int label2 = 0;
		int breaked = 0;
		m_result_new = myset.m_edit;
	    while (repeat(myset.m_edit, label2))//�ұ��ظ�
		{
			string intToString(int);
			string picPathTemp2 = iPathPicture + "/" + intToString(label2 + 1) + ".jpg";
			Mat tempImgTemp2 = imread(picPathTemp2);//�ظ���ͼƬ
			CRepeatDialog tempRepeat2(tempImgTemp2, myImage, istart, iend, label2, iPathExcel, AllString[label2], AllString, iPathPicture);
			if (tempRepeat2.DoModal() == IDOK)
			{
				myset.m_edit = tempRepeat2.edit_str2;
				writeExcelPos(label2, iPathExcel, tempRepeat2.edit_str1);
				AllString[label2] = tempRepeat2.edit_str1;
				m_result_new = myset.m_edit;
				UpdateData(false);
			}
			else
			{
				breaked = 1;
				break;

			}
		}
		if (breaked == 0)
		{
			writeExcelPos(countRecog - 1, iPathExcel, myset.m_edit);
			AllString[countRecog - 1] = myset.m_edit;
			m_result_new = myset.m_edit;
		}
	
      //
			
	}
	

	UpdateData(false);

}

void CchipUIDlg::drawPicture(Mat &image)
{
	Mat input = image.clone();
	IplImage src_ = input;
	CvvImage cv_src;
	cv_src.CopyOf(&src_);
	CRect rect;
	CDC* pDC = GetDlgItem(IDC_Picture)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	GetDlgItem(IDC_Picture)->GetClientRect(&rect);
	cv_src.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);

	UpdateData(false);
}

void CchipUIDlg::OnBnClickedCaculate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (istart >= iend || AllString.empty())
	{
		MessageBox(_T("�����Ƿ��ѽ���ʶ�������ļ�����"));
		return;
	}
	
	int *allRange = new int[iend - istart + 1];

	USES_CONVERSION;
	
	for (int i = 0; i < AllString.size(); ++i)
	{
		string s_temp = W2A(AllString[i]);
		int i_temp = stringToINT(s_temp);
		allRange[i_temp - istart] = 1;
	}
	Book *book = xlCreateBook();
	if (book)
	{
		Sheet *sheet = book->addSheet(L"Sheet1");
		if (sheet)
		{
			for (int i = 0; i < iend - istart + 1; ++i)
			{
				if (allRange[i] != 1)
				{
					size_t maxRows = sheet->lastRow();
					if (sheet)
					{
						string s__temp = intToString(i + istart);
						CString ssss;
						ssss = s__temp.c_str();
						sheet->writeStr(maxRows, 0, ssss);
					}
				}
			}
		}
	}
	string s_iPathExcel(W2A(iPathExcel));
	
	int posMax = s_iPathExcel.size();
	
	if (posMax < 4)
	{
		MessageBox(_T("����excel����"));
		return;
	}
	s_iPathExcel[posMax - 4] = '(';
	s_iPathExcel[posMax - 3] = '2';
	s_iPathExcel[posMax - 2] = ')';
	s_iPathExcel[posMax - 1] = '.';
	s_iPathExcel.push_back('x');
	s_iPathExcel.push_back('l');
	s_iPathExcel.push_back('s');
	CString ss_ss;
	ss_ss = s_iPathExcel.c_str();
	book->save(ss_ss);
	
	MessageBox(_T("ͳ�Ƴɹ����ѱ���"));
}

void BGAPI2CALL BufferHandler(void *callBackOwner, Buffer *pBufferFilled)
{
	if (dlg->firstReturn == false)
	{
		dlg->firstReturn = true;
		return;
	}
	if (dlg->hot_key_check)
		return;
	dlg->hot_key_check = true;
	if (pBufferFilled == NULL)
	{

	}
	else if (pBufferFilled->GetIsIncomplete() == true)
	{
		pBufferFilled->QueueBuffer();
	}
	else
	{
		uchar *transformBuffer = NULL;
		transformBuffer = (uchar*)pBufferFilled->GetMemPtr();

		__int64 unWidth = pBufferFilled->GetWidth();
		__int64 unHeight = pBufferFilled->GetHeight();
		Mat src(unHeight, unWidth, CV_8UC1);
		for (int i = 0; i < src.rows; ++i)
		{
			uchar *data = src.ptr<uchar>(i);
			for (int j = 0; j < src.cols; ++j)
			{
				data[j] = transformBuffer[i*src.cols + j];
			}
		}


		dlg->myImage = src.clone();

		if (dlg->irotate == 1)
		{
			transpose(dlg->myImage, dlg->myImage);
			flip(dlg->myImage, dlg->myImage, 1);
			flip(dlg->myImage, dlg->myImage, -1);
		}
		else if (dlg->irotate == 2)
		{
			transpose(dlg->myImage, dlg->myImage);
			flip(dlg->myImage, dlg->myImage, 1);
		}

		else if (dlg->irotate == 3)
		{
			flip(dlg->myImage, dlg->myImage, -1);
		}


		dlg->drawPicture(dlg->myImage);
		
		dlg->OnBnClickedRecog();
		dlg->hot_key_check = false;
		pBufferFilled->QueueBuffer();
	}

}


void CchipUIDlg::OnBnClickedtouch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	//MessageBox(_T("1"));
	KillTimer(1);
	//MessageBox(_T("2"));
	OnBnClickedClose();
	//MessageBox(_T("3"));
	checkButton = false;
	m_check_Button.SetCheck(0);
	//MessageBox(_T("4"));
	try
	{
		systemList = SystemList::GetInstance();
		systemList->Refresh();

		//SYSTEM DEVICE INFORMATION
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("ϵͳ��ʼ��ʧ��"));
		return;
	}

	//OPEN THE FIRST SYSTEM IN THE LIST WITH A CAMERA CONNECTED
	try
	{
		for (SystemList::iterator sysIterator = systemList->begin(); sysIterator != systemList->end(); sysIterator++)
		{
			try
			{
				sysIterator->second->Open();

				sSystemID = sysIterator->first;

				try
				{
					interfaceList = sysIterator->second->GetInterfaces();
					//COUNT AVAILABLE INTERFACES
					interfaceList->Refresh(100); // timeout of 100 msec					
					//INTERFACE INFORMATION					
				}
				catch (BGAPI2::Exceptions::IException& ex)
				{
					MessageBox(_T("δ��⵽����"));
					return;
				}




				//OPEN THE NEXT INTERFACE IN THE LIST
				try
				{
					for (InterfaceList::iterator ifIterator = interfaceList->begin(); ifIterator != interfaceList->end(); ifIterator++)
					{
						try
						{

							ifIterator->second->Open();
							//search for any camera is connetced to this interface
							deviceList = ifIterator->second->GetDevices();
							deviceList->Refresh(100);
							if (deviceList->size() == 0)
							{
								ifIterator->second->Close();
							}
							else
							{
								sInterfaceID = ifIterator->first;

								if (ifIterator->second->GetTLType() == "GEV")
								{
									bo_int64 iIpAddress = ifIterator->second->GetNode("GevInterfaceSubnetIPAddress")->GetInt();

									bo_int64 iSubnetMask = ifIterator->second->GetNode("GevInterfaceSubnetMask")->GetInt();

								}
								if (ifIterator->second->GetTLType() == "U3V")
								{

								}

								break;
							}
						}
						catch (BGAPI2::Exceptions::ResourceInUseException& ex)
						{
							MessageBox(_T("������Ϣ��ȡʧ��"));
						}
					}
				}
				catch (BGAPI2::Exceptions::IException& ex)
				{
					MessageBox(_T("�������ʧ��"));
					return;
				}


				//if a camera is connected to the system interface then leave the system loop
				if (sInterfaceID != "")
				{
					break;
				}
			}
			catch (BGAPI2::Exceptions::ResourceInUseException& ex)
			{
				MessageBox(_T("ϵͳ�Ѿ���"));
				return;
			}
		}
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("��������"));
		return;
	}

	if (sSystemID == "")
	{
		MessageBox(_T("δ�ҵ�ϵͳ"));
		BGAPI2::SystemList::ReleaseInstance();
		return;
	}
	else
	{
		pSystem = (*systemList)[sSystemID];
	}


	if (sInterfaceID == "")
	{

		MessageBox(_T("δ�ҵ����"));
		pSystem->Close();
		BGAPI2::SystemList::ReleaseInstance();
		return;
	}
	else
	{
		pInterface = (*interfaceList)[sInterfaceID];
	}


	try
	{
		//COUNTING AVAILABLE CAMERAS
		deviceList = pInterface->GetDevices();
		deviceList->Refresh(100);


		//DEVICE INFORMATION BEFORE OPENING

	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("��ȡ���ʧ��"));
	}

	//OPEN THE FIRST CAMERA IN THE LIST
	try
	{
		for (DeviceList::iterator devIterator = deviceList->begin(); devIterator != deviceList->end(); devIterator++)
		{
			try
			{
				devIterator->second->Open();
				sDeviceID = devIterator->first;

				//SERIAL NUMBER

				break;
			}
			catch (BGAPI2::Exceptions::ResourceInUseException& ex)
			{
				MessageBox(_T("����Ѿ���"));
			}
			catch (BGAPI2::Exceptions::AccessDeniedException& ex)
			{
				MessageBox(_T("����Ѿ���"));
			}
		}
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("��������"));
	}

	if (sDeviceID == "")
	{
		MessageBox(_T("δ�ҵ���Ӧ�豸"));
		pInterface->Close();
		pSystem->Close();
		BGAPI2::SystemList::ReleaseInstance();
		return;
	}
	else
	{
		pDevice = (*deviceList)[sDeviceID];
	}

	try
	{
		//SET TRIGGER MODE OFF (FreeRun)
		//pDevice->GetRemoteNode("TriggerMode")->SetString("Off");
		pDevice->GetRemoteNode("PixelFormat")->SetString("Mono8");
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("���ø�ʽ����"));
	}

	try
	{
		//COUNTING AVAILABLE DATASTREAMS

		datastreamList = pDevice->GetDataStreams();
		datastreamList->Refresh();
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("�õ�����������"));
	}

	//OPEN THE FIRST DATASTREAM IN THE LIST
	try
	{
		for (DataStreamList::iterator dstIterator = datastreamList->begin(); dstIterator != datastreamList->end(); dstIterator++)
		{
			sDataStreamID = dstIterator->first;
			//MessageBox(_T("8-5"));
			dstIterator->second->Open();
			//MessageBox(_T("8-6"));

			break;
		}
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("�򿪵�һ������������"));
	}

	if (sDataStreamID == "")
	{
		
		MessageBox(_T("��������ȡʧ��"));
		pDevice->Close();
		pInterface->Close();
		pSystem->Close();
		
		BGAPI2::SystemList::ReleaseInstance();
		return;
	}
	else
	{
		//MessageBox(_T("8-7"));
		pDataStream = (*datastreamList)[sDataStreamID];
	}

	try
	{
		//BufferList
		//MessageBox(_T("8-8"));
		bufferList = pDataStream->GetBufferList();

		// 4 buffers using internal buffer mode
		for (int i = 0; i<4; i++)
		{
			pBuffer = new BGAPI2::Buffer();
			bufferList->Add(pBuffer);
		}
		//MessageBox(_T("8-9"));

	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("δ֪�Ĵ�������"));
		return;
	}

	try
	{
		//MessageBox(_T("8-10"));
		for (BufferList::iterator bufIterator = bufferList->begin(); bufIterator != bufferList->end(); bufIterator++)
		{
			bufIterator->second->QueueBuffer();
		}
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{

	}

	try
	{
		//MessageBox(_T("8-11"));
		pDataStream->StartAcquisitionContinuous();
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{

	}

	try
	{
		//MessageBox(_T("9"));
		pDevice->GetRemoteNode("TriggerMode")->SetString("On");
		//MessageBox(_T("10"));
		pDevice->GetRemoteNode("TriggerSource")->SetString("Line0");
		//MessageBox(_T("11"));
		pDataStream->RegisterNewBufferEventHandler(pDataStream, (Events::NewBufferEventHandler)&BufferHandler);
		//MessageBox(_T("12"));
		pDevice->GetRemoteNode("AcquisitionStart")->Execute();
		//MessageBox(_T("13"));
	}
	catch (BGAPI2::Exceptions::IException& ex)
	{
		MessageBox(_T("��������������"));
		return;
	}



	//MessageBox(_T("��ʼ����ϣ����ȷ����ʼ��һ��ʶ��"));
	/*MessageBox(_T("14"));*/
	m_button_instance.EnableWindow(false);
	m_button_getImage.EnableWindow(false);
	m_button_close.EnableWindow(false);
	m_button_recog.EnableWindow(false);
	m_debug.EnableWindow(false);
	m_stopDebug.EnableWindow(false);
	m_openPicture.EnableWindow(false);
	m_SavePicture.EnableWindow(false);
	m_check_Button.SetCheck(1);
	//MessageBox(_T("15"));
}


void CchipUIDlg::OnBnClickedcanceltouch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
	OnBnClickedClose();
	checkButton = true;
	m_button_instance.EnableWindow(true);
	m_button_getImage.EnableWindow(true);
	m_button_close.EnableWindow(true);
	m_button_recog.EnableWindow(true);
	m_debug.EnableWindow(true);
	m_stopDebug.EnableWindow(true);
	m_openPicture.EnableWindow(true);
	m_SavePicture.EnableWindow(true);
	m_check_Button.SetCheck(0);
	dlg->firstReturn = false;
}


void CchipUIDlg::OnBnClickedcheckbutton()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_check_Button.GetCheck())
		OnBnClickedtouch();
	else
	{
		OnBnClickedcanceltouch();
		OnBnClickedInstance();
	}
}


void CchipUIDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedClose();
	CDialogEx::OnCancel();
}
