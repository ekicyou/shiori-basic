#include "StdAfx.h"
#include "RequestFCGI.h"

CRequestFCGI::CRequestFCGI(int requestId, const BYTE *req, const long reqLength, ByteArray &res)
: mRequestId(requestId)
, mReq(req)
, mReqLength(reqLength)
, mRes(res)
{
}

CRequestFCGI::~CRequestFCGI(void)
{
}
