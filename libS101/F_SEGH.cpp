#include "pch.h"
#include "F_SEGH.h"
#include "NonPrintableCharacter.h"

F_SEGH::F_SEGH(void)
{
	m_intp = 0;
	m_circ = 0;
	m_ycoo = 0;
	m_xcoo = 0;
	m_dist = 0;
	m_disu = 0;
	m_sbrg = 0;
	m_angl = 0;
}

F_SEGH::~F_SEGH(void)
{
}
void F_SEGH::ReadField(BYTE *&buf)
{
	m_intp = *(buf++);
	//m_circ = *(buf++);
	//m_ycoo = buf2double(buf, 8);
	//m_xcoo = buf2double(buf, 8);
	//m_dist = buf2double(buf, 8);
	//m_disu = *(buf++);
	//m_sbrg = buf2double(buf, 8);
	//m_angl = buf2double(buf, 8);
}
BOOL F_SEGH::Save(CFile *file)
{
	file->Write(&m_intp, 1);
	//file->Write(&m_circ, 1);
	//file->Write(&m_ycoo, 8);
	//file->Write(&m_xcoo, 8);
	//file->Write(&m_dist, 8);
	//file->Write(&m_disu, 1);
	//file->Write(&m_sbrg, 8);
	//file->Write(&m_angl, 8);
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}
int F_SEGH::GetFieldLength()
{
	int len = 0;
	len += 1;
	return ++len;
}