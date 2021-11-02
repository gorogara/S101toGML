#include "stdafx.h"
#include "DRDirectoryWriter.h"

DRDirectoryWriter::DRDirectoryWriter()
{

}

DRDirectoryWriter::DRDirectoryWriter(DRReader leader, DRDirectory& dir)
{
	this->lengthCipher = leader.m_fieldLength;
	this->posCipher = leader.m_fieldPosition;
	this->tag = dir.tag;
	this->length = dir.length;
	this->pos = dir.pos;

	int i = 0;
	CString str;


	str.Format(TEXT("%09d"), this->length);
	for (i = 0; i < this->lengthCipher; i++)
	{
		b_length[i] = (BYTE)str[9 - this->lengthCipher + i];
	}
	str.Format(TEXT("%09d"), this->pos);
	for (i = 0; i < this->posCipher; i++)
	{
		b_pos[i] = (BYTE)str[9 - this->posCipher + i];
	}
}

DRDirectoryWriter::~DRDirectoryWriter()
{

}