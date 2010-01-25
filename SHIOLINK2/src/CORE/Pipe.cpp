#include "StdAfx.h"
#include "Pipe.h"
#include "Util.h"

/* ----------------------------------------------------------------------------
 * [CPipe]初期化・開放
 */
CPipe::CPipe(void)
{
}

CPipe::~CPipe(void)
{
}

void CPipe::Close(void)
{
	if(mRead !=NULL) mRead .Close();
	if(mWrite!=NULL) mWrite.Close();
}

/* ----------------------------------------------------------------------------
 * [CPipe]Getter
 */
LPCTSTR CPipe::GetID()       const { return mID; }
LPCTSTR CPipe::GetBaseName() const { return mBaseName; }
CString CPipe::GetReqName()  const { return mBaseName + _T("req"); }
CString CPipe::GetResName()  const { return mBaseName + _T("res"); }

/* ----------------------------------------------------------------------------
 * [CPipe]Setter
 */
void CPipe::SetID(LPCTSTR id)
{
	mID = id;
	CString buf;
	buf.Format(_T("\\\\.\\pipe\\sl2scgi%s"), mID);
	mBaseName = CPath(buf);
}

/* ----------------------------------------------------------------------------
 * [CPipe]書き込み
 */
void CPipe::Write(const LPBYTE buf, DWORD length)
{
	LPBYTE pos = (LPBYTE)buf;
	while(length > 0){
		DWORD resultLen;
		if(!::WriteFile(mWrite, pos, length, &resultLen, NULL)) ::AtlThrowLastWin32();
		pos += resultLen;
		length -= resultLen;
	}
}

/* ----------------------------------------------------------------------------
 * [CPipe]読み込み
 */
void CPipe::Read(LPBYTE buf, DWORD length)
{
	LPBYTE pos = buf;
	while(length > 0){
		DWORD resultLen;
		if(!::ReadFile(mRead, pos, length, &resultLen, NULL)) ::AtlThrowLastWin32();
		pos += resultLen;
		length -= resultLen;
	}
}

/* ----------------------------------------------------------------------------
 * [CServerPipe]初期化・開放
 */
CServerPipe::CServerPipe(void)
	:CPipe()
{
}

CServerPipe::~CServerPipe(void)
{
}

/* ----------------------------------------------------------------------------
 * [CServerPipe]パイプの作成
 */
void CServerPipe::Create(void)
{
	for(int i=0; i<10; i++){
		if(TryCreate()){
			return;
		}
	}
	AtlThrow(ERROR_CANNOT_MAKE);
}

static bool firstCreate = true;
bool CServerPipe::TryCreate(void)
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
		CString id;
		id.Format(_T("%x"), uUnique);
		SetID(id);
		break;
	}
	ATLTRACE2(_T("[%s]  baseName = [%s]\n"), _T(__FUNCTION__), GetBaseName());
	ATLTRACE2(_T("[%s]   reqName = [%s]\n"), _T(__FUNCTION__), GetReqName());
	ATLTRACE2(_T("[%s]   resName = [%s]\n"), _T(__FUNCTION__), GetResName());

	// SECURITY_ATTRIBUTES の設定(パイプを作るのに必要)
	SECURITY_ATTRIBUTES secAtt;
	memset(&secAtt,0,sizeof(secAtt));
	secAtt.nLength =sizeof(secAtt);
	secAtt.lpSecurityDescriptor = NULL;
	secAtt.bInheritHandle       = FALSE;
	const DWORD PIPE_MODE = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;

	// req用(Write)
	HANDLE req = ::CreateNamedPipe(
		GetReqName(),
		PIPE_ACCESS_OUTBOUND,
		PIPE_MODE,
		1, 1024, 1024, 0, &secAtt);
	if (req == INVALID_HANDLE_VALUE) return false;
	mWrite.Attach(req);

	// res用(Read)
	HANDLE res = ::CreateNamedPipe(
		GetResName(),
		PIPE_ACCESS_INBOUND,
		PIPE_MODE,
		1, 1024, 1024, 0, &secAtt);
	if (res == INVALID_HANDLE_VALUE) return false;
	mRead.Attach(res);

	//
	ATLTRACE2(_T("[%s]end.\n"), _T(__FUNCTION__));
	return true;
}

/* ----------------------------------------------------------------------------
 * [CServerPipe]書き込み可能になるまで待機します。
 */
bool CServerPipe::WaitForConnection(void)
{
	BOOL rc = ::ConnectNamedPipe(mWrite, NULL);
	return rc == TRUE;
}

/* ----------------------------------------------------------------------------
 * [CClientPipe]初期化・開放
 */
CClientPipe::CClientPipe(LPCTSTR id)
	:CPipe()
{
	Create(id);
}

CClientPipe::~CClientPipe(void)
{
}

/* ----------------------------------------------------------------------------
 * [CClientPipe]パイプの作成
 */
void CClientPipe::Create(LPCTSTR id)
{
	ATLTRACE2(_T("[%s]start.\n"), _T(__FUNCTION__));
	Close();

	// 名前の作成
	SetID(id);

	// req用(Read)
	{
		CString path(GetReqName());
		HANDLE handle = ::CreateFile( path, GENERIC_READ, 0,
			NULL, OPEN_EXISTING, 0, NULL);
		if (handle == INVALID_HANDLE_VALUE) AtlThrowLastWin32();
		mRead.Attach(handle);
	}

	// res用(Write)
	{
		CString path(GetResName());
		HANDLE handle = ::CreateFile( path, GENERIC_WRITE , 0,
			NULL, OPEN_EXISTING, 0, NULL);
		if (handle == INVALID_HANDLE_VALUE) AtlThrowLastWin32();
		mWrite.Attach(handle);
	}

	//
	ATLTRACE2(_T("[%s]end.\n"), _T(__FUNCTION__));
}

// EOF