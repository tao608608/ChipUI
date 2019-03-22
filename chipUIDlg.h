
// chipUIDlg.h : 头文件
//
#include "bgapi2_genicam.hpp"
#include "opencv2/opencv.hpp"
#include "afxwin.h"
#include "ann.h"
#include "svm.h"
#include "SetDialog.h"
#include "RepeatDialog.h"
#include <memory>
#include "tiny_dnn/tiny_dnn.h"
#include "opencv2/opencv.hpp"
#include "io.h"

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;

using namespace std;
using namespace cv;

using namespace BGAPI2;

#pragma once


// CchipUIDlg 对话框
class CchipUIDlg : public CDialogEx
{
// 构造
public:
	CchipUIDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHIPUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void CreateStretchImage(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedInstance();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedGetimage();
	afx_msg void OnBnClickedRecog();
	afx_msg void OnBnClickedOpenpicture();


public:
	//BGAPI2::ImageProcessor * imgProcessor = NULL;
	BGAPI2::SystemList *systemList = NULL;
	BGAPI2::System * pSystem = NULL;
	BGAPI2::String sSystemID;

	BGAPI2::InterfaceList *interfaceList = NULL;
	BGAPI2::Interface * pInterface = NULL;
	BGAPI2::String sInterfaceID;

	BGAPI2::DeviceList *deviceList = NULL;
	BGAPI2::Device * pDevice = NULL;
	BGAPI2::String sDeviceID;

	BGAPI2::DataStreamList *datastreamList = NULL;
	BGAPI2::DataStream * pDataStream = NULL;
	BGAPI2::String sDataStreamID;

	BGAPI2::BufferList *bufferList = NULL;
	BGAPI2::Buffer * pBuffer = NULL;
	BGAPI2::String sBufferID;
	
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedSave();
	

public:
	cv::Mat myImage;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_result_new;
	CButton m_button_instance;
	CButton m_button_getImage;
	CButton m_button_close;
	CButton m_button_recog;
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int countRecog;
	int irotate;
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnEnChangeResultEdit();
	

private:
	int inumber;
	CString str1;
	
	string itype;

	int istart;
	int iend;
	CString iPathExcel;
	CString iPathPicture;
	string iPathExcel_;
	string iPathPicture_;
public:
	int iowithWindows(CString &,Mat &image);
	CComboBox m_cb6;
	afx_msg void OnCbnSelchangeCombo6();

	CString current_time;
	CString timeForSave;
	CFont m_editFont;
	afx_msg void OnCbnSelchangeCombo7();
	CComboBox m_cb7;
	afx_msg void OnClose();
	//AnnTrain at;
	SvmTrain mySvm;
	

	afx_msg void OnBnClickedButtonDebug();
	afx_msg void OnBnClickedButton3();
public:
	int writeExcel(CString &,CString &);
	afx_msg void OnPaint();
	
	double pauseSeconds;

public:
	int SavePicture(CString &path,Mat &image);
	vector<CString>AllString;
	bool repeat(CString str,int &index);
	bool repeat2(CString str, int &index, int&not);
	int writeExcelPos(int pos, CString &time, CString &str);
	string pBuffer_path;
	char   ppbuffer[MAX_PATH];
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);

	void readExcel();
	bool outRangeCheck(CString &);
	bool checkRepeat();
	
	afx_msg void OnBnClickedButtonrepair();

public:
	network<sequential> nn;
	ifstream ifs;
	afx_msg void OnBnClickedCaculate();
	CEdit m_editPlace;
	afx_msg void OnBnClickedtouch();
	CButton m_debug;
	CButton m_stopDebug;
	CButton m_openPicture;
	CButton m_SavePicture;
	afx_msg void OnBnClickedcanceltouch();
	void drawPicture(Mat &image);
	string specialChar1;
	string specialChar2;
	
	bool hot_key_check;
	string path_cas1;
	bool checkButton;
	int errorCount;
	bool firstReturn;
	afx_msg void OnBnClickedcheckbutton();
	CButton m_check_Button;
	
	afx_msg void OnBnClickedCancel();

	void GetLostNumber(CString &path);

	vector<CString>lostNumbers;
	bool checkLost(string s);
	CString lostPathExcel;
	string lostPathExcel_;
};
