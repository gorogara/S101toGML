#include "pch.h"
#include "F_PTAS.h"
#include "PTAS.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_PTAS::F_PTAS()
{
}

F_PTAS::~F_PTAS()
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		auto ptas = *i;
		delete ptas;
	}

	//POSITION pos = m_arr.GetHeadPosition();

	//while(pos!=NULL)
	//{
	//	PTAS *ptas = m_arr.GetNext(pos);
	//	delete ptas;
	//}
}

void F_PTAS::ReadField(BYTE *&buf)
{
	while(*buf != 0x1E){
		PTAS *ptas = new PTAS();
		ptas->m_name.RCNM = *(buf++);
		ptas->m_name.RCID = buf2uint(buf, 4);
		ptas->m_topi = *(buf++);

		m_arr.push_back(ptas);
	}
}

void F_PTAS::ReadField(BYTE *&buf, int loopCnt)
{
	for(int i = 0; i < loopCnt; i++)
	{
		PTAS *ptas = new PTAS();
		ptas->m_name.RCNM = *(buf++);
		ptas->m_name.RCID = buf2uint(buf, 4);
		ptas->m_topi = *(buf++);

		m_arr.push_back(ptas);
	}
}
BOOL F_PTAS::Save(CFile *file)
{	
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		auto ptas = *i;

		file->Write(&ptas->m_name.RCNM, 1);
		file->Write(&ptas->m_name.RCID, 4);
		file->Write(&ptas->m_topi, 1);
	}

	//POSITION pos = m_arr.GetHeadPosition();

	//while(pos != NULL)
	//{
	//	PTAS *ptas = m_arr.GetNext(pos);
	//	
	//	file->Write(&ptas->m_name.RCNM, 1);
	//	file->Write(&ptas->m_name.RCID, 4);
	//	file->Write(&ptas->m_topi, 1);
	//}
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return TRUE;
}
int F_PTAS::GetFieldLength()
{
	int len = 0;

	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		auto ptas = *i;
		len += ptas->GetSize();
	}

	//POSITION pos = m_arr.GetHeadPosition();
	//while(pos!=NULL)
	//{
	//	PTAS *ptas = m_arr.GetNext(pos);
	//	len += ptas->GetSize();
	//}
		
	return ++len;
}
