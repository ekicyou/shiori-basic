#include "stdafx.h"
#include "Pipe.h"

TEST(PipeTest, Create)
{
	CServerPipe p;
	p.Create();
}

static CString CreateTestExePath()
{
	TCHAR buf[MAX_PATH];
	buf[0]=0;
	GetEnvironmentVariable( _T("TargetDir"), buf, MAX_PATH);
	CString path(buf);
	path += _T("TEST.exe");
	return path;
}

static CString CreateTestExePath(LPCTSTR param)
{
	CString path = CreateTestExePath();
	path += _T(" ");
	path += param;
	return path;
}

static BOOL CreateTestProcess(LPCTSTR param, LPPROCESS_INFORMATION pInfo)
{
	// 起動パラメータ
	CString path = CreateTestExePath(param);
	
	STARTUPINFO sInfo;
	memset(&sInfo,0,sizeof(sInfo));	sInfo.cb = sizeof(sInfo);

	// 起動
	BOOL rc = CreateProcess(NULL, path.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, pInfo);
	return rc;
}

TEST(PipeTest, Exec1) {
	// 起動
	PROCESS_INFORMATION pInfo;
	BOOL rc = CreateTestProcess(_T("1"), &pInfo);
	if(rc != TRUE){
		DWORD dwError = ::GetLastError();
		ATLTRACE2( _T("ShellExecuteEx --> %X\n"), dwError);
		FAIL();
	}
	CHandle hProcess(pInfo.hProcess);
	
	// プロセス終了待機処理
	ASSERT_EQ(WAIT_OBJECT_0, WaitForSingleObject(hProcess, 2000));
	DWORD exitCode = -1;
    GetExitCodeProcess(hProcess, &exitCode);
    EXPECT_EQ(0, exitCode);
}

TEST(PipeTest, Exec2) {
	// パイプの作成
	CServerPipe pipe;
	pipe.Create();

	// 引数
	CString param;
	param.Format(_T("2 %s"), pipe.GetID());

	// 起動
	PROCESS_INFORMATION pInfo;
	BOOL rc = CreateTestProcess(param, &pInfo);
	if(rc != TRUE){
		DWORD dwError = ::GetLastError();
		ATLTRACE2( _T("ShellExecuteEx --> %X\n"), dwError);
		FAIL();
	}
	CHandle hProcess(pInfo.hProcess);
	
	// プロセス終了待機処理
	ASSERT_EQ(WAIT_OBJECT_0, WaitForSingleObject(hProcess, 2000));
	DWORD exitCode = -1;
    GetExitCodeProcess(hProcess, &exitCode);
    EXPECT_EQ(0, exitCode);
}



static int runPipeServer1(int argc, char **argv)
{
	std::cout << "[runPipeServer1]********\n";
	return 0;
}

static int runPipeServer2(int argc, char **argv)
{
	std::cout << "[runPipeServer2]********\n";
	if(argc < 3) return -1;
	CString pipeID(argv[2]);
	std::cout << "[runPipeServer2]pipeID=[" << CT2CA(pipeID) << "]\n";
	return 0;
}

int runPipeServer(int argc, char **argv)
{
	CString mode(argv[1]);
	std::cout << "[runPipeServer]Running mode=[" << CT2CA(mode) << "]\n";
	if(mode == _T("1")) return runPipeServer1(argc, argv);
	if(mode == _T("2")) return runPipeServer2(argc, argv);

	int rc = 0;
	std::cout << "[runPipeServer]Done.\n\n";
	return rc;
}
