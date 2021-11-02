#include "pch.h"
#include "F_PROJ.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_PROJ::F_PROJ()
{
	m_prom = 0;
	m_prp1 = 0;
	m_prp2 = 0;
	m_prp3 = 0;
	m_prp4 = 0;
	m_prp5 = 0;
	m_feas = 0;
	m_fnor = 0;
}


F_PROJ::~F_PROJ()
{
}

void F_PROJ::ReadField(BYTE *&buf)
{
	m_prom = *(buf++);
	m_prp1 = buf2double(buf, 8);
	m_prp2 = buf2double(buf, 8);
	m_prp3 = buf2double(buf, 8);
	m_prp4 = buf2double(buf, 8);
	m_prp5 = buf2double(buf, 8);
	m_feas = buf2double(buf, 8);
	m_fnor = buf2double(buf, 8);
}

void F_PROJ::ReadField(BYTE *&buf, int loopCnt)
{
	m_prom = *(buf++);
	m_prp1 = buf2double(buf, 8);
	m_prp2 = buf2double(buf, 8);
	m_prp3 = buf2double(buf, 8);
	m_prp4 = buf2double(buf, 8);
	m_prp5 = buf2double(buf, 8);
	m_feas = buf2double(buf, 8);
	m_fnor = buf2double(buf, 8);
}
BOOL F_PROJ::Save(CFile *file)
{
	file->Write(&m_prom, 1);
	file->Write(&m_prp1, 8);
	file->Write(&m_prp2, 8);
	file->Write(&m_prp3, 8);
	file->Write(&m_prp4, 8);
	file->Write(&m_prp5, 8);
	file->Write(&m_feas, 8);
	file->Write(&m_fnor, 8);
	
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return TRUE;
}


int F_PROJ::GetFieldLength()
{
	int len = 0;
	len += F_PROJ::GetSize();
	return ++len;
}

int F_PROJ::GetSize()
{
	return 57;
}
