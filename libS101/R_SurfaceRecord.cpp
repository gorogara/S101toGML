#include "pch.h"
#include "R_SurfaceRecord.h"
#include "F_INAS.h"
#include "F_RIAS.h"
#include "RIAS.h"
#include "DRDirectoryInfo.h"
#include "DRDirectory.h"
#include "DRReaderWriter.h"
#include "DRDirectoryInfoWriter.h"

R_SurfaceRecord::R_SurfaceRecord(void)
{
	m_srid.m_name.RCNM = 130;
}


R_SurfaceRecord::~R_SurfaceRecord(void)
{
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}
	for (auto itor = m_rias.begin(); itor != m_rias.end(); itor++)
	{
		delete *itor;
	}
}

BOOL R_SurfaceRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	for(int i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"SRID"))
		{
			m_srid.ReadField(buf);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS* inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);

			OutputDebugString(L"@@@ INAS @@@ \n\n");
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"RIAS"))
		{
			F_RIAS* rias = new F_RIAS();
			auto cnt = (dir->GetDirectory(i)->length - 1) / RIAS::GetSize();
			rias->ReadField(buf, cnt);
			m_rias.push_back(rias);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}
		

		if (*(buf++) != 0x1E)
		{
			TRACE(W2A(TEXT("terminator error")));
		}
	}
	return TRUE;
}

BOOL R_SurfaceRecord::Save(CFile *file)
{
	DRReaderWriter dr;
	DRDirectoryInfoWriter dirInfo;
	DRDirectory *dir;
	unsigned rLen = 0;
	int fieldCnt = 0;
	//if(m_srid.m_name.GetName() != 0)
	{
		fieldCnt++;
	}
	fieldCnt += (int)m_inas.size();
	fieldCnt += (int)m_rias.size();

	dirInfo.ReAllocateDirectory(fieldCnt);
		
	rLen = 0;
	fieldCnt = 0;
	//if(m_srid.m_name.GetName() != 0)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"SRID");
		dir->pos = rLen;
		dir->length = m_srid.GetFieldLength();
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
	for (auto itor = m_rias.begin(); itor != m_rias.end(); itor++)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"RIAS");
		dir->pos = rLen;
		dir->length = (*itor)->GetFieldLength();
		rLen += dir->length;
	}

	dirInfo.CheckLength(&dr);
	dr.m_fieldAreaLoc = DRReader::size + (4 + dr.m_fieldLength + dr.m_fieldPosition) * dirInfo.m_count + 1;
	dr.m_recordLength = dr.m_fieldAreaLoc + rLen;

	//////////////////////////////////////////////
	dr.WriteDRReader(file);
	dirInfo.WriteDRDirectory(file, dr);

	if(m_srid.m_name.GetName() != 0)
	{
		m_srid.Save(file);
	}
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		(*itor)->Save(file);
	}
	for (auto itor = m_rias.begin(); itor != m_rias.end(); itor++)
	{
		(*itor)->Save(file);
	}
	return TRUE;
}

int R_SurfaceRecord::GetRCID() 
{
	return m_srid.m_name.RCID;
}

std::wstring R_SurfaceRecord::GetRCIDasWstring() 
{
	return std::to_wstring(GetRCID());
}