// ----------------------------------------------------------------------------
// 別プロセス通信SHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
// パイプ通信プロセス管理
#pragma once
#include "util.h"
#include "pipe.h"

class CPipeProcess
{
private:
	CServerPipe mPipe;
	STARTUPINFO mStartInfo;
	PROCESS_INFORMATION mProcessInfo;

public:
	CPipeProcess(LPCTSTR str_commandline, LPCTSTR str_wrkdir);
	virtual ~CPipeProcess(void);
	LPCTSTR GetPipeBaseName(void) const;
	CString GetPipeReqName(void) const;
	CString GetPipeResName(void) const;

};
