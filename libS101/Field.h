#pragma once
#undef _WINDOWS_
#include <afxext.h>

class Field
{
public:
	Field();
	virtual ~Field();

public:
	virtual void ReadField(BYTE *&buf) {};
	virtual BOOL Save(CFile *file) { return TRUE; };
	virtual int GetFieldLength() { return 0; };
};