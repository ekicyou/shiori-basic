#include "StdAfx.h"
#include "ShioriAPI.h"

CShioriAPI::CShioriAPI(LPCTSTR loadDir)
: mLoadDir(loadDir)
{
}

CShioriAPI::~CShioriAPI(void)
{
}

// ���N�G�X�g���������A�������쐬���܂��B
bool CShioriAPI::Request(const BYTE *req, const int reqLength, ByteArray &res)
{
	return false;
}
