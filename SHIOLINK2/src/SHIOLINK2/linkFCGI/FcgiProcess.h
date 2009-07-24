#pragma once

class CFcgiProcess
{
private:
	CHandle mWritePipe;
	CHandle mReadPipe;
	CHandle mErrPipe;

	STARTUPINFO mStartInfo;
	PROCESS_INFORMATION mProcessInfo;

public:
	CFcgiProcess(LPCTSTR str_commandline, LPCTSTR str_wrkdir);
	~CFcgiProcess(void);
};
