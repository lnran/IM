
// QQDlg.h: 头文件
//

#pragma once
#include "myClientSock.h"
#include "MyButton.h"

// CQQDlg 对话框
class CQQDlg : public CDialogEx 
{
// 构造
public:
	CQQDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QQ_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);//最小化托盘
	afx_msg void OnClose();//响应关闭
	afx_msg void OnBnClickedSend();//响应发送
	afx_msg void OnDestroy();//响应销毁
	afx_msg void OnMinisize();//最小化
	afx_msg void OnClosew();//最大化

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);//绘制客户端界面
	afx_msg LRESULT OnNcHitTest(CPoint point);//窗口测试信息 用来鼠标移动窗口界面
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	

	NOTIFYICONDATA m_nid;//托盘图标结构
	myClientSock m_clientSock;//定义一个套接字对象

	MyButton *m_BtnMini;//最小化按钮
	MyButton *m_BtnClose;//关闭按钮
	
};
