#pragma once

// ClientSocket 命令目标

class CserverDlg;
class ClientSocket : public CSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();
	virtual void OnReceive(int nErrorCode);
	CserverDlg *m_pDlg;
};


