#pragma once

// ListenSocket 命令目标
class CserverDlg;
class ListenSocket : public CSocket
{
public:
	ListenSocket(CserverDlg *pDlg);
	virtual ~ListenSocket();

	CserverDlg *m_pDlg;
	virtual void OnAccept(int nErrorCode);
};


