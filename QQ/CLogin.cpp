// CLogin.cpp: 实现文件
//

#include "stdafx.h"
#include "QQ.h"
#include "CLogin.h"
#include "afxdialogex.h"


#define WM_SYSTEMTRAY WM_USER+1


// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QQ_LOGIN, pParent)
{
	//读取文件账户
	CFile pFile;
	char usr[6] = {0};
	char pas[6] = {0};
	if (pFile.Open(L".\\USER.ME", CFile::modeRead))
	{
		pFile.Read(usr, 1);
		pFile.Seek(4, CFile::begin);
		pFile.Read(pas, 1);

		userName = usr;
		passWord = pas;
	}
	m_BtnMini = NULL;
	m_BtnClose = NULL;

	
	
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMBO1, userName);
	DDX_Text(pDX, IDC_EDIT1, passWord);
	DDX_Control(pDX, IDC_CHECK1, m_CheckpassWord);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_CLOSE, &CLogin::OnBnClickedClose)
	ON_BN_CLICKED(IDC_MINISIZE,&CLogin::OnBnClickedMinisize)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CLogin 消息处理程序


LRESULT CLogin::OnNcHitTest(CPoint point)
{
	//鼠标移动窗口
	
	

	UINT nHitTest = CDialogEx::OnNcHitTest(point);

	if (nHitTest == HTCLIENT)
	{
		nHitTest = HTCAPTION;
	}

	return nHitTest;
}


void CLogin::OnBnClickedClose()
{
	//关闭按钮
	::Shell_NotifyIcon(NIM_DELETE, &m_nid);//删除系统 托盘
	SendMessage(WM_CLOSE, 0, 0);
}


void CLogin::OnBnClickedMinisize()
{
	// 最小化按钮
	ShowWindow(SW_MINIMIZE);
}


BOOL CLogin::OnEraseBkgnd(CDC* pDC)
{
	//绘图客户区
	CRect rect,bkRect,clientRect;
	GetClientRect(&rect);
	bkRect = clientRect = rect;

	bkRect.bottom = rect.Height() / 2;
	clientRect.top = rect.Height() / 2;

	//填充一个矩形框
	pDC->FillSolidRect(&bkRect,RGB(61,79,174));//上半部分背景区域
	//绘制多边形
	CPen p(PS_INSIDEFRAME,1,RGB(62,80,178));
	CBrush B1(RGB(52, 72, 161));
	CBrush B2(RGB(7, 118, 209));

	POINT pt1[3] = { CPoint(0,0),CPoint(rect.Width() / 4,0),CPoint(0,rect.Width() / 8 * 3) };
	POINT pt2[3] = { CPoint(0,0),CPoint(0,rect.Height() / 2),CPoint(174,167/*rect.Width() / 3,rect.Height() / 2*/) };
	POINT pt3[3] = { CPoint(rect.Width(),rect.Height() / 2),CPoint(rect.Width(),rect.Height() / 8),CPoint(rect.Width() / 3 * 2,rect.Height() / 2) };

	pDC->SelectObject(&p);
	pDC->SelectObject(&B1);
	pDC->Polygon(pt1,sizeof(pt1)/sizeof(pt1[0]));

	pDC->SelectObject(&B2);
	pDC->Polygon(pt2, sizeof(pt2) / sizeof(pt2[0]));

	pDC->SelectObject(&B1);
	pDC->Polygon(pt3, sizeof(pt3) / sizeof(pt3[0]));


	pDC->FillSolidRect(&clientRect, RGB(235, 242, 249));//下半部分颜色


	//添加TIM的logo
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_TIMLOGO);

	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	memdc.SelectObject(&bmp);

	//获取图片宽高
	BITMAP logBmp;
	bmp.GetBitmap(&logBmp);

	pDC->BitBlt(134, 48, logBmp.bmWidth, logBmp.bmHeight, &memdc, 0, 0, SRCCOPY);

	//CImage head;
	//head.Load(L"./672865539.png");
	//HBITMAP hBitmap = head.Detach();
	////获取位图信息
	//GetObject(head, sizeof logBmp, &logBmp);
	//CDC memdcH;
	//memdcH.CreateCompatibleDC(pDC);
	//memdcH.SelectObject(head);
	//pDC->BitBlt(23, 183, logBmp.bmWidth, logBmp.bmHeight, &memdcH, 0, 0, SRCCOPY);


	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


BOOL CLogin::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	::SetWindowPos(this->GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);//使程序不在任务栏显示


	//使程序托盘显示
	m_nid.cbSize = sizeof NOTIFYICONDATA;
	m_nid.hWnd = m_hWnd;
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_nid.uCallbackMessage = WM_SYSTEMTRAY;

	m_hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);//获取图标
	m_nid.hIcon = m_hIcon;//设置托盘图标
	wcscpy_s(m_nid.szTip, L"TIM");
	::Shell_NotifyIcon(NIM_ADD, &m_nid);


	//改变Combox控件高度
	CComboBox* pCtl = (CComboBox*)GetDlgItem(IDC_COMBO1);
	pCtl->SendMessage(CB_SETITEMHEIGHT, (WPARAM)-1, (LPARAM)20);//改变控件本身的高度
	pCtl->SetItemHeight(0, 11);//改变下拉列表每个Item的高度;

	m_CheckpassWord.SetCheck(1);//设置勾选框勾选状态


	//设置按钮
	m_BtnMini = new MyButton;
	m_BtnMini->Create(L"-", WS_VISIBLE, CRect(375, 1, 405, 28), this, IDC_MINISIZE);
	m_BtnMini->SetButtonStyle();
	m_BtnMini->SetFaceColor(RGB(60, 78, 173));
	m_BtnMini->SetTextColor(RGB(157, 166, 214));
	m_BtnMini->SetTextHotColor(RGB(255, 255, 255));


	m_BtnClose = new MyButton;
	m_BtnClose->Create(L"X", WS_VISIBLE, CRect(405, 1, 445, 28), this, IDC_CLOSE);
	m_BtnClose->SetFaceColor(RGB(60, 78, 173));
	m_BtnClose->SetTextColor(RGB(157, 166, 214));
	m_BtnClose->SetTextHotColor(RGB(255, 255, 255));
	m_BtnClose->SetButtonStyle();
	


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


LRESULT CLogin::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 设置鼠标点击托盘弹出程序

	switch (message)
	{
	case WM_SYSTEMTRAY://自定义消息
		if (lParam == WM_LBUTTONDBLCLK)
		{
			ShowWindow(SW_SHOWNORMAL);
			::SetWindowPos(this->GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		}
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CLogin::OnBnClickedOk()
{
	// 登陆按钮
	
	UpdateData(TRUE);

	if (userName.IsEmpty() || passWord.IsEmpty())
	{
		return;
	}
	else if (userName == "1"&&passWord == "1")
	{
		EndDialog(IDOK);
	}
	else
	{
		MessageBox(L"账号或密码错误");
		SetDlgItemText(IDC_EDIT1, L"");
		return;
	}
	
	if (m_CheckpassWord.GetCheck())
	{
		CFile pFile;
		pFile.Open(L".\\USER.ME", CFile::modeCreate | CFile::modeReadWrite);
		pFile.Write(userName, sizeof userName);
		pFile.Write(passWord, sizeof userName);
	}
	

	::Shell_NotifyIcon(NIM_DELETE, &m_nid);//删除登陆框系统托盘

	CDialogEx::OnOK();
}




void CLogin::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType==SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
	}
}

