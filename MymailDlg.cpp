
// MymailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Mymail.h"
#include "MymailDlg.h"
#include "afxdialogex.h"
#include "Download.h"
#include "sql.h"
#include "parse.h"

struct Myparam
{
	CString& m_curUrl;	
	long* m_newUrl;
	long* m_newMail;

};

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


// CMymailDlg 对话框




CMymailDlg::CMymailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMymailDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iniUrl = _T("休息中");
	m_curUrl = _T("");
	m_newUrl = 0;
	m_newMail = 0;
	state=0;
	//  m_totalMail = 0;
	m_totalMail = _T("");
	m_iniUrl = _T("");
}

void CMymailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT2, m_iniUrl);
	//  DDV_MaxChars(pDX, m_iniUrl, 256);
	DDX_Text(pDX, IDC_EDIT3, m_curUrl);
	DDV_MaxChars(pDX, m_curUrl, 1204);
	//  DDX_Text(pDX, IDC_EDIT4, m_newMail);
	//  DDX_Control(pDX, IDC_EDIT4, m_newUrl);
	DDX_Text(pDX, IDC_EDIT4, m_newUrl);
	DDV_MinMaxLong(pDX, m_newUrl, 0, 1024000000);
	DDX_Text(pDX, IDC_EDIT5, m_newMail);
	DDV_MinMaxLong(pDX, m_newMail, 0, 1024000000);
	//  DDX_Text(pDX, IDC_EDIT6, m_totalMail);
	//  DDV_MinMaxLong(pDX, m_totalMail, 0, 1024000000);
	DDX_Text(pDX, IDC_EDIT6, m_totalMail);
	DDV_MaxChars(pDX, m_totalMail, 1024);
	DDX_Text(pDX, IDC_EDIT2, m_iniUrl);
	DDV_MaxChars(pDX, m_iniUrl, 1024);
}

