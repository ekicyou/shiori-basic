#include "StdAfx.h"
#include "Pipe.h"
#include "Util.h"

CPipe::CPipe(void)
{
}

CPipe::~CPipe(void)
{
}

void CPipe::Close(void)
{
	if(mReqPipe!=NULL)	mReqPipe.Close();
	if(mResPipe!=NULL)	mResPipe.Close();
}


/* ----------------------------------------------------------------------------
 * Getter
 */
LPCTSTR CPipe::GetBaseName() const { return mBaseName; }
CString CPipe::GetReqName() const { return mBaseName + _T("req"); }
CString CPipe::GetResName() const { return mBaseName + _T("res"); }

/* ----------------------------------------------------------------------------
 * パイプの作成
 */
void CPipe::Create(void)
{
	for(int i=0; i<10; i++){
		if(TryCreate()) return;
	}
	AtlThrow(ERROR_CANNOT_MAKE);
}


static bool firstCreate = true;
bool CPipe::TryCreate(void)
{
	ATLTRACE2(_T("[%s]start.\n"), _T(__FUNCTION__));
	Close();

	// 初回ならリセット
	if(firstCreate){
		firstCreate = false;
		srand(GetTickCount());
	}

	// 名前の作成
	for(UINT uUnique=rand() ;;uUnique++){
		if(uUnique==0) continue;
		CAtlString buf;
		buf.Format(_T("\\\\.\\pipe\\sl%x"), uUnique);
		mBaseName = CPath(buf);
		break;
	}
	ATLTRACE2(_T("[%s]  baseName = [%s]\n"), _T(__FUNCTION__), GetBaseName());
	ATLTRACE2(_T("[%s]   reqName = [%s]\n"), _T(__FUNCTION__), GetReqName());
	ATLTRACE2(_T("[%s]   resName = [%s]\n"), _T(__FUNCTION__), GetResName());

	// SECURITY_ATTRIBUTES の設定(パイプを作るのに必要)
	SECURITY_ATTRIBUTES secAtt;
	secAtt.nLength              = sizeof(SECURITY_ATTRIBUTES);
	secAtt.lpSecurityDescriptor = NULL;
	secAtt.bInheritHandle       = FALSE;
	const DWORD PIPE_MODE = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;

	// req用(OUT)
	HANDLE req = ::CreateNamedPipe(
		GetReqName(),
		PIPE_ACCESS_OUTBOUND,
		PIPE_MODE,
		1, 1024, 1024, 0, &secAtt);
	if (req == INVALID_HANDLE_VALUE) return false;
	mReqPipe.Attach(req);

	// res用(IN)
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
 * 読み込み
 */
void CPipe::Read(LPBYTE buf, DWORD length)
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
 * 書き込み
 */
void CPipe::Write(const LPBYTE buf, DWORD length)
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