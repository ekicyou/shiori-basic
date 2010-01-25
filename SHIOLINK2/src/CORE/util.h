// ----------------------------------------------------------------------------
// 別プロセス通信SHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#pragma once

class ByteArray : public CAtlArray<CHAR>{};

void CreateBatRequestResponse(ByteArray &res, LPSTR msg);

class Pushd
{
private:
	CString mOldDir;

public:
	Pushd(LPCTSTR newdir);
	~Pushd();
};

// EOF