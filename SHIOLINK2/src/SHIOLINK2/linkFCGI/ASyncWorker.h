#pragma once

class CASyncWorker
{
private:
	static const int COMP_KEY_EXIT = 1;
	static HANDLE g_hIOCP;
	static CSimpleArray<HANDLE> gThreads;

public:
	// OVERLAPPEDç\ë¢ëÃ
	OVERLAPPED	ol;

public:
	CASyncWorker(void);
	virtual ~CASyncWorker(void);
	virtual void Complete(ULONG completionKey, DWORD numberOfBytesTransferred) = 0;

	static int InitWorker(DWORD numberOfThread);
	static int EndWorker(DWORD timeout);
	static unsigned int __stdcall WorkerThreadFunc(PVOID pv);
};


// EOF