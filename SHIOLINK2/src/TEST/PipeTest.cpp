#include "stdafx.h"
#include "Pipe.h"

TEST(PipeTest, Create)
{
	_ATLTRY
	{
		CServerPipe server;
		server.Create();
		CClientPipe client(server.GetID());
		CHAR buf[2];
		{
			LPCSTR send = "a";
			server.Write(send, 1);
			client.Read(buf, 1);
			ASSERT_EQ('a', buf[0]);
		}
		{
			LPCSTR send = "x";
			client.Write(send, 1);
			server.Read(buf, 1);
			ASSERT_EQ('x', buf[0]);
		}
		{
			CStringA text, result;
			server.WriteNetString(text);
			CharArray buf;
			ASSERT_TRUE(client.ReadNetString(buf, result));
			ASSERT_EQ(text, result);
		}
		{
			CStringA text('a',9), result;
			server.WriteNetString(text);
			CharArray buf;
			ASSERT_TRUE(client.ReadNetString(buf, result));
			ASSERT_EQ(text, result);
		}
		{
			CStringA text('b',99), result;
			server.WriteNetString(text);
			CharArray buf;
			ASSERT_TRUE(client.ReadNetString(buf, result));
			ASSERT_EQ(text, result);
		}
		{
			CStringA text('c',999), result;
			server.WriteNetString(text);
			CharArray buf;
			ASSERT_TRUE(client.ReadNetString(buf, result));
			ASSERT_EQ(text, result);
		}
		{
			CStringA text('d',1000), result;
			server.WriteNetString(text);
			CharArray buf;
			ASSERT_TRUE(client.ReadNetString(buf, result));
			ASSERT_EQ(text, result);
		}
	}
	_ATLCATCH(e){
		CString mes = GetWinErrMessage(e.m_hr);
		GTEST_FATAL_FAILURE_(CT2CA(mes));
	}
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
	// �N���p�����[�^
	CString path = CreateTestExePath(param);
	
	STARTUPINFO sInfo;
	memset(&sInfo,0,sizeof(sInfo));	sInfo.cb = sizeof(sInfo);

	// �N��
	BOOL rc = CreateProcess(NULL, path.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, pInfo);
	return rc;
}

TEST(PipeTest, Exec1) {
	// �v���Z�X�N��
	PROCESS_INFORMATION pInfo;
	BOOL rc = CreateTestProcess(_T("1"), &pInfo);
	if(rc != TRUE){
		DWORD dwError = ::GetLastError();
		ATLTRACE2( _T("ShellExecuteEx --> %X\n"), dwError);
		FAIL();
	}
	CHandle hProcess(pInfo.hProcess);
	
	// �v���Z�X�I���ҋ@����
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


TEST(PipeTest, Exec2) {
	// �p�C�v�̍쐬
	CServerPipe pipe;
	pipe.Create();

	// ����
	CString param;
	param.Format(_T("2 %s"), pipe.GetID());

	// �N��
	PROCESS_INFORMATION pInfo;
	BOOL rc = CreateTestProcess(param, &pInfo);
	if(rc != TRUE){
		DWORD dwError = ::GetLastError();
		ATLTRACE2( _T("ShellExecuteEx --> %X\n"), dwError);
		FAIL();
	}
	CHandle hProcess(pInfo.hProcess);

	// �p�C�v���M
	{
		pipe.WaitForConnection();
		LPCSTR send = "a";
		pipe.Write(send, 1);
	}

	// �v���Z�X�I���ҋ@����
	ASSERT_EQ(WAIT_OBJECT_0, WaitForSingleObject(hProcess, 2000));
	DWORD exitCode = -1;
    GetExitCodeProcess(hProcess, &exitCode);
    EXPECT_EQ(0, exitCode);

	// �p�C�v��M
	{
		CHAR buf[2];
		pipe.Read(buf, 1);
		ASSERT_EQ('a', buf[0]);
	}
}
static int runPipeServer2(int argc, char **argv)
{
	std::cout << "[runPipeServer2]********\n";
	if(argc < 3) return 1;
	_ATLTRY
	{
		// �I�E���Ԃ�
		CString pipeID(argv[2]);
		CClientPipe pipe(pipeID);
		CHAR buf[2];
		pipe.Read(buf, 1);
		pipe.Write(buf, 1);
		return 0;
	}
	_ATLCATCHALL(){
		return 2;
	}
	return 99;
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
