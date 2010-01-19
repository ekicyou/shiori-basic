// ----------------------------------------------------------------------------
// 別プロセス通信SHIORI SHIOLINK2.DLL
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
	// リクエストを処理し、応答を作成します。
	bool Request(const BYTE *req, const long reqLength, ByteArray &res);
};
