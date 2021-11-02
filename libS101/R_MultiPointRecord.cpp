#include "pch.h"
#include "R_MultiPointRecord.h"
#include "DRDirectoryInfo.h"
#include "F_C2IL.h"
#include "F_COCC.h"
#include "F_C3IL.h"
#include "F_INAS.h"
#include "IC2D.h"
#include "C3IL.h"
#include "DRDirectory.h"
#include "DRReaderWriter.h"
#include "DRDirectoryInfoWriter.h"

//#include"..\\DLL_MessageProcess\\DLL_MessageProcess.h"

R_MultiPointRecord::R_MultiPointRecord(void)
{
}


R_MultiPointRecord::~R_MultiPointRecord(void)
{
	delete m_cocc;

	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		delete *itor;
	}

	for (auto itor = m_c3il.begin(); itor != m_c3il.end(); itor++)
	{
		delete *itor;
	}

	for (auto itor = m_c2il.begin(); itor != m_c2il.end(); itor++)
	{
		delete *itor;
	}
}

#pragma warning(disable:4018)
BOOL R_MultiPointRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0, j = 0, cnt;
	for(unsigned i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"MRID"))
		{
			m_mrid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS *inas = new F_INAS();
			inas->ReadField(buf);
			m_inas.push_back(inas);

			OutputDebugString(L"@@@ INAS @@@ \n\n");
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"COCC"))
		{
			if (!m_cocc)
			{
				m_cocc = new F_COCC();
			}

			cnt = (dir->GetDirectory(i)->length - 1) / F_INAS::GetSize();
			m_cocc->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C2IL"))
		{
			F_C2IL* c2il = new F_C2IL();
			cnt = (dir->GetDirectory(i)->length - 1) / IC2D::GetSize();
			c2il->ReadField(buf, cnt);
			m_c2il.push_back(c2il);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"C3IL"))
		{
			F_C3IL* c3il = new F_C3IL();
			cnt = (dir->GetDirectory(i)->length - 1) / C3IL::GetSize();
			c3il->ReadField(buf, cnt);
			m_c3il.push_back(c3il);
		}
		else
		{
			buf += dir->GetDirectory(i)->length;
		}
		
		if (*(buf++) != 0x1E)
		{
			//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_ERROR, L"terminator error", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::DataSet);
		}
	}
	return TRUE;
}

BOOL R_MultiPointRecord::Save(CFile *file)
{
	DRReaderWriter dr;
	DRDirectoryInfoWriter dirInfo;
	DRDirectory *dir;
	unsigned rLen = 0;
	int index = 0;
	int fieldCnt = 0;
	
	fieldCnt++;
	if (m_cocc)	{ fieldCnt++; }
	fieldCnt += (int)m_inas.size();
	fieldCnt += (int)m_c2il.size();
	fieldCnt += (int)m_c3il.size();

	dirInfo.ReAllocateDirectory(fieldCnt);

	fieldCnt = 0;
	rLen = 0;
	//if (m_mrid)
	{
		dir = dirInfo.GetDirectory(index++);
		dir->tag = *((unsigned int*)"MRID");
		dir->pos = rLen;
		dir->length = m_mrid.GetFieldLength();
		rLen += dir->length;
	}
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		F_INAS* inas = *itor;
		dir = dirInfo.GetDirectory(index++);
		dir->tag = *((unsigned int*)"INAS");
		dir->pos = rLen;
		dir->length = inas->GetFieldLength();
		rLen += dir->length;
	}
	if (m_cocc)
		//if(m_inas.m_name.RCNM != 0)
	{
		dir = dirInfo.GetDirectory(index++);
		dir->tag = *((unsigned int*)"COCC");
		dir->pos = rLen;
		dir->length = m_cocc->GetFieldLength();
		rLen += dir->length;
	}

	for (auto itor = m_c2il.begin(); itor != m_c2il.end(); itor++)
	{
		F_C2IL* c2il = *itor;
		dir = dirInfo.GetDirectory(index++);
		dir->tag = *((unsigned int*)"C2IL");
		dir->pos = rLen;
		//dir->length = m_c2di.GetFieldLength();
		dir->length = c2il->GetFieldLength();
		rLen += dir->length;
	}
	for (auto itor = m_c3il.begin(); itor != m_c3il.end(); itor++)
	{
		F_C3IL* c3il = *itor;
		dir = dirInfo.GetDirectory(index++);
		dir->tag = *((unsigned int*)"C3IL");
		dir->pos = rLen;
		//dir->length = m_c3di.GetFieldLength();
		dir->length = c3il->GetFieldLength();
		rLen += dir->length;
	}
	//else if(m_c2fl)
	////for (auto itor = m_c2flArr.begin(); itor != m_c2flArr.end(); itor++)
	//{
	//	dir = dirInfo.GetDirectory(index++);
	//	dir->tag = *((unsigned int*)"C2FL");
	//	dir->pos = rLen;
	//	//dir->length = m_c2di.GetFieldLength();
	//	dir->length = m_c2fl->GetFieldLength();
	//	rLen += dir->length;
	//}
	//else if(m_c3fl)
	////for (auto itor = m_c3flArr.begin(); itor != m_c3flArr.end(); itor++)
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

	//if (m_mrid)
	{
		m_mrid.Save(file);
	}
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		(*itor)->Save(file);
	}
	if (m_cocc)
	{
		m_cocc->Save(file);
	}
	for (auto itor = m_c2il.begin(); itor != m_c2il.end(); itor++)
	{
		(*itor)->Save(file);
	}
	for (auto itor = m_c3il.begin(); itor != m_c3il.end(); itor++)
	{
		(*itor)->Save(file);
	}
	return TRUE;
}

int R_MultiPointRecord::GetRCID()
{
	return m_mrid.m_name.RCID;
}

std::wstring R_MultiPointRecord::GetRCIDasWstring()
{
	return std::to_wstring(GetRCID());
}