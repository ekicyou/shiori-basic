// ----------------------------------------------------------------------------
// PLUGIN-SHIORI SHIOLINK3.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#include "stdafx.h"
#define SHIORI_API_EXPORT
#include "shiori.h"

/**----------------------------------------------------------------------------
 * �O���[�o���C���X�^���X
 */
static HINSTANCE hinst;


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

// std::string �� HGLOBAL
static HGLOBAL AllocString(const std::string& test, long& len)
{
   HGLOBAL hText =GlobalAlloc(GMEM_FIXED ,test.length());
   CopyMemory(hText ,test.data() ,test.length());
   len =(long)test.length();
   return hText;
}


/* ----------------------------------------------------------------------------
 * UNICODE �R���o�[�g
 */

// ANSI��UTF8
static std::string ansi_to_utf8(const std::string& ansi)
{
   std::string utf8;
   Poco::UnicodeConverter::toUTF8(CA2W(ansi.c_str()), utf8);
   return utf8;
}



/* ----------------------------------------------------------------------------
 * �x Method / load
 */
SHIORI_API BOOL __cdecl load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
   AutoGrobalFree autoFree(hGlobal_loaddir);
   if (proxy != NULL) {
      delete proxy;
      proxy = NULL;
   }
   std::string loaddir((const char*)hGlobal_loaddir, (size_t)loaddir_len);
   proxy = new ProcessProxy(hinst ,loaddir);
   return true;
}

/* ----------------------------------------------------------------------------
 * �x Method / unload
 */
SHIORI_API BOOL __cdecl unload(void)
{
   if (proxy != NULL) {
      delete proxy;
      proxy = NULL;
   }
   return true;
}

/* ----------------------------------------------------------------------------
 * �x Method / request
 */
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long& len)
{
   AutoGrobalFree autoFree(hGlobal_request);
   std::string request((const char *)hGlobal_request, len);
   std::string response;
   bool rc = proxy->request(request ,response);
   if (!rc) {
      CreateBatRequestResponse(response ,"RequestCLIPS return false");
   }
   return AllocString(response ,len);
}

/**----------------------------------------------------------------------------
 * Dll�G���g���[�|�C���g
 */
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

// EOF