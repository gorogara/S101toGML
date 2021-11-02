#include "stdafx.h"
#include "R_DSCRS.h"
#include "DRDirectoryInfo.h"
#include "F_VDAT.h"
#include "F_CRSH.h"
#include "F_CSAX.h"
#include "F_PROJ.h"
#include "F_GDAT.h"
#include "DRReaderWriter.h"
#include "DRDirectoryInfoWriter.h"

R_DSCRS::R_DSCRS()
{
}


R_DSCRS::~R_DSCRS()
{
	for (auto itor = m_crsh.begin(); itor != m_crsh.end(); itor++)
	{
		delete *itor;
	}

	delete m_csax;
	delete m_proj;
	delete m_gdat;
	delete m_vdat;
}

#pragma warning(disable:4244)
#pragma warning(disable:4018)
BOOL R_DSCRS::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	for(unsigned i = 0; i < dir->m_count; i++)
	{
		if(dir->GetDirectory(i)->tag == *((unsigned int*)"CSID"))
		{
			m_csid.ReadField(buf);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"CRSH"))
		{
			F_CRSH *crsh = new F_CRSH();
			crsh->ReadField(buf);
			m_crsh.push_back(crsh);
		}
		else if(dir->GetDirectory(i)->tag == *((unsigned int*)"CSAX"))
		{
			if (!m_csax) m_csax = new F_CSAX();
			m_csax->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"PROJ"))
		{
			if (!m_proj) m_proj = new F_PROJ();
			m_proj->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"GDAT"))
		{
			if (!m_gdat) m_gdat = new F_GDAT();
			m_gdat->ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"VDAT"))
		{
			if (!m_vdat) m_vdat = new F_VDAT();
			m_vdat->ReadField(buf);
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


BOOL R_DSCRS::Save(CFile *file)
{
	DRReaderWriter dr;
	DRDirectoryInfoWriter dirInfo;
	DRDirectory *dir;
	unsigned rLen = 0;

	int dirInfoCnt = 0;

	dirInfoCnt++;
	dirInfoCnt += (int)m_crsh.size();
	if (m_csax) dirInfoCnt++;
	//if (m_proj) dirInfoCnt++;
	//if (m_gdat) dirInfoCnt++;
	if (m_vdat) dirInfoCnt++;

	dirInfo.ReAllocateDirectory(dirInfoCnt);
	
	dirInfoCnt = 0;
	rLen = 0;
	dir = dirInfo.GetDirectory(dirInfoCnt++);
	dir->tag = *((unsigned int*)"CSID");
	dir->pos = rLen;
	dir->length = m_csid.GetFieldLength();
	rLen += dir->length;

	for (auto itor = m_crsh.begin(); itor != m_crsh.end(); itor++)
	{
		F_CRSH *crsh = *itor;
		dir = dirInfo.GetDirectory(dirInfoCnt++);
		dir->tag = *((unsigned int*)"CRSH");
		dir->pos = rLen;
		dir->length = crsh->GetFieldLength();
		rLen += dir->length;
	}

	if (m_csax)
	{
		dir = dirInfo.GetDirectory(dirInfoCnt++);
		dir->tag = *((unsigned int*)"CSAX");
		dir->pos = rLen;
		dir->length = m_csax->GetFieldLength();
		rLen += dir->length;
	}

	//if (m_proj)
	//{
	//	dir = dirInfo.GetDirectory(dirInfoCnt++);
	//	dir->tag = *((unsigned int*)"PROJ");
	//	dir->pos = rLen;
	//	dir->length = m_proj->GetFieldLength();
	//	rLen += dir->length;
	//}

	//if (m_gdat)
	//{
	//	dir = dirInfo.GetDirectory(dirInfoCnt++);
	//	dir->tag = *((unsigned int*)"GDAT");
	//	dir->pos = rLen;
	//	dir->length = m_gdat->GetFieldLength();
	//	rLen += dir->length;
	//}

	if (m_vdat)
	{
		dir = dirInfo.GetDirectory(dirInfoCnt++);
		dir->tag = *((unsigned int*)"VDAT");
		dir->pos = rLen;
		dir->length = m_vdat->GetFieldLength();
		rLen += dir->length;
	}

	dirInfo.CheckLength(&dr);
	dr.m_fieldAreaLoc = DRReader::size + (4 + dr.m_fieldLength + dr.m_fieldPosition) * dirInfo.m_count + 1;
	dr.m_recordLength = dr.m_fieldAreaLoc + rLen;

	//////////////////////////////////////////////
	dr.WriteDRReader(file);
	dirInfo.WriteDRDirectory(file, dr);
	m_csid.Save(file);
	
	for (auto itor = m_crsh.begin(); itor != m_crsh.end(); itor++)
	{
		F_CRSH *crsh = *itor;
		crsh->Save(file);
	}
	if (m_csax) m_csax->Save(file);
	//if (m_proj) m_proj->Save(file);
	//if (m_gdat) m_gdat->Save(file);
	if (m_vdat) m_vdat->Save(file);
	
	return TRUE;
}