// ----------------------------------------------------------------------------
// �ʃv���Z�X�ʐMSHIORI SHIOLINK.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#include "stdafx.h"
#include "ShioriEXE.h"
#include "Log.h"


#define LOGGER_NAME     *log

#define R 0
#define W 1

static void DupPipe(HANDLE hProc , HANDLE srcPipe ,HANDLE &dist)
{
   DuplicateHandle(hProc, srcPipe, hProc, &dist, 0, TRUE, DUPLICATE_SAME_ACCESS);
   CloseHandle(srcPipe);
}

ShioriEXE::ShioriEXE(
   Logger& logger,
   const std::wstring& command,
   const bool viewConsole, const Poco::Timespan wt):
      waitTime(wt)
{
   log =&logger;
   WRITE_DEBUG("EXE: OPEN command [");
   WRITE_DEBUG(command);
   WRITE_DEBUG("]\r\n");
   errReadThread = NULL;

   // �R�}���h���C�����o�b�t�@�ɃR�s�[
   Poco::Buffer<WCHAR> cmdbuf(command.length()+1);
   std::copy(command.begin() ,command.end() ,cmdbuf.begin());
   cmdbuf[cmdbuf.size()-1] =0;

   // ���W�b�N�Q�l
   // http://www.k5.dion.ne.jp/~minyu/program/windows/pipe.html

   // SECURITY_ATTRIBUTES �̐ݒ�(�p�C�v�����̂ɕK�v)
   SECURITY_ATTRIBUTES secAtt;
   secAtt.nLength              = sizeof(SECURITY_ATTRIBUTES);
   secAtt.lpSecurityDescriptor = NULL;
   secAtt.bInheritHandle       = FALSE;

   // �p�C�v�쐬 �v�͐V�����n���h�����쐬
   CreatePipe(&hWritePipe[R], // �ǂݍ��ݗp�n���h��
              &hWritePipe[W], // �������ݗp�n���h��
              &secAtt,        // ?
              0);             // �o�b�t�@�̃T�C�Y

   CreatePipe(&hReadPipe[R],  // �ǂݍ��ݗp�n���h��
              &hReadPipe[W],  // �������ݗp�n���h��
              &secAtt,        // ?
              0);             // �o�b�t�@�̃T�C�Y

   CreatePipe(&hErrPipe[R],  // �ǂݍ��ݗp�n���h��
              &hErrPipe[W],  // �������ݗp�n���h��
              &secAtt,        // ?
              0);             // �o�b�t�@�̃T�C�Y

   /* �q�v���Z�X�̋N�����̏��
    * �����ݒ肵�Ȃ���, �R���\�[���n�v���Z�X�ł�,
    * �R���\�[���������オ��B */
   memset(&startInfo,0,sizeof(STARTUPINFO));
   startInfo.cb = sizeof(STARTUPINFO); // �\���̂̑傫��;
   startInfo.dwFlags = STARTF_USEFILLATTRIBUTE |
                       STARTF_USECOUNTCHARS |
                       STARTF_USESTDHANDLES | /*�n���h�����p�����郁���o*/
                       STARTF_USESHOWWINDOW;  // �ǂ̃����o���L����

   // �E�B���h�E�\���̃p�����[�^;
   if (viewConsole) startInfo.wShowWindow = SW_SHOWDEFAULT;
   else            startInfo.wShowWindow = SW_HIDE;

   // pipe�̕���
   HANDLE hProc = GetCurrentProcess();
   DupPipe(hProc ,hWritePipe[R] ,startInfo.hStdInput);
   DupPipe(hProc ,hReadPipe[W]  ,startInfo.hStdOutput);
   DupPipe(hProc ,hErrPipe[W]  ,startInfo.hStdError);

   if (CreateProcessW(NULL ,cmdbuf.begin() ,NULL ,NULL ,TRUE,
                      0,NULL,NULL,&startInfo,&proInfo) != TRUE) {
      throw std::exception("NOT OPEN SHIORI EXE");
   }

   std::string cmd8;
   Poco::UnicodeConverter::toUTF8(command ,cmd8);
   LINE_INFO("EXE: RUN command [" ,cmd8 ,"]");
   // �s�v�ȃn���h���͕���
   CloseHandle(proInfo.hThread);

   // stderr�̓ǂݎ̂ăX���b�h
   errReadThread = new StdErrReadThread(hErrPipe[R]);
}


ShioriEXE::~ShioriEXE(void)
{
   LINE_DEBUG("EXE: unload start");
   if (proInfo.hProcess != NULL) {
      // �q�v���Z�X���I��܂Œ�~
      WaitForSingleObject(proInfo.hProcess,INFINITE);
      CloseHandle(proInfo.hProcess);
   }
   if(errReadThread != NULL) delete errReadThread;
   CloseHandle(hWritePipe[W]);
   CloseHandle(hReadPipe[R]);
   LINE_INFO("EXE: unload");
}

void ShioriEXE::Kill(void)
{
   if (proInfo.hProcess != NULL) {
      LINE_WARN("EXE: kill!!");
      TerminateProcess(proInfo.hProcess ,0);
   }
}

bool ShioriEXE::eof() const
{
   return false;
}

// stderr��ǂݎ̂Ă�
void ShioriEXE::ClearStderr()
{
   std::string errText = errReadThread->GetText();
   if(errText.length()==0) return;
   LINE_INFO("EXE: read stderr *---------------------------* start");
   LINE_INFO(errText.c_str());
   LINE_INFO("EXE: *---------------------------------------*   end");
   LINE_INFO("");
}


void ShioriEXE::WriteLine(const std::string& line)
{
   // �������ݖ{��
   DWORD len;
   std::string buf = line + "\r\n";
   WriteFile(hWritePipe[W], buf.data(), buf.length() ,&len ,NULL);
   FlushFileBuffers(hWritePipe[W]);
}

std::string ShioriEXE::ReadLine()
{
   std::ostringstream buf;
   // ���s�𔭌�����܂ŃL���[��ǂݍ���
   while (true) {
      if (queue.empty()) {
         ReadQueue();
         continue;
      }
      char c = queue.front();
      queue.pop();
      if (c == '\n') break;
      if (c == '\r') continue;
      buf << c;
   }
   // ���ʂ�Ԃ�
   std::string line =buf.str();
   return line;
}

void ShioriEXE::ReadQueue()
{
   char buf[1024];
   DWORD readlen;

   // �܂��P�����ǂݍ���
   if (ReadFile(hReadPipe[R], buf ,1 ,&readlen ,NULL)!=TRUE) {
      throw std::ios_base::failure("READ PIPE ERROR(1)");
   }
   queue.push(buf[0]);

   // �c��̃o�C�g�����m�F����
   DWORD remain;
   if (PeekNamedPipe(hReadPipe[R] ,NULL ,0 ,NULL, &remain ,NULL)!=TRUE) {
      throw std::ios_base::failure("PeekNamedPipe ERROR");
   }

   // �c��̏���ǂݍ���
   while (remain > 0) {
      DWORD size = remain;
      if (size>sizeof(buf)) size = sizeof(buf);
      if (ReadFile(hReadPipe[R], buf ,size ,&readlen ,NULL)!=TRUE) {
         throw std::ios_base::failure("READ PIPE ERROR(1)");
      }
      for (DWORD i=0; i<readlen ;i++) queue.push(buf[i]);
      remain -= readlen;
   }
}

