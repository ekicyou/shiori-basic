// SHIOLINK3.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "SHIOLINK3.h"


// ����́A�G�N�X�|�[�g���ꂽ�ϐ��̗�ł��B
SHIOLINK3_API int nSHIOLINK3=0;

// ����́A�G�N�X�|�[�g���ꂽ�֐��̗�ł��B
SHIOLINK3_API int fnSHIOLINK3(void)
{
	return 42;
}

// ����́A�G�N�X�|�[�g���ꂽ�N���X�̃R���X�g���N�^�[�ł��B
// �N���X��`�Ɋւ��Ă� SHIOLINK3.h ���Q�Ƃ��Ă��������B
CSHIOLINK3::CSHIOLINK3()
{
	return;
}
