#include "pch.h"

#include "DRDirectoryInfoWriter.h"
#include "DRDirectoryWriter.h"

#undef _WINDOWS_
#include <afxext.h>
DRDirectoryInfoWriter::DRDirectoryInfoWriter(void) 
	: DRDirectoryInfo()
{
}

DRDirectoryInfoWriter::DRDirectoryInfoWriter(int directoryCount)
	: DRDirectoryInfo(directoryCount)
{
}


DRDirectoryInfoWriter::~DRDirectoryInfoWriter(void)
{
}

BOOL DRDirectoryInfoWriter::WriteDRDirectory(CFile* file, const DRReader dr)
{
	for(int i = 0; i < m_count; i++)
	{
		DRDirectoryWriter dirWriter(dr, directories[i]);
		
		file->Write(&dirWriter.tag, 4);
		file->Write(dirWriter.b_length, dr.m_fieldLength);
		file->Write(dirWriter.b_pos, dr.m_fieldPosition);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

void DRDirectoryInfoWriter::CheckLength(DRReader *dr)
{	
	int len = 0;
	int pos = 0;
	for(int i = 0; i < m_count; i++)
	{
		int fLen = 1;
		int fPos = 1;
	
		len = directories[i].length;
		pos += directories[i].pos;
		
		while(len = len / 10)
		{
			fLen++;
		}
		while(pos = pos / 10)
		{
			fPos++;
		}

		dr->m_fieldLength = dr->m_fieldLength > fLen ? dr->m_fieldLength : fLen;
		dr->m_fieldPosition = dr->m_fieldPosition > fPos ? dr->m_fieldPosition : fPos;
	}
}
