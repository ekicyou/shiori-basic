#include "StdAfx.h"
#include "Pipe.h"
#include "Util.h"

CPipe::CPipe(void)
{
}

CPipe::~CPipe(void)
{
}

/* ----------------------------------------------------------------------------
 * Getter
 */
LPCTSTR CPipe::GetName() const { return mPipeName; }

/* ----------------------------------------------------------------------------
 * �p�C�v�̍쐬
 */
static bool firstCreate = true;

void CPipe::Create(void)
{
	ATLTRACE2(_T("[%s]start.\n"), _T(__FUNCTION__));

	// ����Ȃ烊�Z�b�g
	if(firstCreate){
		firstCreate = false;
		srand(GetTickCount());
	}

	// ���O�̍쐬
	for(UINT uUnique=rand() ;;uUnique++){
		if(uUnique==0) continue;
		CAtlString buf;
		buf.Format(_T("\\\\.\\pipe\\sl%x"), uUnique);
		mPipeName = CPath(buf);
		break;
	}
	ATLTRACE2(_T("[%s]name = [%s]\n"), _T(__FUNCTION__), (LPCTSTR) mPipeName);

	// SECURITY_ATTRIBUTES �̐ݒ�(�p�C�v�����̂ɕK�v)
	SECURITY_ATTRIBUTES secAtt;
	secAtt.nLength              = sizeof(SECURITY_ATTRIBUTES);
	secAtt.lpSecurityDescriptor = NULL;
	secAtt.bInheritHandle       = FALSE;




	//
	ATLTRACE2(_T("[%s]end.\n"), _T(__FUNCTION__));
}

