#pragma once

class CPipe
{
private:
	CPath mPipeName;
	CHandle mWritePipe;
	CHandle mReadPipe;

public:
	CPipe(void);
	virtual ~CPipe(void);

	void Create(void);

	LPCTSTR GetName(void) const;
};
