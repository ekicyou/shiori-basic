// ----------------------------------------------------------------------------
// 別プロセス通信SHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#pragma once

class ByteArray : public CAtlArray<BYTE>{};

void CreateBatRequestResponse(ByteArray &res, LPSTR msg);

class Pushd
{
private:
	CString mOldDir;

public:
	Pushd(LPCTSTR newdir);
	~Pushd();
};

#define SET_SHORT_LH(_st_,_v_,_l_,_h_) {	\
	_st_._l_ = (BYTE) ( (_v_)       % 0x0ff );	\
	_st_._h_ = (BYTE) (((_v_) >> 8) % 0x0ff );	\
}

// EOF