#pragma once

// myClientSock 命令目标

class myClientSock : public CSocket
{
public:
	myClientSock();
	virtual ~myClientSock();
	virtual void OnReceive(int nErrorCode);
};


