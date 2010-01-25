#include "StdAfx.h"
#include "Pipe.h"
#include "Util.h"

/* ----------------------------------------------------------------------------
 * [CPipe]�������E�J��
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
 * [CPipe]��������
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
 * [CPipe]�ǂݍ���
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
 * [CServerPipe]�������E�J��
 */
CServerPipe::CServerPipe(void)
	:CPipe()
{
}

CServerPipe::~CServerPipe(void)
{
}

/* ----------------------------------------------------------------------------
 * [CServerPipe]�p�C�v�̍쐬
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

	// ����Ȃ烊�Z�b�g
	if(firstCreate){
		firstCreate = false;
		srand(GetTickCount());
	}

	// ���O�̍쐬
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

	// SECURITY_ATTRIBUTES �̐ݒ�(�p�C�v�����̂ɕK�v)
	SECURITY_ATTRIBUTES secAtt;
	memset(&secAtt,0,sizeof(secAtt));
	secAtt.nLength =sizeof(secAtt);
	secAtt.lpSecurityDescriptor = NULL;
	secAtt.bInheritHandle       = FALSE;
	const DWORD PIPE_MODE = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT;

	// req�p(Write)
	HANDLE req = ::CreateNamedPipe(
		GetReqName(),
		PIPE_ACCESS_OUTBOUND,
		PIPE_MODE,
		1, 1024, 1024, 0, &secAtt);
	if (req == INVALID_HANDLE_VALUE) return false;
	mWrite.Attach(req);

	// res�p(Read)
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
 * [CServerPipe]�������݉\�ɂȂ�܂őҋ@���܂��B
 */
bool CServerPipe::WaitForConnection(void)
{
	BOOL rc = ::ConnectNamedPipe(mWrite, NULL);
	return rc == TRUE;
}

/* ----------------------------------------------------------------------------
 * [CClientPipe]�������E�J��
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
 * [CClientPipe]�p�C�v�̍쐬
 */
void CClientPipe::Create(LPCTSTR id)
{
	ATLTRACE2(_T("[%s]start.\n"), _T(__FUNCTION__));
	Close();

	// ���O�̍쐬
	SetID(id);

	// req�p(Read)
	{
		CString path(GetReqName());
		HANDLE handle = ::CreateFile( path, GENERIC_READ, 0,
			NULL, OPEN_EXISTING, 0, NULL);
		if (handle == INVALID_HANDLE_VALUE) AtlThrowLastWin32();
		mRead.Attach(handle);
	}

	// res�p(Write)
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