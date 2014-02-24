
// MymailDlg.h : 头文件
//

#pragma once
#include "sql.h"

// CMymailDlg 对话框
class CMymailDlg : public CDialogEx
{
// 构造
public:
	CMymailDlg(CWnd* pParent = NULL);	// 标准构造函数
	
// 对话框数据
	enum { IDD = IDD_MYMAIL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit2();

	static void  CALLBACK TimerProc (HWND, UINT, UINT,   DWORD ) ;

	CString m_curUrl;
	int state;

	long m_newUrl;
	long m_newMail;
	long ini_email;
	HANDLE pThread;

	CString m_totalMail;
	afx_msg void OnBnClickedButton4();

	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	CString m_iniUrl;
};
