// ClientSocket.cpp: 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "ClientSocket.h"


// ClientSocket

ClientSocket::ClientSocket()
{
	
}

ClientSocket::~ClientSocket()
{
}


// ClientSocket 成员函数


void ClientSocket::OnReceive(int nErrorCode)
{
	TCHAR str[256];
	Receive(str,256);

	CEdit *pEdit = (CEdit *)AfxGetApp()->GetMainWnd()->GetDlgItem(IDC_EDIT1);
	pEdit->SetSel(-1);
	pEdit->ReplaceSel(str);

	CSocket::OnReceive(nErrorCode);
}
