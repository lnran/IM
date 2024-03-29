// myClientSock.cpp: 实现文件
//

#include "stdafx.h"
#include "QQ.h"
#include "myClientSock.h"



// myClientSock

myClientSock::myClientSock()
{
}

myClientSock::~myClientSock()
{
}


// myClientSock 成员函数


void myClientSock::OnReceive(int nErrorCode)
{
	// 接收消息

	char buff[1024] = { 0 };
	this->Receive(buff,1024, 0);
	

	int n = MultiByteToWideChar(CP_UTF8, NULL, buff, 1024, NULL, NULL);

	wchar_t *pStr = new wchar_t[n + 1];
	memset(pStr, 0, sizeof(wchar_t)*(n + 1));

	MultiByteToWideChar(CP_UTF8, NULL, buff, 1024, pStr, n);

	pStr[n] = '\0';


	CEdit *pEdit = (CEdit *)(AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_RECIVE));
	pEdit->SetSel(-1);
	pEdit->ReplaceSel(L"\r\n");
	pEdit->ReplaceSel((LPCTSTR)pStr);
	pEdit->ReplaceSel(L"\r\n");

	delete[]pStr;



	CSocket::OnReceive(nErrorCode);
}
