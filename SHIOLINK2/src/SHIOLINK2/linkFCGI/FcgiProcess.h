#pragma once

class CFcgiProcess
{
private:
	CAtlFile mStdin;
	CAtlFile mStdout;
	CAtlFile mStderr;

public:
	CFcgiProcess(LPCTSTR commandline, LPCTSTR wrkdir);
	~CFcgiProcess(void);
};
