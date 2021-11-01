#include "StdAfx.h"
#include "R_FeatureRecord.h"
#include "F_SPAS.h"
#include "F_FASC.h"
#include "F_MASK.h"
#include "F_ATTR.h"
#include "F_INAS.h"
#include "ATTR.h"
#include "FASC.h"
#include "MASK.h"
#include "SPAS.h"
#include "DRReaderWriter.h"
#include "DRDirectoryInfoWriter.h"

#include "..\\S100Geometry\\SGeometry.h"
#include "..\\S100Geometry\\SPoint.h"
#include "..\\S100Geometry\\SCompositeCurve.h"
#include "..\\S100Geometry\\SSurface.h"
#include "..\\S100Geometry\\SCurveHasOrient.h"
#include "..\\S100Geometry\\SCurve.h"

#include "..\\DLL_MessageProcess\\DLL_MessageProcess.h"

R_FeatureRecord::R_FeatureRecord(void)
{
	m_geometry = NULL;

	//m_cs = NULL;
	m_scaleMin = 0;
}

R_FeatureRecord::~R_FeatureRecord(void)
{
	for (auto i = m_attr.begin(); i != m_attr.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_attr.clear();

	for (auto i = m_inas.begin(); i != m_inas.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_inas.clear();

	for (auto i = m_spas.begin(); i != m_spas.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_spas.clear();

	for (auto i = m_fasc.begin(); i != m_fasc.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_fasc.clear();

	for (auto i = m_mask.begin(); i != m_mask.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	m_mask.clear();

	delete m_geometry;
	m_geometry = nullptr;

	//delete m_cs;
	//m_cs = nullptr;

	//	[Text Placement 활용 연구]
	delete m_textBearing;
	m_textBearing = nullptr;
}

#pragma warning(disable:4018)
BOOL R_FeatureRecord::ReadRecord(DRDirectoryInfo *dir, BYTE*& buf)
{
	USES_CONVERSION;
	unsigned i = 0, j = 0, cnt;
	for (i = 0; i < dir->m_count; i++)
	{
		if (dir->GetDirectory(i)->tag == *((unsigned int*)"FRID"))
		{
			m_frid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"FOID"))
		{
			m_foid.ReadField(buf);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"ATTR"))
		{
			cnt = 0;
			j = 0;
			BYTE *sIndex = buf;

			unsigned head = 0;
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
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"INAS"))
		{
			F_INAS *inas = new F_INAS();
			//cnt = (dir->GetDirectory(i)->length - 1) / F_INAS::size;
			inas->ReadField(buf);
			m_inas.push_back(inas);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"SPAS"))
		{
			F_SPAS* spas = new F_SPAS();
			cnt = (dir->GetDirectory(i)->length - 1) / SPAS::GetSize();
			spas->ReadField(buf, cnt);
			m_spas.push_back(spas);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"FASC"))
		{
			F_FASC *fasc = new F_FASC();
			cnt = (dir->GetDirectory(i)->length - 1) / FASC::GetSize();
			fasc->ReadField(buf, cnt);
			m_fasc.push_back(fasc);
		}
		else if (dir->GetDirectory(i)->tag == *((unsigned int*)"MASK"))
		{

			F_MASK* mask = new F_MASK();
			cnt = (dir->GetDirectory(i)->length - 1) / MASK::GetSize();
			mask->ReadField(buf, cnt);
			m_mask.push_back(mask);

		}
		else
		{
			buf += dir->GetDirectory(i)->length;

		}

		if (*(buf++) != 0x1E)//{}
		{
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"terminator error", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::DataSet);
			//TRACE(W2A(TEXT("terminator error")));
		}

	}
	return true;
}

