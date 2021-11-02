#include "pch.h"
#include "R_CompositeRecord.h"
#include "DRDirectoryInfo.h"
#include "DRDirectory.h"
#include "F_CCOC.h"
#include "F_CUCO.h"
#include "F_INAS.h"
#include "DRReaderWriter.h"
#include "DRDirectoryInfoWriter.h"

R_CompositeRecord::R_CompositeRecord(void)
{
	m_ccid.m_name.RCNM = 125;
}


R_CompositeRecord::~R_CompositeRecord(void)
{
	delete m_ccoc;

	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}

	for (auto itor = m_cuco.begin(); itor != m_cuco.end(); itor++)
	{
		delete *itor;
	}
}


BOOL R_CompositeRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0, j = 0;
	for(i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"CCID"))
		{
			m_ccid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS* inas = new F_INAS();

			inas->ReadField(buf);

			m_inas.push_back(inas);

			OutputDebugString(L"@@@ INAS @@@ \n\n");
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"COCC"))
		{
			if (nullptr == m_ccoc)
			{
				m_ccoc = new F_CCOC();
			}

			m_ccoc->ReadField(buf);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"CUCO"))
		{
			F_CUCO* cuco = new F_CUCO();
			cuco->ReadField(buf);
			m_cuco.push_back(cuco);
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

BOOL R_CompositeRecord::Save(CFile *file)
{
	DRReaderWriter dr;
	DRDirectoryInfoWriter dirInfo;
	DRDirectory *dir;
	unsigned rLen = 0;
	int fieldCnt = 0;
	//if(m_ccid)
	{
		fieldCnt++;
	}
	fieldCnt += (int)m_inas.size();
	if (m_ccoc) 		fieldCnt++;
	fieldCnt += (int)m_cuco.size();

	dirInfo.ReAllocateDirectory(fieldCnt);
		
	rLen = 0;
	fieldCnt = 0;
	//if(m_ccid)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"CCID");
		dir->pos = rLen;
		dir->length = m_ccid.GetFieldLength();
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
	if (m_ccoc)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"CCOC");
		dir->pos = rLen;
		dir->length = m_ccoc->GetFieldLength();
		rLen += dir->length;
	}
	for (auto itor = m_cuco.begin(); itor != m_cuco.end(); itor++)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"CUCO");
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

	//if (m_ccid) 
	{ m_ccid.Save(file); }
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		(*itor)->Save(file);
	}
	if (m_ccoc) { m_ccoc->Save(file); }
	for (auto itor = m_cuco.begin(); itor != m_cuco.end(); itor++)
	{
		(*itor)->Save(file);
	}
	return TRUE;
}

int R_CompositeRecord::GetRCID()
{
	return m_ccid.m_name.RCID;
}

std::wstring R_CompositeRecord::GetRCIDasWstring() 
{
	return std::to_wstring(GetRCID());
}