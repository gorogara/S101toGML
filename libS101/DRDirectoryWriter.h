#pragma once

#include "DRDirectory.h"
#include "DRReader.h"

#include <atlstr.h>

struct DRDirectoryWriter : public DRDirectory
{
public:
	DRDirectoryWriter();
	DRDirectoryWriter(DRReader leader, DRDirectory& dir);
	virtual ~DRDirectoryWriter();

public:
	int lengthCipher;
	int posCipher;

	BYTE b_length[9];
	BYTE b_pos[9];
};
