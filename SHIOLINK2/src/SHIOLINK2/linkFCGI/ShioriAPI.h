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
	// リクエストを処理し、応答を作成します。
	bool Request(const BYTE *req, const int reqLength, ByteArray &res);
};
