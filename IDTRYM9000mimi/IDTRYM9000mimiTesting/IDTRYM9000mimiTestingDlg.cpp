
// IDTRYM9000mimiTestingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IDTRYM9000mimiTesting.h"
#include "IDTRYM9000mimiTestingDlg.h"
#include "afxdialogex.h"
#include "ImportDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CIDTRYM9000mimiTestingDlg 对话框




CIDTRYM9000mimiTestingDlg::CIDTRYM9000mimiTestingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIDTRYM9000mimiTestingDlg::IDD, pParent)
	, m_csResults(_T(""))
	, m_nPort(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIDTRYM9000mimiTestingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_csResults);
	DDX_Text(pDX, IDC_EDIT2, m_nPort);
}

BEGIN_MESSAGE_MAP(CIDTRYM9000mimiTestingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_Open, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtOpen)
	ON_BN_CLICKED(IDC_BT_RFturnon, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtRfturnon)
	ON_BN_CLICKED(IDC_BT_RFTrunoff, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtRftrunoff)
	ON_BN_CLICKED(IDC_BT_Authentication, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtAuthentication)
	ON_BN_CLICKED(IDC_BT_Getdata, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtGetdata)
	ON_BN_CLICKED(IDC_BT_CRT_GetStatus, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtCrtGetstatus)
	ON_BN_CLICKED(IDC_BT_ActiveIC, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtActiveic)
	ON_BN_CLICKED(IDC_BT_DeactivateIC, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtDeactivateic)
	ON_BN_CLICKED(IDC_BT_ActiveContactlessIC, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtActivecontactlessic)
	ON_BN_CLICKED(IDC_BT_DeactivateContactlessIC, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtDeactivatecontactlessic)
	ON_BN_CLICKED(IDC_BT_WarmReset, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtWarmreset)
	ON_BN_CLICKED(IDC_BT_ICcommand, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtIccommand)
	ON_BN_CLICKED(IDC_BT_RFcommand, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtRfcommand)
	ON_BN_CLICKED(IDC_BT_GetDevCategory, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtGetdevcategory)
	ON_BN_CLICKED(IDC_BT_Reset, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtReset)
	ON_BN_CLICKED(IDC_BT_DevClose, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtDevclose)
	ON_BN_CLICKED(IDC_BT_GetLastErr, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtGetlasterr)
	ON_BN_CLICKED(IDC_BT_CRTOPEN, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtCrtopen)
	ON_BN_CLICKED(IDC_Bt_Create, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtCreate)
	ON_BN_CLICKED(IDC_Bt_Release, &CIDTRYM9000mimiTestingDlg::OnBnClickedBtRelease)
END_MESSAGE_MAP()


// CIDTRYM9000mimiTestingDlg 消息处理程序

BOOL CIDTRYM9000mimiTestingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_nPort=7;
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIDTRYM9000mimiTestingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIDTRYM9000mimiTestingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIDTRYM9000mimiTestingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//*****************************************************************************************************************///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtCreate()
{
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	UpdateData(TRUE);
	enumErrorCode=CImportDll::GetInstance()->Import_Create();
	if (Error_Succeed==enumErrorCode)
	{
		m_csResults.Format("create successfully...");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("create fail...");
		UpdateData(FALSE);
	}
}


