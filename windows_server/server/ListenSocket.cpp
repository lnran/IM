// ListenSocket.cpp: 实现文件
//

#include "stdafx.h"
#include "server.h"
#include "ListenSocket.h"
#include "ClientSocket.h"


// ListenSocket

ListenSocket::ListenSocket(CserverDlg *pDlg)
{
	m_pDlg = pDlg;
}


ListenSocket::~ListenSocket()
{
}


// ListenSocket 成员函数




void ListenSocket::OnAccept(int nErrorCode)
{
	//绑定
	
	//有客户端连接服务器
	ClientSocket *pSocket = new ClientSocket;
	
	Accept(*pSocket);

	//获取连接上的客户端ip和端口号
	CString strIPAddress;
	UINT nPort;
	pSocket->GetPeerName(strIPAddress, nPort);

	//显示到界面上
	

	CSocket::OnAccept(nErrorCode);
}
