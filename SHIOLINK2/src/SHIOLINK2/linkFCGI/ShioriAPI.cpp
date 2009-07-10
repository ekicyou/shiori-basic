// ----------------------------------------------------------------------------
// 別プロセス通信SHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ShioriAPI.h"

CShioriAPI::CShioriAPI(LPCTSTR loadDir)
: mLoadDir(loadDir)
{
}

CShioriAPI::~CShioriAPI(void)
{
}

// リクエストを処理し、応答を作成します。
bool CShioriAPI::Request(const BYTE *req, const long reqLength, ByteArray &res)
{
	return false;
}
