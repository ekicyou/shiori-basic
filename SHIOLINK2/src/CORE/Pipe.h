#pragma once

class CPipe
{
private:
	CPath mBaseName;
	CHandle mReqPipe;
	CHandle mResPipe;

public:
	CPipe(void);
	virtual ~CPipe(void);
	void Close(void);

	LPCTSTR GetBaseName(void) const;
	CString GetReqName(void) const;
	CString GetResName(void) const;

	void Create(void);

	void Read (      LPBYTE buf, DWORD length);
	void Write(const LPBYTE buf, DWORD length);


private:
	bool TryCreate(void);

};
