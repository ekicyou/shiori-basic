// ----------------------------------------------------------------------------
// �ʃv���Z�X�ʐMSHIORI SHIOLINK.DLL
//   The MIT License
//   http://sourceforge.jp/projects/opensource/wiki/licenses%2FMIT_license
// ----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ProcessProxy.h"
#include "SHIOLINKINI.h"


#define LOGGER_NAME     log


/* ----------------------------------------------------------------------------
 * �R���X�g���N�^
 */
ProcessProxy::ProcessProxy(const HINSTANCE hinst, const std::string& loaddirA):
      charMode(MODE_UTF_8),
      uuidGen(),
      waitLimit(1000000),
      log()

{
   this->hinst   = hinst;
   this->loaddir = CA2W(loaddirA.c_str());
   Init();
}

void ProcessProxy::Init(void)
{
   WCHAR buf[ _MAX_PATH + 1 ];
   Poco::Path dllPath;
   Poco::Path dirPath;
   std::wstring dirPathW;
   std::string baseName;
   std::string dirName;
   std::wstring commandW;

   // DLL���̎擾
   {
      ::GetModuleFileNameW( hinst, buf, _MAX_PATH );
      std::string utf8;
      Poco::UnicodeConverter::toUTF8(buf, utf8);
      dllPath.assign(utf8);
      dirPath  = dllPath.parent();
      Poco::UnicodeConverter::toUTF16(dirPath.toString() ,dirPathW);
      baseName = dllPath.getBaseName();
   }

   bool isLogOpen=false;
   try {
      // ���ݒ�t�@�C���ǂݍ���
      std::string fname(dirPath.toString());
      fname += baseName;
      fname += ".ini";
      std::wstring fnameW;
      Poco::UnicodeConverter::toUTF16(fname ,fnameW);
      SHIOLINKINI ini(fnameW);
      // ��charset�ݒ�
      if (ini.SHIOLINK.charmode == _T("ANSI")) charMode = MODE_ANSI;
      if (ini.SHIOLINK.charmode == _T("ansi")) charMode = MODE_ANSI;

      // �����M���O�ݒ�
      while (true) {
         std::wstring file = Poco::trim(ini.LOGGING.file);
         std::wstring sLv  = Poco::trim(ini.LOGGING.level);
         if (file.empty()) break;
         if (sLv.empty()) break;
         Logger::vLevel level = Logger::levelId(sLv);
         if (level > Logger::vFatal) break;
         std::wstring logpath(dirPathW);
         logpath += file;
         log.SetPath(logpath);
         log.SetLevel(level);
         break;
      }
      LINE_INFO("SHIOLINK: LOGGING START");
      if (charMode == MODE_ANSI) {
         LINE_DEBUG("SHIOLINK: charsetMode = ANSI");
      } else                         {
         LINE_DEBUG("SHIOLINK: charsetMode = UTF-8");
      }

      // ��viewconsole�ݒ�
      bool viewConsole = false;
      // �ǂ������܂������Ȃ��̂ł�������ۗ��B��߂Ƃ����ق����ǂ�����
      /*
      if(ini.SHIOLINK.viewconsole.length() > 0){
         switch(ini.SHIOLINK.viewconsole[0]){
            case _T('t'):
            case _T('T'):
               viewConsole = true;
         }
      }
      */

      // ���q�v���Z�X�N��
      Pushd pushdir(loaddir); // �J�����g�f�B���N�g����GHOST�̃f�B���N�g����
      exe = new ShioriEXE(
         log, ini.SHIOLINK.commandline ,
         viewConsole, waitLimit);

      // ��load�C�x���g���s
      std::string loaddirX;
      if (charMode == MODE_UTF_8) {
         Poco::UnicodeConverter::toUTF8(loaddir ,loaddirX);
      } else {
         loaddirX = CW2A(loaddir.c_str());
      }
      std::string loadCommand("*L:");
      loadCommand += loaddirX;
      exe->WriteLine(loadCommand);
      WRITE_DEBUG(">>>>[load] ");
      WRITE_DEBUG(loadCommand);
      WRITE_DEBUG("\r\n");

   } catch (std::exception& ex) {
      LINE_ERROR("ProcessProxy::Init:: !!exception!! " ,ex.what());
   }

}


/* ----------------------------------------------------------------------------
 * �f�X�g���N�^
 */
ProcessProxy::~ProcessProxy(void)
{
   LINE_DEBUG("ProcessProxy::~ProcessProxy");
   if (exe!=NULL) {
      exe->ClearStderr();
      std::string res;
      exe->WriteLine("*U:");
      LINE_DEBUG(">>>>[unload] *U:");
      SleepEx(0,TRUE);
      exe->ClearStderr();
      delete exe;
   }
   LINE_INFO("SHIOLINK: LOGGING END");
}

/* ----------------------------------------------------------------------------
 * ���N�G�X�g
 */

bool ProcessProxy::request(const std::string& request ,std::string& response)
{
   // ��������
   if (exe==NULL)return false;
   try {
      LINE_DEBUG("\r\n#======================= ProcessProxy::request  ..start");
      // �P�F����������
      Poco::UUID uuid = uuidGen.create();
      std::string syncText("*S:");
      syncText += uuid.toString();

      // �Q�F�����炩�瓯��������𑗂�
      exe->ClearStderr();
      exe->WriteLine(syncText);

      WRITE_DEBUG(">>>>[sync] ");
      WRITE_DEBUG(syncText);
      WRITE_DEBUG("\r\n");

      // �R�F���������瓯�������񂪖߂��Ă���܂Ń��[�v
      while (true) {
         std::string line =  exe->ReadLine();
         if (syncText.compare(line)==0) break;
         if (exe->eof()) throw Poco::IOException("READ PIPE EOF");
      }
      WRITE_DEBUG("<<<<       *SEQ COMMAND RETURN\r\n\r\n");

      // �S�F�{�̂𑗐M
      exe->ClearStderr();
      exe->WriteLine(request);
      WRITE_DEBUG(">>>>[request]\r\n");
      WRITE_DEBUG(request);

      // �T�F�{��������M�A��s��M�ŏI��
      std::ostringstream resbuf;
      while (true) {
         std::string line =  exe->ReadLine();
         resbuf << line << "\r\n";
         if (line.empty()) break;
         if (exe->eof()) throw Poco::IOException("READ PIPE EOF");
      }
      response = resbuf.str();
      WRITE_DEBUG("<<<<[response]\r\n");
      WRITE_DEBUG(response);
      LINE_DEBUG("..done\r\n");
      exe->ClearStderr();
      return true;
   } catch (std::exception &ex) {
      // ��O�����������ꍇ�A������exe�������I������
      LINE_DEBUG("ProcessProxy::request:: !!exception!! " ,ex.what());
      CreateBatRequestResponse(response ,ex.what());
      exe->ClearStderr();
      exe->Kill();
      delete exe;
      exe =NULL;
      return false;
   }
}

// EOFto