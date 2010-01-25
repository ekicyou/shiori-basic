// ----------------------------------------------------------------------------
// �ʃv���Z�X�ʐMSHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#include "StdAfx.h"
#include "util.h"

/* ----------------------------------------------------------------------------
 * �G���[���b�Z�[�W�쐬
 */
void CreateBatRequestResponse(CharArray &res, LPSTR msg){
	CAtlStringA text(      
		"SHIORI/3.0 500 Internal Server Error\r\n"
		"Charset: UTF-8\r\n"
		"Sender: SHIOLINK2\r\n"
		"X-SHIOLINK-Reason: "
		);
	text += msg;
	text += "\r\n\r\n";
	res.SetCount(text.GetLength());
	::CopyMemory(res.GetData(), (LPCSTR)text, text.GetLength());
}

/**----------------------------------------------------------------------------
 * �J�����g�f�B���N�g���ړ������A
 */

Pushd::Pushd(LPCTSTR newdir)
	:mOldDir()
{
	TCHAR buf[_MAX_PATH+1];
	GetCurrentDirectory(sizeof(buf), buf);
	mOldDir = buf;
	BOOL rc = SetCurrentDirectory(newdir);
	if(!rc) AtlThrow( FAILED(ERROR_CURRENT_DIRECTORY) );
}

Pushd::~Pushd()
{
	if(mOldDir.IsEmpty()) return;
	SetCurrentDirectory(mOldDir);
}

// EOF