#include "StdAfx.h"
#include "R_PointRecord.h"
#include "DRDirectoryInfo.h"
#include "F_C2IT.h"
#include "F_C3IT.h"
#include "F_INAS.h"
#include "DRReaderWriter.h"
#include "DRDirectoryInfoWriter.h"

R_PointRecord::R_PointRecord(void)
{
	m_prid.m_name.RCNM = 110;
}

R_PointRecord::~R_PointRecord(void)
{
	delete m_c2it;
	delete m_c3it;
	//if (m_c2ft) delete m_c2ft;
	//if (m_c3ft) delete m_c3ft;
	//if (m_inas) delete m_inas;

	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}
}

BOOL R_PointRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{	
	USES_CONVERSION;
	int i = 0, j = 0;
	for(i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"PRID"))
		{
			m_prid.ReadField(buf);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS *inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);

			OutputDebugString(L"@@@ INAS @@@ \n\n");
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C2IT"))
		{
			m_c2it = new F_C2IT();
			m_c2it->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C3IT"))
		{
			m_c3it = new F_C3IT();
			m_c3it->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C2FT"))
		{
			//m_c2ft = new F_C2FT();
			//m_c2ft->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C3FT"))
		{
			//m_c3ft = new F_C3FT();
			//m_c3ft->ReadField(buf);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}
		
		if(*(buf++)!= 0x1E)//{}
			TRACE(W2A(TEXT("terminator error")));
	}

	return TRUE;
}


BOOL R_PointRecord::Save(CFile *file)
{
	DRReaderWriter dr;
	DRDirectoryInfoWriter dirInfo;
	DRDirectory *dir;
	unsigned rLen = 0;
	int fieldCnt = 0;
	if(m_prid.m_name.RCNM != 0)
	{
		fieldCnt++;
	}
	fieldCnt += (int)m_inas.size();
	if (m_c2it) fieldCnt++;
	if (m_c3it) fieldCnt++;

	dirInfo.ReAllocateDirectory(fieldCnt);
		
	rLen = 0;
	fieldCnt = 0;
	if(m_prid.m_name.RCNM != 0)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"PRID");
		dir->pos = rLen;
		dir->length = m_prid.GetFieldLength();
		rLen += dir->length;
	}

	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		F_INAS* inas = *itor;
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"INAS");
		dir->pos = rLen;
		dir->length = inas->GetFieldLength();
		rLen += dir->length;
	}

	//if(m_c2di.m_arr.GetCount() > 0)
	if (m_c2it)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"C2IT");
		dir->pos = rLen;
		//dir->length = m_c2di.GetFieldLength();
		dir->length = m_c2it->GetFieldLength();
		rLen += dir->length;
	}
	//else if(m_c3di.m_arr.GetCount() > 0)
	else if (m_c3it)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"C3IT");
		dir->pos = rLen;
		//dir->length = m_c3di.GetFieldLength();
		dir->length = m_c3it->GetFieldLength();
		rLen += dir->length;
	}

	//if(m_c2di.m_arr.GetCount() > 0)
	//else if (m_c2ft)
	//{
	//	dir = dirInfo.GetDirectory(index++);
	//	dir->tag = *((unsigned int*)"C2FT");
	//	dir->pos = rLen;
	//	//dir->length = m_c2di.GetFieldLength();
	//	dir->length = m_c2ft->GetFieldLength();
	//	rLen += dir->length;
	//}
	////else if(m_c3di.m_arr.GetCount() > 0)
	//else if (m_c3ft)
	//{
	//	dir = dirInfo.GetDirectory(index++);
	//	dir->tag = *((unsigned int*)"C3FT");
	//	dir->pos = rLen;
	//	//dir->length = m_c3di.GetFieldLength();
	//	dir->length = m_c3ft->GetFieldLength();
	//	rLen += dir->length;
	//}

	dirInfo.CheckLength(&dr);
	dr.m_fieldAreaLoc = DRReader::size + (4 + dr.m_fieldLength + dr.m_fieldPosition) * dirInfo.m_count + 1;
	dr.m_recordLength = dr.m_fieldAreaLoc + rLen;

	//////////////////////////////////////////////
	dr.WriteDRReader(file);
	dirInfo.WriteDRDirectory(file, dr);

	if(m_prid.m_name.RCNM != 0)
	{
		m_prid.Save(file);
	}
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		F_INAS* inas = *itor;
		inas->Save(file);
	}
	//if(m_c2di.m_arr.GetCount() > 0)
	if (m_c2it)
	{
		m_c2it->Save(file);
	}
	//else if(m_c3di.m_arr.GetCount() > 0)
	else if (m_c3it)
	{
		m_c3it->Save(file);
	}
	////if(m_c2di.m_arr.GetCount() > 0)
	//else if (m_c2ft)
	//{
	//	m_c2ft->Save(file);
	//}
	////else if(m_c3di.m_arr.GetCount() > 0)
	//else if (m_c3ft)
	//{
	//	m_c3ft->Save(file);
	//}
	return TRUE;
}

int R_PointRecord::GetRCID()
{
	return  m_prid.m_name.RCID;
}

std::wstring R_PointRecord::GetRCIDasWstring() 
{
	return std::to_wstring(GetRCID());
}

void R_PointRecord::SetC2IT(F_C2IT* value)
{
	m_c2it = value;
}

void R_PointRecord::SetC3IT(F_C3IT* value) 
{
	m_c3it = value;
}