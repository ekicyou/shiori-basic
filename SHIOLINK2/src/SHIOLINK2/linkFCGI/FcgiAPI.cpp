#include "StdAfx.h"
#include "FcgiAPI.h"


// ストリームパケットを送信する。
BOOL CFcgiAPI::SendStreamPacket(HANDLE hSend, int recType, USHORT recID, ByteArray buf)
{
	int pos = 0;
	FCGI_Header header;
	header.version  = 1;
	header.reserved = 0;
	header.type     = (BYTE) recType;
	SET_SHORT_LH(header, recType, requestIdB0, requestIdB1);




	return TRUE;
}
