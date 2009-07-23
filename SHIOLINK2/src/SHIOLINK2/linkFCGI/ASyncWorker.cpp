#include "StdAfx.h"
#include "ASyncWorker.h"

CASyncWorker::CASyncWorker(void)
{
	ZeroMemory( &ol, sizeof(ol) );
}

CASyncWorker::~CASyncWorker(void)
{
}


/* ----------------------------------------------------------------------------
 * ���[�J������
 */
int CASyncWorker::InitWorker(DWORD numberOfThread)
{
	g_hIOCP = CreateIoCompletionPort (
		INVALID_HANDLE_VALUE,
		NULL,
		NULL,
		numberOfThread - 2);

	if ( !g_hIOCP ) {
		ATLTRACE2(_T("[CASyncWorker::InitWorker] CreateIoCompletionPort failed.\n"));
		return FALSE;
	}

	for ( DWORD i=0; i < numberOfThread; i++ ) {
		UINT uThreadId;
		HANDLE hThread = (HANDLE) _beginthreadex(
			NULL, 
			NULL, 
			WorkerThreadFunc, 
			(void*) NULL, 
			NULL, 
			&uThreadId);

		if ( !hThread ) {
			ATLTRACE2(_T("[CASyncWorker::InitWorker] Failed to begin a new thread.\n"));
			if(gThreads.GetSize() > 0) EndWorker(30000);
			return FALSE;
		}
		gThreads.Add(hThread);
	}

	return TRUE;
}


/* ----------------------------------------------------------------------------
 * ���[�J�I��
 */
int CASyncWorker::EndWorker(DWORD timeout)
{
	for(int i=0; i<gThreads.GetSize(); i++){
		LPOVERLAPPED pOl = (LPOVERLAPPED) malloc ( sizeof (OVERLAPPED) );
		PostQueuedCompletionStatus ( g_hIOCP, 0, COMP_KEY_EXIT, pOl);
	}
	WaitForMultipleObjects ( gThreads.GetSize(), gThreads.GetData() , TRUE, timeout );
	gThreads.RemoveAll();
	return TRUE;
}


/* ----------------------------------------------------------------------------
 * ���[�J�X���b�h�{��
 */
unsigned int __stdcall CASyncWorker::WorkerThreadFunc(PVOID pv)
{
	LPOVERLAPPED pol = NULL;
	DWORD cbNumberOfBytesTransferred = 0;
	ULONG uCompletionKey;
	BOOL bRet;

	while(1){
		// [�C�x���g��M�ҋ@����]
		bRet = GetQueuedCompletionStatus (
			g_hIOCP, 
			&cbNumberOfBytesTransferred, 
			&uCompletionKey, 
			&pol, 
			INFINITE );
		
		if ( !bRet ) {
			// Error!
			ATLTRACE2(_T("[CASyncWorker::WorkerThreadFunc] Failed GetQueuedCompletionStatusd.\n"));
			break;
		}

		// [�X���b�h�̏I������M]
		if ( COMP_KEY_EXIT == uCompletionKey ) {
			free ( pol );
			break;
		}

		// [�N���X�|�C���^��pol���擾���Ċ����������Ăяo��]
		CASyncWorker* pWorker = (CASyncWorker*) CONTAINING_RECORD(pol, CASyncWorker, ol);
		pWorker->Complete(uCompletionKey, cbNumberOfBytesTransferred);
	}

	return 0;
}

// EOF