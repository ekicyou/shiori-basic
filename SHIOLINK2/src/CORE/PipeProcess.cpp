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
LPCTSTR CPipeProcess::GetPipeBaseName() const { return mPipe.GetBaseName(); }
CString CPipeProcess::GetPipeReqName()  const { return mPipe.GetReqName(); }
CString CPipeProcess::GetPipeResName()  const { return mPipe.GetResName(); }

// ----------------------------------------------------------------------------
// EOF