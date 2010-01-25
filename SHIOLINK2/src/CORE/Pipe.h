#pragma once

class CServerPipe
{
private:
	CString mID;
	CPath   mBaseName;
	CHandle mReqPipe;
	CHandle mResPipe;

public:
	CServerPipe(void);
	virtual ~CServerPipe(void);
	void Close(void);

	LPCTSTR GetID(void) const;
	LPCTSTR GetBaseName(void) const;
	CString GetReqName(void) const;
	CString GetResName(void) const;

	void Create(void);

	void Read (      LPBYTE buf, DWORD length);
	void Write(const LPBYTE buf, DWORD length);


private:
	bool TryCreate(void);

};
