// ----------------------------------------------------------------------------
// �ʃv���Z�X�ʐMSHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
// �p�C�v�ʐM�v���Z�X�Ǘ�
#include "StdAfx.h"
#include "PipeProcess.h"
#include <stdlib.h>

/* ----------------------------------------------------------------------------
 * �f�X�g���N�^
 */
CPipeProcess::~CPipeProcess(void)
{
}

/* ----------------------------------------------------------------------------
 * �R���X�g���N�^
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
 * �p�C�v�̍쐬
 */

void CPipeProcess::CreatePipe()
{
	ATLTRACE2(_T("[CPipeProcess::CreatePipe]start.\n"));

	// ���O�̍쐬
	for(UINT uUnique=GenRandomNumber() ;;uUnique++){
		TCHAR buf[64];
		if(uUnique==0) continue;
		if(GetTempFileName(_T("\\\\.\\pipe\\"), _T("sl"), uUnique, buf)) continue;

		mPipeName = buf;
		break;
	}
	ATLTRACE2(_T("[CPipeProcess::CreatePipe]name = [%s]\n"),(LPCTSTR) mPipeName);

	// SECURITY_ATTRIBUTES �̐ݒ�(�p�C�v�����̂ɕK�v)
	SECURITY_ATTRIBUTES secAtt;
	secAtt.nLength              = sizeof(SECURITY_ATTRIBUTES);
	secAtt.lpSecurityDescriptor = NULL;
	secAtt.bInheritHandle       = FALSE;




	//
	ATLTRACE2(_T("[CPipeProcess::CreatePipe]end.\n"));
}

// ----------------------------------------------------------------------------
// EOF