BEGIN_MESSAGE_MAP(CMymailDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMymailDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMymailDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &CMymailDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON4, &CMymailDlg::OnBnClickedButton4)
//	ON_WM_TIMER()
//ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTON5, &CMymailDlg::OnBnClickedButton5)
ON_BN_CLICKED(IDC_BUTTON3, &CMymailDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMymailDlg 消息处理程序

BOOL CMymailDlg::OnInitDialog()
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
	usesql sql;
	m_totalMail=CString(sql.get_total_mail().data()); //总邮件数目
	ini_email=atoi(sql.get_total_mail().data()); //为计算新增邮件数目设置
	m_curUrl=CString(sql.get_total_url().data());
	m_newUrl=atoi(sql.get_unscrached_url().data());
	UpdateData(false); //更新到屏幕
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMymailDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMymailDlg::OnPaint()
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
HCURSOR CMymailDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

unsigned long __stdcall ThreadProc(void* pAny)
{
	Myparam* p=(Myparam*)pAny; //获得参数
	Download d;
	parse pa;
	usesql sql;
	string curUrl=sql.get_next_url();

	
	while(curUrl!="")//还有没有抓取结束的网址
	{
		sql.update_a_url(curUrl);//将本curUrl标志为已经抓取过了
		try
		{
			if(curUrl.length()<6 || curUrl.length()>45)
			{
				curUrl=sql.get_next_url();
				continue;
			}
			d.SetUrl(curUrl.data());		
		}
		catch(CException* e)
		{
			curUrl=sql.get_next_url();
			continue;
		}
		try
		{
			pa.set_url(curUrl);	
		}
		catch(CException* e)
		{
			curUrl=sql.get_next_url();
			continue;
		}
		CString s("");
		//获得网页数据
		try
		{
			s=d.Action();
			//如果S太短，直接pass
			if(s.GetLength()<100)
			{
				curUrl=sql.get_next_url();
				continue;
			}
		}
		catch(CException* e)		
		{
			curUrl=sql.get_next_url();
			continue;
		}
		//传入网页数据
		try
		{
			pa.set_file(string((char*)s.GetString()));//设置文件
		}
		catch(CException* e)		
		{
			curUrl=sql.get_next_url();
			continue;
		}
		//解析网页数据
		try
		{			
			pa.parse_email();//得到email文件,并且新增计数			
			pa.parse_url();//得到新增的URL地址		
		}
		catch(CException* e)
		{
			curUrl=sql.get_next_url();
			continue;
		}
		curUrl=sql.get_next_url();
	}
	return 0;
}
void CMymailDlg::OnBnClickedOk()
{		
}

//CMymailDlg* my;
void CALLBACK CMymailDlg::TimerProc(HWND, UINT, UINT, DWORD )
{	
	//my->UpdateData(false);
}

//需要用的线程库
#define BOOST_ALL_NO_LIB
#include <boost\thread.hpp>
//windows下需要包涵的源文件
namespace boost  {void tss_cleanup_implemented(void){}}
#include <libs\thread\src\win32\thread.cpp>
#include <libs\thread\src\win32\tss_dll.cpp>
#include <libs\thread\src\win32\tss_pe.cpp>
//#include <libs\system\error_code.cpp>
#include <libs\system\src\error_code.cpp>
#include <libs\chrono\src\chrono.cpp>
#include <libs\chrono\src\process_cpu_clocks.cpp>
#include <libs\chrono\src\thread_clock.cpp>
#include <libs\timer\src\auto_timers_construction.cpp>
#include <libs\timer\src\cpu_timer.cpp>



void CMymailDlg::OnBnClickedButton1()
{
	 //单线程方式
	/*
	if(state==1) return; //已经在运行了
	DWORD Threadid;
	pThread=CreateThread(NULL,0,ThreadProc,(void*)(0),0,&Threadid);
	m_iniUrl=CString("工作中");
	UpdateData(false);	
	state=1;
	*/
	
	//改写成多线程的方式
	if(state==1) return; //已经在运行了

	//开五个线程同时进行工作
	using namespace boost;
	boost::thread f1(ThreadProc,(void*)0);//临时thread对象进行工作
	boost::thread f2(ThreadProc,(void*)0);//临时thread对象进行工作
	boost::thread f3(ThreadProc,(void*)0);//临时thread对象进行工作
	boost::thread f4(ThreadProc,(void*)0);//临时thread对象进行工作
	boost::thread f5(ThreadProc,(void*)0);//临时thread对象进行工作
	f1.detach();f2.detach();
	f3.detach();f4.detach();
	f5.detach();
	//thread(ThreadProc);//临时thread对象进行工作
	//thread(ThreadProc);//临时thread对象进行工作
	//thread(ThreadProc);//临时thread对象进行工作
	//thread(ThreadProc);//临时thread对象进行工作
	//thread(ThreadProc);//临时thread对象进行工作
	//this_thread::sleep_for(chrono::seconds(2));
	//
	state=1;
}
void CMymailDlg::OnEnChangeEdit2()
{
}


void CMymailDlg::OnBnClickedButton4()
{
	// TODO: 更新数据库中邮件地址数目
	usesql sql;
	m_totalMail=CString(sql.get_total_mail().data());
	m_curUrl=CString(sql.get_total_url().data());
	m_newUrl=atoi(sql.get_unscrached_url().data());
	m_newMail=atoi(sql.get_total_mail().data())-ini_email;
	UpdateData(false);
}


void CMymailDlg::OnBnClickedButton5()
{
	/*
	if(state==0)return;
	if(state==1) { SuspendThread(pThread); state=2;m_iniUrl=CString("被挂起");}
	if(state==2) { ResumeThread(pThread);state=1;m_iniUrl=CString("工作中");}
	UpdateData(false);	
	*/

}


void CMymailDlg::OnBnClickedButton3()
{
	//终结线程
	DWORD Exitcode=0;
	TerminateThread(pThread,Exitcode);
	state=0;
	m_iniUrl=CString("休息中");
	UpdateData(false);	

}
