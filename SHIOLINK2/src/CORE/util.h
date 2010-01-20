// ----------------------------------------------------------------------------
// 別プロセス通信SHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#pragma once

class ByteArray : public CAtlArray<BYTE>{};

void CreateBatRequestResponse(ByteArray &res, LPSTR msg);
void ResetRandomNumber();
UINT GenRandomNumber();

class Pushd
{
private:
	CString mOldDir;

public:
	Pushd(LPCTSTR newdir);
	~Pushd();
};

// EOF