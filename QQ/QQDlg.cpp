
// QQDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "QQ.h"
#include "QQDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_SYSTEMTRAY WM_USER+1


// CQQDlg 对话框



CQQDlg::CQQDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QQ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_BtnMini = nullptr;
	m_BtnClose = nullptr;

}

void CQQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQQDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDSEND, &CQQDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_MINISIZE,&CQQDlg::OnMinisize)
	ON_BN_CLICKED(IDC_CLOSE,&CQQDlg::OnClosew)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CQQDlg 消息处理程序

BOOL CQQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

									//设置子窗口最小化样式
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	//设置系统托盘
	m_nid.cbSize = sizeof NOTIFYICONDATA;
	m_nid.hWnd = m_hWnd;
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_nid.uCallbackMessage = WM_SYSTEMTRAY;//自定义消息
	m_nid.hIcon = m_hIcon;
	wcscpy_s(m_nid.szTip, L"TIM");
	::Shell_NotifyIcon(NIM_ADD, &m_nid);

	//初始化套接字
	if (!m_clientSock.Create())
	{
		MessageBox(L"创建套接字失败");
		DestroyWindow();
	}
	else if(!m_clientSock.Connect(L"98.142.143.169", 8888))
	{
		MessageBox(L"服务器连接失败");
		DestroyWindow();
	}



	//绘制按钮
	m_BtnMini = new MyButton;
	m_BtnMini->Create(L"-", WS_VISIBLE, CRect(720, 5, 740, 25), this, IDC_MINISIZE);
	m_BtnMini->SetButtonStyle();
	m_BtnMini->SetFaceColor(RGB(24, 144, 237));
	m_BtnMini->SetTextColor(RGB(157, 166, 214));
	m_BtnMini->SetTextHotColor(RGB(255,255,255));

	m_BtnClose = new MyButton;
	m_BtnClose->Create(L"X", WS_VISIBLE, CRect(740, 5, 760, 25), this, IDC_CLOSE);
	m_BtnClose->SetButtonStyle();
	m_BtnClose->SetFaceColor(RGB(24, 144, 237));
	m_BtnClose->SetTextColor(RGB(157, 166, 214));
	m_BtnClose->SetTextHotColor(RGB(255, 255, 255));
	


	//设置焦点
	GetDlgItem(IDC_SEND)->SetFocus();

	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQQDlg::OnPaint()
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
HCURSOR CQQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CQQDlg::OnSize(UINT nType, int cx, int cy)
{
	//获取最小化消息
	CDialogEx::OnSize(nType, cx, cy);

	if (nType==SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
	}
	
}


void CQQDlg::OnClose()
{
	
	::Shell_NotifyIcon(NIM_DELETE, &m_nid);
	m_clientSock.Close();

	CDialogEx::OnClose();
}


LRESULT CQQDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//双击托盘按钮显示程序

	switch (message)
	{
	case WM_SYSTEMTRAY:
		if (lParam == WM_LBUTTONDBLCLK)
		{
			ShowWindow(SW_SHOWNORMAL);
			::SetWindowPos(this->GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
		break;
	}


	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CQQDlg::OnMinisize()
{
	//最小化
	ShowWindow(SW_HIDE);
}

void CQQDlg::OnClosew()
{
	//销毁
	DestroyWindow();
}

void CQQDlg::OnBnClickedSend()
{
	//发送消息
	CString str;
	CString name;

	GetDlgItemTextW(IDC_SEND, str);
	GetDlgItemTextW(IDC_NAME, name);
	if (name.IsEmpty())
	{
		MessageBox(L"请输入你的昵称!");
		return;
	}
	if (str.IsEmpty())
	{
		return;
	}
	SetDlgItemText(IDC_SEND, L"");
	
	CString send = name + ":" + str;
	int LEN = WideCharToMultiByte(CP_ACP, 0, send, send.GetLength(), NULL, 0, NULL, NULL);
	char* pStr = new char[LEN + 1];
	WideCharToMultiByte(CP_ACP, 0, send, send.GetLength(), pStr, LEN, NULL, NULL);

	pStr[LEN + 1] = '\0';
	m_clientSock.Send(pStr, LEN, 0);//向服务器发送消息




	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_RECIVE);
	pEdit->SetSel(-1);
	pEdit->ReplaceSel(L"\r\n");
	pEdit->ReplaceSel(L"我：" + str + "\r\n");

	
	GetDlgItem(IDC_SEND)->SetFocus();
}


void CQQDlg::OnDestroy()
{
	::Shell_NotifyIcon(NIM_DELETE, &m_nid);
	m_clientSock.Close();

	CDialogEx::OnDestroy();

}




LRESULT CQQDlg::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CDialogEx::OnNcHitTest(point);

	if (nHitTest == HTCLIENT)
	{
		nHitTest = HTCAPTION;
	}

	return nHitTest;
}


BOOL CQQDlg::OnEraseBkgnd(CDC* pDC)
{
	//绘制客户端背景
	CRect rect, rectCaption, rectClient;
	GetClientRect(&rect);
	rectCaption = rectClient = rect;

	rectCaption.bottom = 30;
	rectClient.top = 30;


	pDC->FillSolidRect(&rectCaption, RGB(24, 144, 237));//填充标题栏颜色
	pDC->FillSolidRect(&rectClient, RGB(255, 255, 255));//填充客户区颜色



	CFont font;
	font.CreatePointFont(110, L"微软雅黑");
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&font);

	pDC->TextOutW(30, 8, L"聊天");//设置标题
	//pDC->DrawIcon(5, 5, m_hIcon);//设置图标 注：无法改变图标大小
	::DrawIconEx(pDC->GetSafeHdc(), 5, 5, m_hIcon, 18, 18, 0, NULL, DI_MASK | DI_IMAGE);//设置图标

	return TRUE;
}
