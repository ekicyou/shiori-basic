// ----------------------------------------------------------------------------
// �ʃv���Z�X�ʐMSHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
// �p�C�v�ʐM�v���Z�X�Ǘ�
#pragma once
#include "util.h"
#include "pipe.h"

class CPipeProcess
{
private:
	CServerPipe mPipe;
	STARTUPINFO mStartInfo;
	PROCESS_INFORMATION mProcessInfo;

public:
	CPipeProcess(LPCTSTR str_commandline, LPCTSTR str_wrkdir);
	virtual ~CPipeProcess(void);
	const CString GetReqName(void) const;
	const CString GetResName(void) const;

	void Write(LPCSTR buf, DWORD length);
	void Read (LPSTR  buf, DWORD length);

	void Write(const CStringA& text);
	void WriteNetString(const CharArray& buf);
	void WriteNetString(const CStringA& text);

	bool ReadNetString(CharArray& buf, LPSTR& pStart, int& length);
	bool ReadNetString(CharArray& buf, CStringA& text);
};
