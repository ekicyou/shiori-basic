#include "stdafx.h"
#include "Pipe.h"

TEST(PipeTest, Create) {
	CPipe p;
	p.Create();
}

static CString CreateTestExePath() {
	TCHAR buf[MAX_PATH];
	buf[0]=0;
	GetEnvironmentVariable( _T("TargetDir"), buf, MAX_PATH);
	CString path(buf);
	path += _T("TEST.exe");
	return path;
}


TEST(PipeTest, Exec1) {
	// 起動パラメータ
	CString path = CreateTestExePath();
	path += _T(" 1");
	
	STARTUPINFO sInfo;
	memset(&sInfo,0,sizeof(sInfo));	sInfo.cb = sizeof(sInfo);
	PROCESS_INFORMATION pInfo;

	// 起動
	BOOL rc = CreateProcess(NULL, path.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
	if(rc != TRUE){
		DWORD dwError = ::GetLastError();
		ATLTRACE2( _T("ShellExecuteEx --> %X\n"), dwError);
		FAIL();
	}
	CHandle p(pInfo.hProcess);
	
	// 終了を待つ
	ASSERT_EQ(WAIT_OBJECT_0, WaitForSingleObject(p, 2000));
}


static int runPipeServer1(int argc, char **argv)
{
	std::cout << "[runPipeServer1]********\n";
	return 0;
}


int runPipeServer(int argc, char **argv)
{
	CString mode(argv[1]);
	std::cout << "[runPipeServer]Running mode=[" << CT2CA(mode) << "]\n";
	if(mode == _T("1")) return runPipeServer1(argc, argv);

	int rc = 0;
	std::cout << "[runPipeServer]Done.\n\n";
	return rc;
}
