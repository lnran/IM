#pragma once
#include "MyButton.h"

// CLogin 对话框

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLogin();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QQ_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedClose();//响应关闭按钮
	afx_msg void OnBnClickedMinisize();//响应最小化按钮
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedOk();//响应登陆
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	NOTIFYICONDATA m_nid;//托盘显示
	HICON m_hIcon;//窗口图标句柄

	MyButton *m_BtnMini;
	MyButton *m_BtnClose;

	CString userName;
	CString passWord;
	CButton m_CheckpassWord;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
