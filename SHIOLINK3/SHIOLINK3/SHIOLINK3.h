// �ȉ��� ifdef �u���b�N�� DLL ����̃G�N�X�|�[�g��e�Ղɂ���}�N�����쐬���邽�߂� 
// ��ʓI�ȕ��@�ł��B���� DLL ���̂��ׂẴt�@�C���́A�R�}���h ���C���Œ�`���ꂽ SHIOLINK3_EXPORTS
// �V���{�����g�p���ăR���p�C������܂��B���̃V���{���́A���� DLL ���g�p����v���W�F�N�g�ł͒�`�ł��܂���B
// �\�[�X�t�@�C�������̃t�@�C�����܂�ł��鑼�̃v���W�F�N�g�́A 
// SHIOLINK3_API �֐��� DLL ����C���|�[�g���ꂽ�ƌ��Ȃ��̂ɑ΂��A���� DLL �́A���̃}�N���Œ�`���ꂽ
// �V���{�����G�N�X�|�[�g���ꂽ�ƌ��Ȃ��܂��B
#ifdef SHIOLINK3_EXPORTS
#define SHIOLINK3_API __declspec(dllexport)
#else
#define SHIOLINK3_API __declspec(dllimport)
#endif

// ���̃N���X�� SHIOLINK3.dll ����G�N�X�|�[�g����܂����B
class SHIOLINK3_API CSHIOLINK3 {
public:
	CSHIOLINK3(void);
	// TODO: ���\�b�h�������ɒǉ����Ă��������B
};

extern SHIOLINK3_API int nSHIOLINK3;

SHIOLINK3_API int fnSHIOLINK3(void);
