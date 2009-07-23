#pragma once

class CASyncWorker
{
private:
	static const int COMP_KEY_EXIT = 1;
	static CHandle g_hIOCP;
	static CSimpleArray<HANDLE> gThreads;

public:
	// OVERLAPPED構造体
	OVERLAPPED	ol;

private:
	// IO完了ポート
	CHandle m_hIOCP;

public:
	CASyncWorker(void);
	virtual ~CASyncWorker(void);
	int WaitComplete(int timeout);

protected:
	int Attach(HANDLE h);
	virtual void Complete(ULONG completionKey, DWORD numberOfBytesTransferred) = 0;

public:
	static int InitWorker(DWORD numberOfThread);
	static int EndWorker(DWORD timeout);

private:
	static unsigned int __stdcall WorkerThreadFunc(PVOID pv);
};


// EOF