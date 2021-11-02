#pragma once
#include "DRDirectoryInfo.h"
#include "NonPrintableCharacter.h"

#undef _WINDOWS_
#include <afxext.h>
class DRDirectoryInfoWriter : public DRDirectoryInfo
{
public:
	DRDirectoryInfoWriter();
	DRDirectoryInfoWriter(int directoryCount);
	virtual ~DRDirectoryInfoWriter();

public:
	BOOL WriteDRDirectory(CFile* file, const DRReader dr);
	void CheckLength(DRReader *dr);
};