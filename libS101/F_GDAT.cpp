#include "stdafx.h"
#include "F_GDAT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_GDAT::F_GDAT()
{	/*
	* Datum Name
	* A
	* Name of the geodetic datum
	*/
	m_dtnm = L"";
	/*
	* Ellipsoid Name
	* A
	* Name of the ellipsoid
	*/
	m_elnm = L"";
	/*
	* Ellipsoid semi major axis
	* b48
	* Semi major axis of the ellipsoid in metre
	*/
	m_esma = 0;
	/*
	* Ellipsoid second parameter type
	* b11
	* {1} - Semi minor axis in metres
	* {2} - Inverse Flattening
	*/
	m_espt = 1;
	/*
	* Ellipsoid second parameter
	* b48
	* The second defining parameter of the ellipsoid
	*/
	m_espm = 0;
	/*
	* Central Meridian Name
	* A
	* Name of the central meridian
	*/
	m_cmnm = L"";
	/*
	* Central Meridian Greenwich Longitude
	* b48
	* Greenwich longitude of the central meridian in degrees
	*/
	m_cmgl = 0;
}


F_GDAT::~F_GDAT()
{
}

void F_GDAT::ReadField(BYTE *&buf)
{
	buf2charArr(m_dtnm, buf);
	buf2charArr(m_elnm, buf);
	m_esma = buf2double(buf, 8);
	m_espt = buf2uint(buf, 1);
	m_espm = buf2double(buf, 8);
	buf2charArr(m_cmnm, buf);
	m_cmgl = buf2double(buf, 8);
}

BOOL F_GDAT::Save(CFile *file)
{
	CT2CA outputString(m_dtnm, CP_UTF8);
	file->Write(outputString, (UINT)::strlen(outputString));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputString2(m_elnm, CP_UTF8);
	file->Write(outputString2, (UINT)::strlen(outputString2));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	file->Write(&m_esma, 8);
	file->Write(&m_espt, 1);
	file->Write(&m_espm, 8);

	CT2CA outputString3(m_cmnm, CP_UTF8);
	file->Write(outputString3, (UINT)::strlen(outputString3));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	file->Write(&m_cmgl, 8);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_GDAT::GetFieldLength()
{
	int len = 0;

	len += m_dtnm.GetLength() + 1;
	len += m_elnm.GetLength() + 1;
	len += 8;
	len += 1;
	len += 8;
	len += m_cmnm.GetLength() + 1;
	len += 8;

	return ++len;
}