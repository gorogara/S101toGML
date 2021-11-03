#include "pch.h"
#include "R_CurveRecord.h"
#include "F_PTAS.h"
#include "F_SEGH.h"
#include "F_SECC.h"
#include "F_COCC.h"
#include "F_C2IL.h"
#include "F_INAS.h"
#include "IC2D.h"
#include "PTAS.h"
#include "DRDirectory.h"
#include "DRDirectoryInfo.h"
#include "DRReaderWriter.h"
#include "DRDirectoryInfoWriter.h"

//#include"..\\DLL_MessageProcess\\DLL_MessageProcess.h"

R_CurveRecord::R_CurveRecord()
{
	m_crid.m_name.RCNM = 120;
}

R_CurveRecord::~R_CurveRecord()
{
	delete m_ptas;
	m_ptas = nullptr;

	delete m_secc;
	m_secc = nullptr;

	delete m_cocc;
	m_cocc = nullptr;

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_inas.clear();

	for (auto i = m_segh.begin(); i != m_segh.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_segh.clear();

	for (auto i = m_c2il.begin(); i != m_c2il.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_c2il.clear();
}

#pragma warning(disable:4018)
BOOL R_CurveRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	for (int i = 0; i < dir->m_count; i++)
	{
		if (dir->GetDirectory(i)->tag == *((unsigned int*)"CRID"))
		{
			m_crid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS* inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);

			OutputDebugString(L"@@@ INAS @@@ \n\n");
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"PTAS"))
		{
			if (nullptr == m_ptas)
			{
				m_ptas = new F_PTAS();
			}

			auto cnt = (dir->GetDirectory(i)->length  - 1) / PTAS::GetSize();

			m_ptas->ReadField(buf, cnt);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"SECC"))
		{
			if (nullptr == m_secc)
			{
				m_secc = new F_SECC();
			}

			m_secc->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"SEGH"))
		{
			F_SEGH* segh = new F_SEGH();

			segh->ReadField(buf);

			m_segh.push_back(segh);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"COCC"))
		{
			if (nullptr == m_cocc)
			{
				m_cocc = new F_COCC();
			}

			m_cocc->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C2IL"))
		{
			F_C2IL *c2il = new F_C2IL();

			auto cnt = (dir->GetDirectory(i)->length - 1) / IC2D::GetSize();

			c2il->ReadField(buf, cnt);

			m_c2il.push_back(c2il);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}

		if (*(buf++) != 0x1E)
		{
			//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_ERROR, L"terminator error",KRS_MSG_PROCESS::User_Developer_Mode,KRS_MSG_PROCESS::DataSet);
		}
	}

	return true;
}

BOOL R_CurveRecord::Save(CFile *file)
{
	DRReaderWriter dr;
	DRDirectoryInfoWriter dirInfo;
	DRDirectory *dir;
	unsigned rLen = 0;
	int fieldCnt = 0;
	//if(m_crid)
	fieldCnt++;
	fieldCnt += (int)m_inas.size();
	if(m_ptas) 		fieldCnt++;
	fieldCnt += (int)m_segh.size();
	if (m_cocc)		fieldCnt++;
	fieldCnt += (int)m_c2il.size();
	
	dirInfo.ReAllocateDirectory(fieldCnt);
		
	rLen = 0;
	fieldCnt = 0;
	//if(m_crid)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"CRID");
		dir->pos = rLen;
		dir->length = m_crid.GetFieldLength();
		rLen += dir->length;
	}
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"INAS");
		dir->pos = rLen;
		dir->length = (*itor)->GetFieldLength();
		rLen += dir->length;
	}
	if(m_ptas)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"PTAS");
		dir->pos = rLen;
		dir->length = m_ptas->GetFieldLength();
		rLen += dir->length;
	}
	for (auto itor = m_segh.begin(); itor != m_segh.end(); itor++)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"SEGH");
		dir->pos = rLen;
		dir->length = (*itor)->GetFieldLength();
		rLen += dir->length;
	}
	if(m_cocc)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"COCC");
		dir->pos = rLen;
		dir->length = m_cocc->GetFieldLength();
		rLen += dir->length;
	}
	for (auto itor = m_c2il.begin(); itor != m_c2il.end(); itor++)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"C2IL");
		dir->pos = rLen;
		//dir->length = m_c3di.GetFieldLength();
		dir->length = (*itor)->GetFieldLength();
		rLen += dir->length;
	}
	//else if (m_c3il)
	//{
	//	dir = dirInfo.GetDirectory(index++);
	//	dir->tag = *((unsigned int*)"C3IL");
	//	dir->pos = rLen;
	//	//dir->length = m_c3di.GetFieldLength();
	//	dir->length = m_c3il->GetFieldLength();
	//	rLen += dir->length;
	//}
	//else if (m_c2fl)
	//{
	//	dir = dirInfo.GetDirectory(index++);
	//	dir->tag = *((unsigned int*)"C2FL");
	//	dir->pos = rLen;
	//	//dir->length = m_c3di.GetFieldLength();
	//	dir->length = m_c2fl->GetFieldLength();
	//	rLen += dir->length;
	//}
	//else if (m_c3fl)
	//{
	//	dir = dirInfo.GetDirectory(index++);
	//	dir->tag = *((unsigned int*)"C3FL");
	//	dir->pos = rLen;
	//	//dir->length = m_c3di.GetFieldLength();
	//	dir->length = m_c3fl->GetFieldLength();
	//	rLen += dir->length;
	//}


	dirInfo.CheckLength(&dr);
	dr.m_fieldAreaLoc = DRReader::size + (4 + dr.m_fieldLength + dr.m_fieldPosition) * dirInfo.m_count + 1;
	dr.m_recordLength = dr.m_fieldAreaLoc + rLen;

	//////////////////////////////////////////////
	dr.WriteDRReader(file);
	dirInfo.WriteDRDirectory(file, dr);

	//if (m_crid) 
	m_crid.Save(file);
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		(*itor)->Save(file);
	}
	if (m_ptas) m_ptas->Save(file);
	if (m_secc) m_secc->Save(file);
	for (auto itor = m_segh.begin(); itor != m_segh.end(); itor++)
	{
		(*itor)->Save(file);
	}
	if (m_cocc) m_cocc->Save(file);
	for (auto itor = m_c2il.begin(); itor != m_c2il.end(); itor++)
	{
		(*itor)->Save(file);
	}
	//else if(m_c3di.m_arr.GetCount() > 0)
	//else if (m_c3il)
	//{
	//	m_c3il->Save(file);
	//}
	////if(m_c2di.m_arr.GetCount() > 0)
	//else if (m_c2fl)
	//{
	//	m_c2fl->Save(file);
	//}
	////else if(m_c3di.m_arr.GetCount() > 0)
	//else if (m_c3fl)
	//{
	//	m_c3fl->Save(file);
	//}

	return TRUE;
}

int R_CurveRecord::GetRCID() 
{
	return m_crid.m_name.RCID;
}

std::wstring R_CurveRecord::GetRCIDasWstring() 
{
	return std::to_wstring(GetRCID());
}