BOOL R_FeatureRecord::Save(CFile *file)
{
	DRReaderWriter dr;
	DRDirectoryInfoWriter dirInfo;
	DRDirectory *dir;
	unsigned rLen = 0;
	int fieldCnt = 0;

	/*if (m_frid) */fieldCnt++;
	/*if (m_foid) */fieldCnt++;
	fieldCnt += (int)m_attr.size();
	fieldCnt += (int)m_inas.size();
	fieldCnt += (int)m_spas.size();
	fieldCnt += (int)m_fasc.size();
	fieldCnt += (int)m_mask.size();
	dirInfo.ReAllocateDirectory(fieldCnt);

	rLen = 0;
	fieldCnt = 0;

	//	if(m_frid)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"FRID");
		dir->pos = rLen;
		dir->length = m_frid.GetFieldLength();
		rLen += dir->length;
	}
	//	if(m_foid)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"FOID");
		dir->pos = rLen;
		dir->length = m_foid.GetFieldLength();
		rLen += dir->length;
	}
	//	if(m_attr.m_arr.GetCount() > 0)

	for (auto itor = m_attr.begin(); itor != m_attr.end(); itor++)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"ATTR");
		dir->pos = rLen;
		dir->length = (*itor)->GetFieldLength();
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
	for (auto itor = m_spas.begin(); itor != m_spas.end(); itor++)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"SPAS");
		dir->pos = rLen;
		dir->length = (*itor)->GetFieldLength();
		rLen += dir->length;
	}
	//	if(m_fasc)
	for (auto itor = m_fasc.begin(); itor != m_fasc.end(); itor++)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"FASC");
		dir->pos = rLen;
		dir->length = (*itor)->GetFieldLength();
		rLen += dir->length;
	}
	for (auto itor = m_mask.begin(); itor != m_mask.end(); itor++)
	{
		dir = dirInfo.GetDirectory(fieldCnt++);
		dir->tag = *((unsigned int*)"MASK");
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

	/*if (m_frid) */m_frid.Save(file);
	/*if (m_foid) */m_foid.Save(file);
	for (auto itor = m_attr.begin(); itor != m_attr.end(); itor++)
	{
		(*itor)->Save(file);
	}
	for (auto itor = m_inas.begin(); itor != m_inas.end(); itor++)
	{
		(*itor)->Save(file);
	}
	for (auto itor = m_spas.begin(); itor != m_spas.end(); itor++)
	{
		(*itor)->Save(file);
	}
	for (auto itor = m_fasc.begin(); itor != m_fasc.end(); itor++)
	{
		(*itor)->Save(file);
	}
	for (auto itor = m_mask.begin(); itor != m_mask.end(); itor++)
	{
		(*itor)->Save(file);
	}

	return TRUE;
}

void R_FeatureRecord::Draw(CDC *pDC, Scaler *scaler, double offset)
{
	if (m_geometry != NULL) {
		//m_geometry->Draw(pDC, scaler, offset);
	}
}


void R_FeatureRecord::Draw(HDC &hdc, Scaler *scaler, double offset)
{
	//if (m_frid->m_objc == 168)
	if (m_geometry != NULL)
	{
		m_geometry->DrawGeometry(hdc, scaler, offset);
		m_geometry->DrawTextInfo(hdc, scaler, offset);
	}
}

void R_FeatureRecord::Draw(HDC &hDC, Scaler *scaler, int type, int priority, double offset)
{

	//if (m_geometry != NULL && (m_scaleMin == 0 || m_scaleMin > (int)scaler->GetCurrentScale()))
	//{
	//	if (m_geometry->tp)
	//	{
	//		if (m_geometry->tp->camelCase.compare(L"TwoWayRoutePart") == 0)
	//		{
	//			int i = 0;
	//		}
	//	}
	//	switch (type)
	//	{
	//	case 1:
	//		m_geometry->DrawSPoint(hDC, scaler, priority, offset);
	//		break;                                                                                                                                                                                                                                                                                                                          
	//	case 2:
	//		m_geometry->DrawSCurve(hDC, scaler, priority, offset);
	//		break;
	//	case 3:
	//		m_geometry->DrawSSurface(hDC, scaler, priority, offset);
	//		break;
	//	case 4:
	//		m_geometry->DrawSText(hDC, scaler, priority, offset);
	//		break;
	//	}
	//}
}



