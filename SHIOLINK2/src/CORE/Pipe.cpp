#include "StdAfx.h"
#include "Pipe.h"
#include "Util.h"

CServerPipe::CServerPipe(void)
{
}

CServerPipe::~CServerPipe(void)
{
}

void CServerPipe::Close(void)
{
	if(mReqPipe!=NULL)	mReqPipe.Close();
	if(mResPipe!=NULL)	mResPipe.Close();
}


/* ----------------------------------------------------------------------------
 * Getter
 */
LPCTSTR CServerPipe::GetID()       const { return mID; }
LPCTSTR CServerPipe::GetBaseName() const { return mBaseName; }
CString CServerPipe::GetReqName()  const { return mBaseName + _T("req"); }
CString CServerPipe::GetResName()  const { return mBaseName + _T("res"); }

/* ----------------------------------------------------------------------------
 * �p�C�v�̍쐬
 */
void CServerPipe::Create(void)
{
	for(int i=0; i<10; i++){
		if(TryCreate()) return;
	}
	AtlThrow(ERROR_CANNOT_MAKE);
}


static bool firstCreate = true;
bool CServerPipe::TryCreate(void)
{
	ATLTRACE2(_T("[%s]start.\n"), _T(__FUNCTION__));
	Close();

	// ����Ȃ烊�Z�b�g
	if(firstCreate){
		firstCreate = false;
		srand(GetTickCount());
	}

	// ���O�̍쐬
	for(UINT uUnique=rand() ;;uUnique++){
		if(uUnique==0) continue;
		mID.Format(_T("%x"), uUnique);
		CAtlString buf;
		buf.Format(_T("\\\\.\\pipe\\sl%x"), uUnique);
		mBaseName = CPath(buf);
		break;
	}
	ATLTRACE2(_T("[%s]  baseName = [%s]\n"), _T(__FUNCTION__), GetBaseName());
	ATLTRACE2(_T("[%s]   reqName = [%s]\n"), _T(__FUNCTION__), GetReqName());
	ATLTRACE2(_T("[%s]   resName = [%s]\n"), _T(__FUNCTION__), GetResName());

	// SECURITY_ATTRIBUTES �̐ݒ�(�p�C�v�����̂ɕK�v)
	SECURITY_ATTRIBUTES secAtt;
	memset(&secAtt,0,sizeof(secAtt)); secAtt.nLength =sizeof(secAtt);
	const DWORD PIPE_MODE = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;

	// req�p(OUT)
	HANDLE req = ::CreateNamedPipe(
		GetReqName(),
		PIPE_ACCESS_OUTBOUND,
		PIPE_MODE,
		1, 1024, 1024, 0, &secAtt);
	if (req == INVALID_HANDLE_VALUE) return false;
	mReqPipe.Attach(req);

	// res�p(IN)
	HANDLE res = ::CreateNamedPipe(
		GetResName(),
		PIPE_ACCESS_INBOUND,
		PIPE_MODE,
		1, 1024, 1024, 0, &secAtt);
	if (res == INVALID_HANDLE_VALUE) return false;
	mResPipe.Attach(res);

	//
	ATLTRACE2(_T("[%s]end.\n"), _T(__FUNCTION__));
	return true;
}


/* ----------------------------------------------------------------------------
 * �ǂݍ���
 */
void CServerPipe::Read(LPBYTE buf, DWORD length)
{
	LPBYTE pos = buf;
	while(length > 0){
		DWORD resultLen;
		if(!::ReadFile(mResPipe, pos, length, &resultLen, NULL)) ::AtlThrowLastWin32();
		pos += resultLen;
		length -= resultLen;
	}
}

/* ----------------------------------------------------------------------------
 * ��������
 */
void CServerPipe::Write(const LPBYTE buf, DWORD length)
{
	LPBYTE pos = (LPBYTE)buf;
	while(length > 0){
		DWORD resultLen;
		if(!::WriteFile(mReqPipe, pos, length, &resultLen, NULL)) ::AtlThrowLastWin32();
		pos += resultLen;
		length -= resultLen;
	}
}

// EOF