// ----------------------------------------------------------------------------
// �ʃv���Z�X�ʐMSHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#pragma once

#include "fcgibase.h"

class CShioriAPI
{
private:
	HINSTANCE  mInst;
	CPath mLoadDir;

public:
	CShioriAPI(HINSTANCE hInst, LPCTSTR loadDir);
	~CShioriAPI(void);
public:
	// ���N�G�X�g���������A�������쐬���܂��B
	bool Request(const BYTE *req, const long reqLength, ByteArray &res);
};
