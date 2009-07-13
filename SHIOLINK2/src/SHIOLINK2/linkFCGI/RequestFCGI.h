#pragma once

#include "ShioriAPI.h"


class CRequestFCGI
{
private:
	int mRequestId;
	const BYTE *mReq;
	const long mReqLength;
	ByteArray &mRes;

public:
	CRequestFCGI(int requestId, const BYTE *req, const long reqLength, ByteArray &res);
	~CRequestFCGI(void);
};