void R_FeatureRecord::CreateCS(S101Cell *cell, CString csName, bool bSENC)
{
	if (csName.Find(L"L") >= 0)
		int io = 0;

#ifdef _DEBUG11
	return;
#endif

	//	if (csName.Find(L"QUAPOS") == 0)
	//	{
	//		//if (	(m_frid.OBJL == (unsigned)30 &&  m_frid.PRIM == 2 ) 
	//		//	||	(m_frid.OBJL == (unsigned)71 && (m_frid.PRIM == 2 || m_frid.PRIM == 1)) // LNDARE
	//		//	)
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_QUAPOS01(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//	}
	//	else if (csName.Find(L"DEPARE") == 0)
	//		//else if(m_frid.OBJL == (unsigned)42 && m_frid.PRIM == 3) // depth area
	//	{
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_DEPARE02(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//
	//		if (m_geometry->type == 3)
	//		{
	//			auto itor = cell->m_dsgir.m_ftcs->m_arr.find(m_frid.m_nftc);
	//			if (itor != cell->m_dsgir.m_ftcs->m_arr.end())
	//			{
	//				CodeWithNumericCode* nc = itor->second;
	//
	//				if (nc->m_code.Compare(L"DredgedArea") == 0)
	//				{
	//					((S101_CS_DEPARE02*)m_cs)->SetDredgedArea(true);
	//				}
	//			}
	//		}
	//		m_cs->ChooseSymbol();
	//	}
	//	else if (csName.Find(L"DEPCNT") == 0)
	//		//else if (m_frid.OBJL == (unsigned)43 && m_frid.PRIM == 2) // DEPCNT
	//	{
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_DEPCNT03(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//	}
	//	else if (csName.Find(L"LIGHTS") == 0)
	//		//else if(m_frid.OBJL == (unsigned)75) // light
	//	{
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_LIGHTS05(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//	}
	//	else if (csName.Find(L"OBSTRN") == 0)
	//		//else if(m_frid.OBJL == (unsigned)86) // OBSTRN
	//	{
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_OBSTRN06(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//	}
	//	else if (csName.Find(L"RESARE") == 0)
	//		//	else if(m_frid.OBJL == (unsigned)112) // RESARE
	//	{
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_RESARE03(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//	}
	//	else if (csName.Find(L"SLCONS") == 0)
	//		//else if (m_frid.OBJL == (unsigned)122) // SLCONS
	//	{
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_SLCONS03(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//	}
	//	else if (csName.Find(L"TOPMAR") == 0)
	//		//else if(m_frid.OBJL == (unsigned)144) // topmark
	//	{
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_TOPMAR01(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//	}
	//	else if (csName.Find(L"WRECKS") == 0)
	//		//else if (m_frid.OBJL == (unsigned)159 && (m_frid.PRIM == 3 || m_frid.PRIM == 1)) // WRECKS
	//	{
	//#ifndef _DEBUG11
	//		//FRID fr = m_frid;
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_WRECKS04(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//#endif
	//	}
	//	else if (csName.Find(L"RESTRN") == 0)
	//		//else if (m_frid.OBJL == (unsigned)159 && (m_frid.PRIM == 3 || m_frid.PRIM == 1)) // WRECKS
	//	{
	//		//FRID fr = m_frid;
	//		if (!bSENC)
	//		{
	//			m_cs = new S101_CS_RESTRN01(this, (std::wstring)csName);
	//		}
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//	}
	//	else if (csName.Find(L"LEGLIN") == 0)
	//		//else if(m_frid.OBJL == (unsigned)8198)
	//	{
	//		if (m_cs != NULL)
	//		{
	//			S101_CS_LEGLIN03* cs = (S101_CS_LEGLIN03*)m_cs;
	//			delete cs;
	//		}
	//		m_cs = new S101_CS_LEGLIN03(this, (std::wstring)csName);
	//		m_cs->CheckCSVariable(cell, bSENC);
	//		m_cs->ChooseSymbol();
	//	}
	return;
}

MBR R_FeatureRecord::GetMBR()
{
	if (!m_geometry)
		return MBR(0, 0, 0, 0);

	return m_geometry->m_mbr;
}

int R_FeatureRecord::GetRCID()
{
	return m_frid.m_name.RCID;
}

void R_FeatureRecord::GetXYPointOfTheVisiblePoints(double &x, double &y)
{
	std::list<SCurveHasOrient> *pListCurveLink = NULL;
	if (m_geometry->type == 1)
	{
		SPoint* p = (SPoint*)m_geometry;
		x = p->m_point.GetX();
		y = p->m_point.GetY();

		return;
	}
	else if (m_geometry->type == 2)
	{
		SCompositeCurve* cc = (SCompositeCurve*)m_geometry;

		pListCurveLink = &cc->m_listCurveLink;
	}
	else if (m_geometry->type == 3)
	{
		SSurface* s = (SSurface*)m_geometry;

		pListCurveLink = &s->m_listCurveLink;
	}

	if (pListCurveLink)
	{
		for (auto i = pListCurveLink->begin(); i != pListCurveLink->end(); i++)
		{
			SCurve* curve = (*i).GetCurve();
			if (!(*i).GetMasking())
			{
				if (curve->m_numPoints > 2)
				{
					x = curve->m_pPoints[1].GetX();
					y = curve->m_pPoints[1].GetY();

					return;
				}
			}
		}

		if (m_geometry->type == 3)
		{
			SSurface* s = (SSurface*)m_geometry;

			if (s->m_centerPoint)
			{
				x = s->m_centerPoint->GetX();
				y = s->m_centerPoint->GetY();
				return;
			}
		}
	}
	return;
}


int R_FeatureRecord::GetAttributeIndex(ATTR* value)
{
	int result = 1;
	for (auto i = m_attr.begin(); i != m_attr.end(); i++)
	{
		auto fATTR = *i;
		if (fATTR == nullptr)
		{
			return -1;
		}
		if (fATTR != nullptr)
		{
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
	}
	return result;
}

int R_FeatureRecord::GetAssociationCount()
{
	return GetFeatureAssociationCount() + GetInformationAssociationCount();
}

int R_FeatureRecord::GetFeatureAssociationCount()
{
	return (int)m_fasc.size();
}

int R_FeatureRecord::GetInformationAssociationCount()
{
	return (int)m_inas.size();
}

int R_FeatureRecord::GetNumericCode()
{
	return m_frid.m_nftc;
}

SGeometry* R_FeatureRecord::GetGeometry()
{
	return m_geometry;
}