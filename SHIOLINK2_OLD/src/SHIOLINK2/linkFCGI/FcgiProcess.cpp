#include "StdAfx.h"
#include "FcgiProcess.h"

static void DupPipe(HANDLE hProc , HANDLE srcPipe ,HANDLE &dist)
{
   DuplicateHandle(hProc, srcPipe, hProc, &dist, 0, TRUE, DUPLICATE_SAME_ACCESS);
   CloseHandle(srcPipe);
}

CFcgiProcess::CFcgiProcess(LPCTSTR str_commandline, LPCTSTR str_wrkdir)
{
	ATLTRACE2(_T("[CFcgiProcess::CFcgiProcess]start.\n"));
	ATLTRACE2(_T("[CFcgiProcess::CFcgiProcess]  cmd=[%s]\n") ,str_commandline);
	ATLTRACE2(_T("[CFcgiProcess::CFcgiProcess]  wrkdir=[%s]\n"),str_wrkdir);
	CString commandline(str_commandline);
	CPath wrkdir(str_wrkdir);
	if(! wrkdir.IsDirectory()) AtlThrow( FAILED(ERROR_PATH_NOT_FOUND) );

	// ロジック参考
	// http://www.k5.dion.ne.jp/~minyu/program/windows/pipe.html

	// SECURITY_ATTRIBUTES の設定(パイプを作るのに必要)
	SECURITY_ATTRIBUTES secAtt;
	secAtt.nLength              = sizeof(SECURITY_ATTRIBUTES);
	secAtt.lpSecurityDescriptor = NULL;
	secAtt.bInheritHandle       = FALSE;

	// パイプの作成
	HANDLE hPipe[3][2];
	const int R=0, W=1, STDIN=0, STDOUT=1, STDERR=2;
	CHandle hSTDIN, hSTDOUT, hSTDERR;
	CreatePipe(	&hPipe[STDIN][R],	// 読み込み用ハンドル
				&hPipe[STDIN][W],	// 書き込み用ハンドル
				&secAtt,			// ?
				0);					// バッファのサイズ
	CreatePipe(	&hPipe[STDOUT][R],	// 読み込み用ハンドル
				&hPipe[STDOUT][W],	// 書き込み用ハンドル
				&secAtt,			// ?
				0);					// バッファのサイズ
	CreatePipe(	&hPipe[STDERR][R],	// 読み込み用ハンドル
				&hPipe[STDERR][W],	// 書き込み用ハンドル
				&secAtt,			// ?
				0);					// バッファのサイズ
	mWritePipe.Attach(hPipe[STDIN][W]);
	mReadPipe.Attach(hPipe[STDOUT][R]);
	mErrPipe.Attach(hPipe[STDERR][R]);

	// 子プロセスの起動時の情報
    // これを設定しないと, コンソール系プロセスでは,
    // コンソールが立ち上がる。
	memset(&mStartInfo,0,sizeof(STARTUPINFO));
	mStartInfo.cb = sizeof(STARTUPINFO); // 構造体の大きさ;
	mStartInfo.dwFlags = STARTF_USEFILLATTRIBUTE |
                         STARTF_USECOUNTCHARS |
                         STARTF_USESTDHANDLES | /*ハンドルを継承するメンバ*/
                         STARTF_USESHOWWINDOW;  // どのメンバが有効か
	mStartInfo.wShowWindow = SW_HIDE;

	// pipeの複製
	HANDLE hProc = GetCurrentProcess();
	DupPipe(hProc ,hPipe[STDIN][R]  ,mStartInfo.hStdInput);
	DupPipe(hProc ,hPipe[STDOUT][W] ,mStartInfo.hStdOutput);
	DupPipe(hProc ,hPipe[STDERR][W] ,mStartInfo.hStdError);

	// プロセスの起動
	if(CreateProcess(NULL ,commandline.GetBuffer() ,NULL ,NULL ,TRUE
		,0 ,NULL ,NULL	,&mStartInfo ,&mProcessInfo) != TRUE)
	{
		AtlThrow(FAILED(CO_E_CREATEPROCESS_FAILURE));
	}
	commandline.ReleaseBuffer();


	ATLTRACE2(_T("[CFcgiProcess::CFcgiProcess]end.\n"));
}

CFcgiProcess::~CFcgiProcess(void)
{
	ATLTRACE2(_T("[CFcgiProcess::~CFcgiProcess]start.\n"));
	// 子プロセスが終るまで停止
	if (mProcessInfo.hProcess != NULL) {
		WaitForSingleObject(mProcessInfo.hProcess,INFINITE);
		CloseHandle(mProcessInfo.hProcess);
	}


	ATLTRACE2(_T("[CFcgiProcess::~CFcgiProcess]end.\n"));
}
