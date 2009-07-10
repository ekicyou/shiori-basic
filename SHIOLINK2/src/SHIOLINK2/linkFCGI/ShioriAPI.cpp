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
bool CShioriAPI::Request(const BYTE *req, const int reqLength, ByteArray &res)
{
	return false;
}
