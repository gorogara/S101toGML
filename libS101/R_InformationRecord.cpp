#include "StdAfx.h"
#include "R_InformationRecord.h"
#include "DRDirectoryInfo.h"
#include "F_INAS.h"
#include "ATTR.h"
#include "DRReaderWriter.h"
#include "DRDirectoryInfoWriter.h"

R_InformationRecord::R_InformationRecord(void)
{
}


R_InformationRecord::~R_InformationRecord(void)
{
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}

	m_inas.clear();

	for (auto itor = m_attr.begin(); itor != m_attr.end(); itor++)
	{
		delete *itor;
	}

	m_attr.clear();
}

bool R_InformationRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0, j = 0, cnt;
	for(i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"IRID"))
		{
			m_irid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS *inas = new F_INAS();
			//cnt = (dir->GetDirectory(i)->length - 1) / F_INAS::size;
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"ATTR"))
		{
			cnt = 0;
			j = 0;
			BYTE *sIndex = buf;

			int head = 0;
			for (j = 1; j < dir->GetDirectory(i)->length; j++)
			{
				if (head == 0)
				{
					j += ATTR::GetOffsetToATVL() - 1;
					buf += ATTR::GetOffsetToATVL();
					head = 1;
				}
				else if (*(buf++) == 0x1F)
				{
					++cnt;
					head = 0;
				}
			}
			buf = sIndex;

			F_ATTR* attr = new F_ATTR();
			attr->ReadField(buf, cnt);
			m_attr.push_back(attr);
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

bool R_InformationRecord::Save(CFile *file)
{
	DRReaderWriter dr;
	DRDirectoryInfoWriter dirInfo;
	DRDirectory *dir;
	int rLen = 0;
	
	int fieldCnt = 0;

	fieldCnt++;
	fieldCnt += (int)m_inas.size();
	fieldCnt += (int)m_attr.size();
	
	dirInfo.ReAllocateDirectory(fieldCnt);
	
	rLen = 0;
	fieldCnt = 0;

	//if(m_irid.m_name.RCNM != 0)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"IRID");
		dir->pos = rLen;
		dir->length = m_irid.GetFieldLength();
		rLen += dir->length;
	}

	for (auto itor = m_attr.begin(); itor != m_attr.end(); itor++)
	{
		F_ATTR* attr = *itor;
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"ATTR");
		dir->pos = rLen;
		dir->length = attr->GetFieldLength();
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

	dirInfo.CheckLength(&dr);
	dr.m_fieldAreaLoc = DRReader::size + (4 + dr.m_fieldLength + dr.m_fieldPosition) * dirInfo.m_count + 1;
	dr.m_recordLength = dr.m_fieldAreaLoc + rLen;

	//////////////////////////////////////////////
	dr.WriteDRReader(file);
	dirInfo.WriteDRDirectory(file, dr);


	if(m_irid.m_name.RCNM != 0)
	{
		m_irid.Save(file);
	}
	for (auto itor = m_attr.begin(); itor != m_attr.end(); itor++)
	{
		F_ATTR* attr = *itor;
		attr->Save(file);
	}
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		F_INAS* inas = *itor;
		inas->Save(file);
	}
	
	return TRUE;
}

int R_InformationRecord::GetRCID()
{
	return m_irid.m_name.RCID;
}

int R_InformationRecord::GetAttributeIndex(ATTR* value)
{
	int result = 1;

	for (auto i = m_attr.begin(); i != m_attr.end(); i++)
	{
		auto fATTR = *i;
		for (auto j = fATTR->m_arr.begin(); j != fATTR->m_arr.end(); j++)
		{
			auto attr = *j;
			if (attr == value)
			{
				return result;
			}
			else
			{
				result++;
			}
		}
	}

	return result;

}

int R_InformationRecord::GetAssociationCount()
{
	return 0;
}

int R_InformationRecord::GetInformationAssociationCount()
{
	return m_inas.size();
}

int R_InformationRecord::GetNumericCode()
{
	return m_irid.m_nitc;
}