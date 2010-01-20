// ----------------------------------------------------------------------------
// �ʃv���Z�X�ʐMSHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
// �p�C�v�ʐM�v���Z�X�Ǘ�
#include "StdAfx.h"
#include "PipeProcess.h"

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
LPCTSTR CPipeProcess::GetPipeBaseName() const { return mPipe.GetBaseName(); }
CString CPipeProcess::GetPipeReqName()  const { return mPipe.GetReqName(); }
CString CPipeProcess::GetPipeResName()  const { return mPipe.GetResName(); }

// ----------------------------------------------------------------------------
// EOF