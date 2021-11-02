#include "pch.h"
#include "DRReaderWriter.h"

#undef _WINDOWS_
#include <afxext.h>

DRReaderWriter::DRReaderWriter(void)
{
	b_recordLength[0]		= '0';
	b_recordLength[1]		= '0';
	b_recordLength[2]		= '0';
	b_recordLength[3]		= '0';
	b_recordLength[4]		= '0';
	b_interchangeLevel	= ' ';
	b_leaderIdentifier	= 'D';
	b_inLineCodeExtensionIndicator	= ' ';
	b_versionNumber		=' ';
	b_applicationIndicator = ' ';
	b_fieldControlLength[0] = ' ';
	b_fieldControlLength[1] = ' ';
	b_baseAddressOfFieldArea[0] = '0';
	b_baseAddressOfFieldArea[1] = '0';
	b_baseAddressOfFieldArea[2] = '0';
	b_baseAddressOfFieldArea[3] = '0';
	b_baseAddressOfFieldArea[4] = '0';

	b_extendedCharacterSetIndicator[0] = ' ';
	b_extendedCharacterSetIndicator[1] = ' ';
	b_extendedCharacterSetIndicator[2] = ' ';

	b_fieldAreaLoc = '0';
	b_fieldLength = '0';
	b_Reserved = '0';
	b_fieldPosition = '4';	

	m_recordLength = 0;
	m_fieldAreaLoc = 0;
	m_fieldLength = 0;
	m_fieldPosition = 0;
}


DRReaderWriter::~DRReaderWriter(void)
{
}



BOOL DRReaderWriter::WriteDRReader(CFile* file)
{
	SetByteInfo();

	//RP	Len Entry name Content
	//0	5	Record length number of bytes in record
	//5	1	Interchange level SPACE
	//6	1	Leader identifier "D"
	//7	1	In line code extension indicator SPACE
	//8	1	Version number SPACE
	//9	1	Application indicator SPACE
	//10	2	Field control length 2 SPACEs
	//12	5	Base address of field area Start address of field area (number of bytes in leader and directory)
	//17	3	Extended character set indicator 3 SPACEs
	//20	4	Entry map (see table A.4) 
	
	file->Write(b_recordLength, 5);
	file->Write(&b_interchangeLevel, 1);
	file->Write(&b_leaderIdentifier, 1);
	file->Write(&b_inLineCodeExtensionIndicator, 1);
	file->Write(&b_versionNumber, 1);
	file->Write(&b_applicationIndicator, 1);
	file->Write(b_fieldControlLength, 2);
	file->Write(b_baseAddressOfFieldArea, 5);
	file->Write(b_extendedCharacterSetIndicator, 3);
	file->Write(&b_fieldAreaLoc, 1);
	file->Write(&b_fieldLength, 1);
	file->Write(&b_Reserved, 1);
	file->Write(&b_fieldPosition, 1);
	return TRUE;
}

void DRReaderWriter::SetByteInfo()
{
	CString str;
	int i = 0;
	
	str.Format(TEXT("%05d"), m_recordLength);
	for(i = 0; i < 5; i++)
	{
		b_recordLength[i] = (BYTE)str[i];
	}

	str.Format(TEXT("%05d"), m_fieldAreaLoc);
	for(i = 0; i < 5; i++)
	{
		b_baseAddressOfFieldArea[i] = (BYTE)str[i];
	}

	b_fieldAreaLoc = '0' + m_fieldLength;
	b_fieldLength = '0' + m_fieldPosition;
}