void CIDTRYM9000mimiTestingDlg::OnBnClickedBtRelease()
{
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	enumErrorCode=CImportDll::GetInstance()->Import_Release();
	if (Error_Succeed==enumErrorCode)
	{
		m_csResults.Format("release successfully...");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("release fail...");
		UpdateData(FALSE);
	}
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtOpen()
{
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	UpdateData(TRUE);
	DWORD dwPort=m_nPort;
	enumErrorCode=CImportDll::GetInstance()->m_lpID->DevOpen(dwPort);
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("ID Card Reader Open successfully...");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("ID Card Reader Open fail...");
		UpdateData(FALSE);
	}
	
	/*enumErrorCode=CImportDll::GetInstance()->Import_Create();
	if (enumErrorCode==Error_Succeed)
	{
		UpdateData(TRUE);
		DWORD dwPort=m_nPort;
		enumErrorCode=CImportDll::GetInstance()->m_lpID->DevOpen(dwPort);
		if (enumErrorCode==Error_Succeed)
		{
			m_csResults.Format("ID Card Reader Open successfully...");
			UpdateData(FALSE);
		}
		else
		{
			m_csResults.Format("ID Card Reader Open fail...");
			UpdateData(FALSE);
		}
	}*/
	
	
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtRfturnon()
{
	bool bOn=true;
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	enumErrorCode=CImportDll::GetInstance()->m_lpID->IDCerRFControl(bOn);
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("RF on...");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("fail to open RF...");
		UpdateData(FALSE);
	}
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtRftrunoff()
{
	bool bOn=false;
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	enumErrorCode=CImportDll::GetInstance()->m_lpID->IDCerRFControl(bOn);
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("RF off...");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("fail to close RF...");
		UpdateData(FALSE);
	}
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtAuthentication()
{
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	enumErrorCode=CImportDll::GetInstance()->m_lpID->IDCerAuthenticate();
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("ID authenticated...");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("NO ID CARD...");
		UpdateData(FALSE);
	}
	
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtGetdata()
{
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	IDCerInfo info;
	enumErrorCode=CImportDll::GetInstance()->m_lpID->IDCerGetData(info);
	if (enumErrorCode==Error_Succeed)
	{
		//m_csResults.Format("name: %s;sex: %s;nation: %s;birthday: %s;address: %s;IDNo: %s;department: %s;StartDate: %s;EndDate: %s;PhotoPath: %s",info.name.data,info.nation.data,info.birthday.data,info.address.data,info.idno.data,info.department.data,info.startDate.data,info.endDate.data);
		m_csResults.Format("name: %s\r\nsex: %s\r\nnation: -%s\r\nbirthday: %s\r\naddress: %s\r\nNo: %s\r\nDepartment: %s\r\nStartDate: %s\r\nEndDate: %s\r\nPhoto: %s",info.name.data,info.sex.data,info.nation.data,info.birthday.data,info.address.data,info.idno.data,info.department.data,info.startDate.data,info.endDate.data,info.photoPath.data);
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("cannot get ID info...");
		UpdateData(FALSE);
	}
}

void CIDTRYM9000mimiTestingDlg::OnBnClickedBtGetdevcategory()
{
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	DevCategoryInfo devinfo;
	enumErrorCode=CImportDll::GetInstance()->m_lpID->GetDevCategory(devinfo);
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("GetDevCategory successfully");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("GetDevCategory fail");
		UpdateData(FALSE);
	}
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtReset()
{
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	enumErrorCode=CImportDll::GetInstance()->m_lpID->Reset();
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("Reset successfully");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("Reset fail");
		UpdateData(FALSE);
	}
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtDevclose()
{
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	enumErrorCode=CImportDll::GetInstance()->m_lpID->DevClose();
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("DevClose successfully");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("DevClose fail");
		UpdateData(FALSE);
	}
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtGetlasterr()
{
	ErrorCodeEnum enumErrorCode=Error_Succeed;
	DevErrorInfo devLastError;
	enumErrorCode=CImportDll::GetInstance()->m_lpID->GetLastErr(devLastError);
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("GetLastErr successfully");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("GetLastErr fail");
		UpdateData(FALSE);
	}
}




