// ----------------------------------------------------------------------------
// �ʃv���Z�X�ʐMSHIORI SHIOLINK.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#include "stdafx.h"
#include "StdErrReadThread.h"

// �X���b�h���C�����[�`���̌Ăяo��
static DWORD WINAPI DoThread(LPVOID lpThreadParameter)
{
   StdErrReadThread* obj =(StdErrReadThread*) lpThreadParameter;
   return obj->MainLoop();
}

// �R���X�g���N�^
StdErrReadThread::StdErrReadThread(HANDLE hReadPipe)
{
   isJoinWait = false;
   ossLegth = 0;
   this->hReadPipe = hReadPipe;
   InitializeCriticalSection(&ce);
   hThread = CreateThread(NULL, 0, DoThread,	(LPVOID)this, 0, &threadID);
}

// �f�X�g���N�^
StdErrReadThread::~StdErrReadThread(void)
{
   Kill();
   Join();
   CloseHandle(hReadPipe);
   CloseHandle(hThread);
   DeleteCriticalSection(&ce);
}

// �X���b�h�̋�����~
void StdErrReadThread::Kill()
{
   TerminateThread(hThread, 0);
}

// �X���b�h�̏I���ҋ@
void StdErrReadThread::Join()
{
   isJoinWait = true;
   WaitForSingleObject(hThread, 1000);
   ATLTRACE("NOT 1000ms END. Kill!!\n");
   Kill();
   WaitForSingleObject(hThread, INFINITE);
}

// ���݂̓��e��S�Ď��o��
std::string StdErrReadThread::GetText()
{
   if(ossLegth == 0) return "";

   CriticalSection criticalSection(&ce);
   std::string rc = oss.str();
   ossLegth = 0;
   oss.str("");
   oss.clear(oss.goodbit);
   return rc;
}

DWORD StdErrReadThread::MainLoop()
{
   char buf[1025];
   DWORD readlen;
   ATLTRACE("StdErrReadThread::MainLoop start\n");

   while(!isJoinWait)
   {
      // �P�o�C�g�ǂݍ���
      BOOL rc = ReadFile(hReadPipe, buf, 1, &readlen,NULL);
      if(rc = FALSE){
         DWORD err = GetLastError();
         switch(err){
            case ERROR_BROKEN_PIPE:
            case ERROR_HANDLE_EOF: ExitThread(0);
         }
         ATLTRACE("SttErrReadThread::MainLoop UNNONE ERROR! %d\n",err);
         ATLTRACE("StdErrReadThread::MainLoop end\n");
         ExitThread(err);
      }
      else{
         CriticalSection criticalSection(&ce);
         if(ossLegth < MAX_OSS_LENGTH){
            oss << buf[0];
            ossLegth += 1;
         }

         // �c��̃o�C�g�����m�F����
         DWORD remain;
         if (PeekNamedPipe(hReadPipe ,NULL ,0 ,NULL, &remain ,NULL)!=TRUE) {
            throw std::ios_base::failure("PeekNamedPipe ERROR");
         }

         // �c��̏���ǂݍ���
         while (remain > 0) {
            DWORD size = remain;
            if (size>sizeof(buf)-1) size = sizeof(buf)-1;
            if (ReadFile(hReadPipe, buf ,size ,&readlen ,NULL)!=TRUE) {
               throw std::ios_base::failure("READ PIPE ERROR(1)");
            }
            if(ossLegth < MAX_OSS_LENGTH){
               buf[readlen] = '\0';
               oss << buf;
               ossLegth += readlen;
            }
            remain -= readlen;
         }
      }
   }
   ATLTRACE("StdErrReadThread::MainLoop end\n");
   ExitThread(0);
   return 0;
}


// EOF