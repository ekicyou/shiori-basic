#pragma once

#include "fcgibase.h"
#include "fastcgi.h"
#include "util.h"

class CFcgiAPI
{
public:

	// �X�g���[���p�P�b�g�𑗐M����B���M�I����-1��Ԃ��B
	static BOOL SendStreamPacket(HANDLE hSend, int recType, USHORT recID, ByteArray buf);
};
