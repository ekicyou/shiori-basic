#pragma once

#include "fcgibase.h"


class CRequestFCGI
{
private:
	int mRequestId;
	const BYTE *mReq;
	const long mReqLength;
	ByteArray &mRes;
	CEvent mEventEndRequest;

public:
	CRequestFCGI(int requestId, const BYTE *req, const long reqLength, ByteArray &res);
	virtual ~CRequestFCGI(void);

	int Execute();
};
