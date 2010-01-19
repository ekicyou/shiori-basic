#pragma once

#include "fcgibase.h"
#include "fastcgi.h"
#include "util.h"

class CFcgiAPI
{
public:

	// ストリームパケットを送信する。送信終了で-1を返す。
	static BOOL SendStreamPacket(HANDLE hSend, int recType, USHORT recID, ByteArray buf);
};
