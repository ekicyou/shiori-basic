// ----------------------------------------------------------------------------
// �ʃv���Z�X�ʐMSHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#include "stdafx.h"

#include "ShioriAPI.h"
#include "shiori.h"

/**----------------------------------------------------------------------------
 * �O���[�o���C���X�^���X
 */
static HINSTANCE hinst;
static CShioriAPI *api;


/**----------------------------------------------------------------------------
 * HGLOBAL�֌W
 */
// �����J��
class AutoGrobalFree
{
public:
   HGLOBAL m_hGlobal;
   AutoGrobalFree(HGLOBAL hGlobal) {
      m_hGlobal =hGlobal;
   }
   ~AutoGrobalFree() {
      GlobalFree(m_hGlobal);
   }
};

// ���������쐬
static HGLOBAL AllocArray(const ByteArray &res){


	return NULL;
}

/* ----------------------------------------------------------------------------
 * �G���[���b�Z�[�W�쐬
 */
static void CreateBatRequestResponse(ByteArray &res, LPSTR msg){



}

/* ----------------------------------------------------------------------------
 * �x Method / load
 */
SHIORI_API BOOL __cdecl load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
   AutoGrobalFree autoFree(hGlobal_loaddir);
   if (api != NULL) {
      delete api;
      api = NULL;
   }
//   CAtlStringA loaddirA((const char*)hGlobal_loaddir, (size_t)loaddir_len);
//   CAtlString loaddir(loaddirA);
   CAtlString loaddir((LPSTR)hGlobal_loaddir, (size_t)loaddir_len);
   api = new CShioriAPI(loaddir);
   return true;
}

/* ----------------------------------------------------------------------------
 * �x Method / unload
 */
SHIORI_API BOOL __cdecl unload(void)
{
   if (api != NULL) {
      delete api;
      api = NULL;
   }
   return true;
}

/* ----------------------------------------------------------------------------
 * �x Method / request
 */
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long& len)
{
   AutoGrobalFree autoFree(hGlobal_request);
   ByteArray res;
   bool rc = api->Request((const BYTE*) hGlobal_request, len, res);
   if (!rc) {
      CreateBatRequestResponse(res ,"SHIOLINK2 API return false");
   }
   return AllocArray(res);
}

/**----------------------------------------------------------------------------
 * Dll�G���g���[�|�C���g
 */

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" __declspec(dllexport) BOOL WINAPI DllMain(
      HINSTANCE hinstDLL,  // DLL ���W���[���̃n���h��
      DWORD fdwReason,     // �֐����Ăяo�����R
      LPVOID lpvReserved   // �\��ς�
   )
{
   switch (fdwReason) {
   case    DLL_PROCESS_ATTACH: // �v���Z�X�ڑ�
      hinst =hinstDLL;
      break;

   case    DLL_PROCESS_DETACH: // �v���Z�X�؂藣��
      unload();
      break;

   case    DLL_THREAD_ATTACH:  // �X���b�h�ڑ�
      break;

   case    DLL_THREAD_DETACH:  // �X���b�h�؂藣��
      break;
   }
   return true;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// EOF