//*****************************************************************************************************************///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CIDTRYM9000mimiTestingDlg::OnBnClickedBtCrtopen()
{
	/*ErrorCodeEnum enumErrorCode=Error_Succeed;

	enumErrorCode=CImportDll::GetInstance()->Import_Create(4);
	if (enumErrorCode==Error_Succeed)
	{
		enumErrorCode=CImportDll::GetInstance()->m_lpCardSwiper->DevOpen(0,0);
		if (enumErrorCode==Error_Succeed)
		{
			m_csResults.Format("CRT Open successfully...");
			UpdateData(FALSE);
		}
		else
		{
			m_csResults.Format("CRT Open fail...");
			UpdateData(FALSE);
		}
	}*/
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtCrtGetstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	/*ErrorCodeEnum enumErrorCode=Error_Succeed;
	m_csResults="";
	UpdateData(FALSE);
	CardSwiperStatus eStatus;
	enumErrorCode=CImportDll::GetInstance()->m_lpCardSwiper->GetDevStatus(eStatus);
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("状态为（0-无卡，1-有卡,4-IC卡，6-RF卡）：%d",eStatus.eMedia);
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("get status fail...");
		UpdateData(FALSE);
	}*/
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtActiveic()
{
	//// TODO: 在此添加控件通知处理程序代码
	//ErrorCodeEnum enumErrorCode=Error_Succeed;
	//enumErrorCode=CImportDll::GetInstance()->m_lpCardSwiper->ActiveICCard();
	//if (enumErrorCode==Error_Succeed)
	//{
	//	m_csResults.Format("上电成功");
	//	UpdateData(FALSE);
	//}
	//else
	//{
	//	m_csResults.Format("上电失败");
	//	UpdateData(FALSE);
	//}
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtDeactivateic()
{
	/*ErrorCodeEnum enumErrorCode=Error_Succeed;
	enumErrorCode=CImportDll::GetInstance()->m_lpCardSwiper->DeactivateICCard();
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("下电成功");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("下电失败");
		UpdateData(FALSE);
	}*/
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtActivecontactlessic()
{
	/*ErrorCodeEnum enumErrorCode=Error_Succeed;
	char szOutCharacter;
	enumErrorCode=CImportDll::GetInstance()->m_lpCardSwiper->ActiveContactlessICCard('A','B','M',szOutCharacter);
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("上电成功, 上电卡类型：%c",szOutCharacter);
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("上电失败");
		UpdateData(FALSE);
	}*/
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtDeactivatecontactlessic()
{
	/*ErrorCodeEnum enumErrorCode=Error_Succeed;
	enumErrorCode=CImportDll::GetInstance()->m_lpCardSwiper->DeactContactlessICCard();
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("下电成功");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("下电失败");
		UpdateData(FALSE);
	}*/
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtWarmreset()
{
	/*ErrorCodeEnum enumErrorCode=Error_Succeed;
	char szOutCharacter;
	enumErrorCode=CImportDll::GetInstance()->m_lpCardSwiper->WarmReset();
	if (enumErrorCode==Error_Succeed)
	{
		m_csResults.Format("warm reset successfully");
		UpdateData(FALSE);
	}
	else
	{
		m_csResults.Format("warm reset fail");
		UpdateData(FALSE);
	}*/
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtIccommand()
{
	//ErrorCodeEnum enumErrorCode=Error_Succeed;
	//CmdInfo sendBuf;
	//sendBuf.dwSize=5;
	//PBYTE byteSend=new BYTE[5];
	//memset(byteSend,0,sizeof(BYTE)*5);//00B2010C00
	//byteSend[0]=0x00;
	//byteSend[1]=0xB2;
	//byteSend[2]=0x01;
	//byteSend[3]=0x0C;
	//byteSend[4]=0x00;
	//memcpy(sendBuf.data,byteSend,sizeof(BYTE)*sendBuf.dwSize);
	//CmdInfo revcBuf;
	//enumErrorCode=CImportDll::GetInstance()->m_lpCardSwiper->ICCommand(sendBuf,revcBuf);
	//if (enumErrorCode==Error_Succeed)
	//{
	//	m_csResults.Format("command successfully");
	//	UpdateData(FALSE);
	//}
	//else
	//{
	//	m_csResults.Format("command fail");
	//	UpdateData(FALSE);
	//}
}
void CIDTRYM9000mimiTestingDlg::OnBnClickedBtRfcommand()
{
	// TODO: 在此添加控件通知处理程序代码
}






