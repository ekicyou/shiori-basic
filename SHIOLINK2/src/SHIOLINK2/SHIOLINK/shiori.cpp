// ----------------------------------------------------------------------------
// 別プロセス通信SHIORI SHIOLINK2.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#include "stdafx.h"

#include "ShioriAPI.h"
#include "shiori.h"
#include "util.h"

/**----------------------------------------------------------------------------
 * グローバルインスタンス
 */
static HINSTANCE hinst;
static CShioriAPI *api = NULL;


/**----------------------------------------------------------------------------
 * HGLOBAL関係
 */
// 自動開放
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


/* ----------------------------------------------------------------------------
 * 栞 Method / load
 */
SHIORI_API BOOL __cdecl load(HGLOBAL hGlobal_loaddir, long loaddir_len)
{
   AutoGrobalFree autoFree(hGlobal_loaddir);
   unload();
   CAtlString loaddir((LPSTR)hGlobal_loaddir, (int)loaddir_len);
   api = new CShioriAPI(hinst, loaddir);
   return true;
}

/* ----------------------------------------------------------------------------
 * 栞 Method / unload
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
 * 栞 Method / request
 */
SHIORI_API HGLOBAL __cdecl request(HGLOBAL hGlobal_request, long& len)
{
	AutoGrobalFree autoFree(hGlobal_request);
	ByteArray res;
	bool rc = api->Request((const BYTE*) hGlobal_request, len, res);
	if (!rc) {
		CreateBatRequestResponse(res ,"SHIOLINK2 API return false");
	}

	// 応答情報の作成
	HGLOBAL hRES =GlobalAlloc(GMEM_FIXED ,res.GetCount());
	CopyMemory(hRES ,res.GetData() ,res.GetCount());
	len =(long)res.GetCount();
	return hRES;
}

/**----------------------------------------------------------------------------
 * Dllエントリーポイント
 */

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" __declspec(dllexport) BOOL WINAPI DllMain(
      HINSTANCE hinstDLL,  // DLL モジュールのハンドル
      DWORD fdwReason,     // 関数を呼び出す理由
      LPVOID lpvReserved   // 予約済み
   )
{
   switch (fdwReason) {
   case    DLL_PROCESS_ATTACH: // プロセス接続
      hinst =hinstDLL;
      break;

   case    DLL_PROCESS_DETACH: // プロセス切り離し
      unload();
      break;

   case    DLL_THREAD_ATTACH:  // スレッド接続
      break;

   case    DLL_THREAD_DETACH:  // スレッド切り離し
      break;
   }
   return true;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// EOF