// ----------------------------------------------------------------------------
// 別プロセス通信SHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
// パイプ通信プロセス管理
#include "StdAfx.h"
#include "PipeProcess.h"

/* ----------------------------------------------------------------------------
 * デストラクタ
 */
CPipeProcess::~CPipeProcess(void)
{
}

/* ----------------------------------------------------------------------------
 * コンストラクタ
 */
CPipeProcess::CPipeProcess(LPCTSTR str_commandline, LPCTSTR str_wrkdir)
{
	ATLTRACE2(_T("[CPipeProcess::CPipeProcess]start.\n"));
	ATLTRACE2(_T("[CPipeProcess::CPipeProcess]  cmd=[%s]\n") ,str_commandline);
	ATLTRACE2(_T("[CPipeProcess::CPipeProcess]  wrkdir=[%s]\n"),str_wrkdir);


	//
	ATLTRACE2(_T("[CPipeProcess::CPipeProcess]end.\n"));
}

/* ----------------------------------------------------------------------------
 * Getter
 */
const CString CPipeProcess::GetReqName() const { return mPipe.GetReqName(); }
const CString CPipeProcess::GetResName() const { return mPipe.GetResName(); }

/* ----------------------------------------------------------------------------
 * Pipeアクセス
 */
void CPipeProcess::Write(LPCSTR buf, DWORD length) { mPipe.Write(buf, length); }
void CPipeProcess::Read (LPSTR  buf, DWORD length) { mPipe.Read (buf, length); }

void CPipeProcess::Write(const CStringA& text)          { mPipe.Write(text); }
void CPipeProcess::WriteNetString(const CharArray& buf) { mPipe.WriteNetString(buf); }
void CPipeProcess::WriteNetString(const CStringA& text) { mPipe.WriteNetString(text); }

bool CPipeProcess::ReadNetString(CharArray& buf, LPSTR& pStart, int& length) { return mPipe.ReadNetString(buf, pStart, length); }
bool CPipeProcess::ReadNetString(CharArray& buf, CStringA& text)             { return mPipe.ReadNetString(buf, text); }

// ----------------------------------------------------------------------------
// EOF