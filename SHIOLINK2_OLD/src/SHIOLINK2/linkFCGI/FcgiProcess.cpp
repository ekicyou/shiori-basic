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

	// ���W�b�N�Q�l
	// http://www.k5.dion.ne.jp/~minyu/program/windows/pipe.html

	// SECURITY_ATTRIBUTES �̐ݒ�(�p�C�v�����̂ɕK�v)
	SECURITY_ATTRIBUTES secAtt;
	secAtt.nLength              = sizeof(SECURITY_ATTRIBUTES);
	secAtt.lpSecurityDescriptor = NULL;
	secAtt.bInheritHandle       = FALSE;

	// �p�C�v�̍쐬
	HANDLE hPipe[3][2];
	const int R=0, W=1, STDIN=0, STDOUT=1, STDERR=2;
	CHandle hSTDIN, hSTDOUT, hSTDERR;
	CreatePipe(	&hPipe[STDIN][R],	// �ǂݍ��ݗp�n���h��
				&hPipe[STDIN][W],	// �������ݗp�n���h��
				&secAtt,			// ?
				0);					// �o�b�t�@�̃T�C�Y
	CreatePipe(	&hPipe[STDOUT][R],	// �ǂݍ��ݗp�n���h��
				&hPipe[STDOUT][W],	// �������ݗp�n���h��
				&secAtt,			// ?
				0);					// �o�b�t�@�̃T�C�Y
	CreatePipe(	&hPipe[STDERR][R],	// �ǂݍ��ݗp�n���h��
				&hPipe[STDERR][W],	// �������ݗp�n���h��
				&secAtt,			// ?
				0);					// �o�b�t�@�̃T�C�Y
	mWritePipe.Attach(hPipe[STDIN][W]);
	mReadPipe.Attach(hPipe[STDOUT][R]);
	mErrPipe.Attach(hPipe[STDERR][R]);

	// �q�v���Z�X�̋N�����̏��
    // �����ݒ肵�Ȃ���, �R���\�[���n�v���Z�X�ł�,
    // �R���\�[���������オ��B
	memset(&mStartInfo,0,sizeof(STARTUPINFO));
	mStartInfo.cb = sizeof(STARTUPINFO); // �\���̂̑傫��;
	mStartInfo.dwFlags = STARTF_USEFILLATTRIBUTE |
                         STARTF_USECOUNTCHARS |
                         STARTF_USESTDHANDLES | /*�n���h�����p�����郁���o*/
                         STARTF_USESHOWWINDOW;  // �ǂ̃����o���L����
	mStartInfo.wShowWindow = SW_HIDE;

	// pipe�̕���
	HANDLE hProc = GetCurrentProcess();
	DupPipe(hProc ,hPipe[STDIN][R]  ,mStartInfo.hStdInput);
	DupPipe(hProc ,hPipe[STDOUT][W] ,mStartInfo.hStdOutput);
	DupPipe(hProc ,hPipe[STDERR][W] ,mStartInfo.hStdError);

	// �v���Z�X�̋N��
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
	// �q�v���Z�X���I��܂Œ�~
	if (mProcessInfo.hProcess != NULL) {
		WaitForSingleObject(mProcessInfo.hProcess,INFINITE);
		CloseHandle(mProcessInfo.hProcess);
	}


	ATLTRACE2(_T("[CFcgiProcess::~CFcgiProcess]end.\n"));
}
