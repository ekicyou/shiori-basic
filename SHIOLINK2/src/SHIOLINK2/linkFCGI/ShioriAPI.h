#pragma once

class ByteArray : CAtlArray<BYTE>{};

class CShioriAPI
{
private:
	CAtlString mLoadDir;
public:
	CShioriAPI(LPCTSTR loadDir);
	~CShioriAPI(void);
public:
	// ���N�G�X�g���������A�������쐬���܂��B
	bool Request(const BYTE *req, const int reqLength, ByteArray &res);
};
