#pragma once

class CPipe
{
protected:
	CString mID;
	CPath   mBaseName;
	CHandle mWrite;
	CHandle mRead;

public:
	CPipe(void);
	virtual ~CPipe(void);
	void Close(void);

public:
	LPCTSTR GetID(void) const;
	LPCTSTR GetBaseName(void) const;
	CString GetReqName(void) const;
	CString GetResName(void) const;

	void Write(const LPBYTE buf, DWORD length);
	void Read (      LPBYTE buf, DWORD length);

protected:
	void SetID(LPCTSTR id);
};

class CServerPipe : public CPipe
{
public:
	CServerPipe(void);
	virtual ~CServerPipe(void);

public:
	void Create(void);
private:
	bool TryCreate(void);
};

class CClientPipe : public CPipe
{
public:
	CClientPipe(LPCTSTR id);
	virtual ~CClientPipe(void);

public:
	void Create(LPCTSTR id);
};
