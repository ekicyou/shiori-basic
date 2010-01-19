#include "StdAfx.h"
#include "RequestFCGI.h"

CRequestFCGI::CRequestFCGI(int requestId, const BYTE *req, const long reqLength, ByteArray &res)
: mRequestId(requestId)
, mReq(req)
, mReqLength(reqLength)
, mRes(res)
, mEventEndRequest(FALSE, FALSE)
{
}

CRequestFCGI::~CRequestFCGI(void)
{
}

int CRequestFCGI::Execute(void)
{
	return TRUE;
}
