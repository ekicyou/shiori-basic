// ----------------------------------------------------------------------------
// 別プロセス通信SHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
// パイプ通信プロセス管理
#include "StdAfx.h"
#include "PipeProcess.h"
#include <stdlib.h>

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
LPCTSTR CPipeProcess::GetPipeName(){ return mPipeName; }

/* ----------------------------------------------------------------------------
 * パイプの作成
 */

void CPipeProcess::CreatePipe()
{
	ATLTRACE2(_T("[CPipeProcess::CreatePipe]start.\n"));

	// 名前の作成
	for(UINT uUnique=GenRandomNumber() ;;uUnique++){
		TCHAR buf[64];
		if(uUnique==0) continue;
		if(GetTempFileName(_T("\\\\.\\pipe\\"), _T("sl"), uUnique, buf)) continue;

		mPipeName = buf;
		break;
	}
	ATLTRACE2(_T("[CPipeProcess::CreatePipe]name = [%s]\n"),(LPCTSTR) mPipeName);

	// SECURITY_ATTRIBUTES の設定(パイプを作るのに必要)
	SECURITY_ATTRIBUTES secAtt;
	secAtt.nLength              = sizeof(SECURITY_ATTRIBUTES);
	secAtt.lpSecurityDescriptor = NULL;
	secAtt.bInheritHandle       = FALSE;




	//
	ATLTRACE2(_T("[CPipeProcess::CreatePipe]end.\n"));
}

// ----------------------------------------------------------------------------
// EOF