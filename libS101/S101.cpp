#include "pch.h"
#include "S101.h"

#include "R_InformationRecord.h"
#include "R_FeatureRecord.h"


#include <fstream>
#include <iostream>

namespace libS101
{

	void S101::Test()
	{
		std::cout << "이거 안나오는거 아니냐" << std::endl;
	}
	bool S101::Open(std::wstring _filepath) // Dataset 시작, .000 파일읽음
	{
		SetFilePath(_filepath);

		//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"DataSET start", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::DataSet);
		//USES_CONVERSION;

		//RemoveAll();

	//	CFile file;
		std::wifstream file;
		//if (file.Open(_filepath, CFile::modeRead))
		file.open(_filepath);
		if (file.is_open())
		{
			//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, KRS_MSG_PROCESS::AddLayerMessage(_filepath), KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::DataSet);
			BYTE* pBuf = nullptr;
			BYTE* sBuf = nullptr;
			BYTE* endOfBuf = nullptr;

			LONGLONG fileLength = file.tellg();
			//LONGLONG fileLength = file.GetLength();

			pBuf = new BYTE[(unsigned int)fileLength];
			sBuf = pBuf;

			//file.Read(pBuf, (unsigned)fileLength);
			file.read(pBuf,fileLength);

			m_FileType = FILE_S_100_VECTOR;
			S100Product = S100::S100_DataProduct::S_101;

			endOfBuf = pBuf + fileLength - 1;

			file.close();
			//file.Close();

			ReadDDR(pBuf);

			DRDirectoryInfo drDir;

			int tcnt = 0;
			while (pBuf < endOfBuf)
			{
				auto curRecordAddress = pBuf;

				tcnt++;
				DRReader drReader;
				int subFieldCount = 0;
				drReader.ReadReader(pBuf);
				subFieldCount = (drReader.m_fieldAreaLoc - DR_LENGTH - 1) / (4 + drReader.m_fieldLength + drReader.m_fieldPosition);

				if (subFieldCount < 1)
				{
					//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_ERROR, L"SubFieldCount Error", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::DataSet);
					continue;
				}

				drDir.ReAllocateDirectory(subFieldCount);

				drDir.ReadDir(drReader, pBuf);

				if (*(pBuf++) != 0x1E)
				{
					//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_ERROR, L"terminator Error", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::DataSet);
				}

				if (drDir.GetDirectory(0)->tag == *((unsigned int*)"DSID"))
				{
					m_dsgir.ReadRecord(&drDir, pBuf);
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CSID"))
				{
					m_dscrs.ReadRecord(&drDir, pBuf);
				}

				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"IRID"))
				{
					R_InformationRecord* r = new R_InformationRecord();
					r->ReadRecord(&drDir, pBuf);
					InsertInformationRecord(r->m_irid.m_name.GetName(), r);
				}

				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"PRID"))
				{
					R_PointRecord* r = new R_PointRecord();
					r->ReadRecord(&drDir, pBuf);
					auto names = r->m_prid.m_name.GetName();

					InsertPointRecord(r->m_prid.m_name.GetName(), r);
					//m_vecMap.insert({ r->m_prid.m_name.GetName(), r });
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"MRID"))
				{
					R_MultiPointRecord* r = new R_MultiPointRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertMultiPointRecord(r->m_mrid.m_name.GetName(), r);
					//m_vecMap.insert({ r->m_mrid.m_name.GetName(), r });
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CRID"))  //경고와 에러 메세지 위주의 출력
				{
					R_CurveRecord* r = new R_CurveRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertCurveRecord(r->m_crid.m_name.GetName(), r);
					//m_vecMap.insert({ r->m_crid.m_name.GetName(), r });
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CCID"))
				{
					R_CompositeRecord* r = new R_CompositeRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertCompositeCurveRecord(r->m_ccid.m_name.GetName(), r);
					//m_vecMap.insert({ r->m_ccid.m_name.GetName(), r });
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"SRID"))
				{
					R_SurfaceRecord* r = new R_SurfaceRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertSurfaceRecord(r->m_srid.m_name.GetName(), r);
					//m_vecMap.insert({ r->m_srid.m_name.GetName(), r });
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"FRID")) //피쳐 단위처리
				{
					R_FeatureRecord* r = new R_FeatureRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertFeatureRecord(r->m_frid.m_name.GetName(), r);
				}

				pBuf = curRecordAddress + drReader.m_recordLength;
			}

			delete[] sBuf;

			MakeFullSpatialData();

			//CalcMBR();
			Check();

			return true;
		}

		return false;
	}

	void  S101::InsertInformationRecord(__int64 key, R_InformationRecord* record) 
	{
		vecInformation.push_back(record);
	}

	void  S101::InsertPointRecord(__int64 key, R_PointRecord* record)
	{;
		vecPoint.push_back(record);
	}

	void  S101::InsertMultiPointRecord(__int64 key, R_MultiPointRecord* record)
	{
		vecMultiPoint.push_back(record);
	}

	void  S101::InsertCurveRecord(__int64 key, R_CurveRecord* record)
	{
		vecCurve.push_back(record);
	}

	void  S101::InsertCompositeCurveRecord(__int64 key, R_CompositeRecord* record)
	{
		vecComposite.push_back(record);
	}

	void  S101::InsertSurfaceRecord(__int64 key, R_SurfaceRecord* record)
	{
		vecSurface.push_back(record);
	}

	void  S101::InsertFeatureRecord(__int64 key, R_FeatureRecord* record)
	{
		vecFeature.push_back(record);
	}

	bool S101::MakeFullSpatialData()
	{
		ClearCurveMap();

		POSITION spasPos = NULL;
		R_FeatureRecord* fr;

		POSITION pos = m_feaMap.GetStartPosition();
		__int64 iKey;

		while (pos)
		{
			m_feaMap.GetNextAssoc(pos, iKey, fr);
			if (fr->m_spas.size() == 0)
			{
				continue;
			}

			for (auto itorParent = fr->m_spas.begin(); itorParent != fr->m_spas.end(); itorParent++)
			{
				F_SPAS* spasParent = *itorParent;

				for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
				{
					SPAS* spas = *itor;

					if (spas->m_name.RCNM == 110)
					{
						MakePointData(fr);
					}
					else if (spas->m_name.RCNM == 115)
					{
						MakeSoundingData(fr);
					}
					else if (spas->m_name.RCNM == 120 || spas->m_name.RCNM == 125)
					{
						MakeLineData(fr);
					}
					else if (spas->m_name.RCNM == 130)
					{
						MakeAreaData(fr);
					}
				}
			}

			//GetFullMaskData(fr);
		}
		return TRUE;
	}
	bool S101::MakePointData(R_FeatureRecord* fe)
	{
		fe->m_curveList.clear();
		SPAS* spas = NULL;
		R_PointRecord* r;
		__int64 iKey;

		GeoPoint geoArr;

		for (auto itorParent = fe->m_spas.begin(); itorParent != fe->m_spas.end(); itorParent++)
		{
			F_SPAS* spasParent = *itorParent;

			for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
			{
				SPAS* spas = *itor;

				iKey = ((__int64)spas->m_name.RCNM) << 32 | spas->m_name.RCID;
				if (m_ptMap.Lookup(iKey, r))
				{
					if (r->m_c2it)
					{
						GeoPoint geoArr;
						GetFullCurveData(fe, r);
						GetFullSpatialData(r, geoArr);

						if (fe->m_geometry)
							delete fe->m_geometry;

						fe->m_geometry = new SPoint();
						SPoint* geo = (SPoint*)fe->m_geometry;

						geo->SetPoint(geoArr.x, geoArr.y);

						geo->m_mbr.CalcMBR(geoArr.x, geoArr.y);
					}
					else if (r->m_c3it)
					{
						GeoPointZ geoArr;

						GetFullCurveData(fe, r);
						GetFullSpatialData(r, geoArr);

						int cnt = 1;

						if (fe->m_geometry)
							delete fe->m_geometry;

						fe->m_geometry = new SMultiPoint();
						SMultiPoint* geo = (SMultiPoint*)fe->m_geometry;

						geo->m_numPoints = cnt;
						if (!geo->m_pPoints)
						{
							geo->m_pPoints = new std::vector<GeoPointZ>(geo->m_numPoints);//new GeoPointZ[geo->m_numPoints];
						}
						else
						{
							geo->m_pPoints->clear();
							if ((int)(*geo->m_pPoints).size() < geo->m_numPoints)
								(*geo->m_pPoints).resize(geo->m_numPoints + 1);
						}

						for (int i = 0; i < cnt; i++)
						{
							(*geo->m_pPoints)[i].SetPoint(geoArr.x, geoArr.y, geoArr.z);
						}
					}
				}
			}
		}
		return TRUE;
	}
	bool S101::MakeSoundingData(R_FeatureRecord* fe)
	{
		fe->m_curveList.clear();
		R_MultiPointRecord* r;
		__int64 iKey;

		CArray<GeoPointZ> geoArr;


		for (auto itorParent = fe->m_spas.begin(); itorParent != fe->m_spas.end(); itorParent++)
		{
			F_SPAS* spasParent = *itorParent;

			for (auto itor = spasParent->m_arr.begin(); itor != spasParent->m_arr.end(); itor++)
			{
				SPAS* spas = *itor;

				iKey = ((__int64)spas->m_name.RCNM) << 32 | spas->m_name.RCID;
				if (m_mpMap.Lookup(iKey, r))
				{
					GetFullSpatialData(r, geoArr);
				}
			}
		}

		int cnt = 0;
		cnt = (int)geoArr.GetCount();

		if (fe->m_geometry)
			delete fe->m_geometry;

		fe->m_geometry = new SMultiPoint();
		SMultiPoint* geo = (SMultiPoint*)fe->m_geometry;

		geo->m_numPoints = cnt;
		if (!geo->m_pPoints) geo->m_pPoints = new std::vector<GeoPointZ>(geo->m_numPoints);//new GeoPointZ[geo->m_numPoints];
		else
		{
			geo->m_pPoints->clear();
			if ((int)(*geo->m_pPoints).size() < geo->m_numPoints)
				(*geo->m_pPoints).resize(geo->m_numPoints + 1);
		}

		if (geo->m_numPoints > SGeometry::sizeOfPoint)
		{
			SGeometry::sizeOfPoint = geo->m_numPoints;
			delete SGeometry::viewPoints;
			SGeometry::viewPoints = new CPoint[int(SGeometry::sizeOfPoint * 1.5)];
		}

		for (int i = 0; i < cnt; i++)
		{
			(*geo->m_pPoints)[i].SetPoint(geoArr[i].x, geoArr[i].y, geoArr[i].z);
			geo->m_mbr.CalcMBR(geoArr[i].x, geoArr[i].y);
		}

		geoArr.RemoveAll();
		return TRUE;
	}
	bool S101::MakeLineData(R_FeatureRecord* fe)
	{
		fe->m_curveList.clear();

		R_CurveRecord* cr = nullptr;

		R_CompositeRecord* ccr = nullptr;

		__int64 iKey = 0;

		CArray<GeoPoint> geoArr;

		if (fe->m_geometry)
		{
			delete fe->m_geometry;
			fe->m_geometry = nullptr;
		}

		for (auto i = fe->m_spas.begin(); i != fe->m_spas.end(); i++)
		{
			F_SPAS* spasParent = *i;

			for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
			{
				SPAS* spas = *j;
				iKey = ((__int64)spas->m_name.RCNM) << 32 | spas->m_name.RCID;

				if (m_comMap.Lookup(iKey, ccr))
				{
					GetFullCurveData(fe, ccr, spas->m_ornt);
				}
				else if (m_curMap.Lookup(iKey, cr))
				{
					GetFullCurveData(fe, cr, spas->m_ornt);
				}
			}
		}

		SCompositeCurve* scc = new SCompositeCurve();
		fe->m_geometry = scc;

		SetSCurveList(&fe->m_curveList, &scc->m_listCurveLink);

		scc->SetMBR();

		if (gisLib == nullptr)
		{
			return false;
		}


		scc->CreateD2Geometry(gisLib->D2.pD2Factory);

		geoArr.RemoveAll();

		return TRUE;
	}
	bool S101::MakeAreaData(R_FeatureRecord* fe)
	{
		fe->m_curveList.clear();

		R_SurfaceRecord* sr;
		__int64 iKey;
		CArray<GeoPoint> geoArr;
		std::vector<POINT> vecPoint;
		std::vector<int> boundaryList;

		for (auto i = fe->m_spas.begin(); i != fe->m_spas.end(); i++)
		{
			F_SPAS* spasParent = *i;

			for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
			{
				SPAS* spas = *j;

				iKey = spas->m_name.GetName();

				if (m_surMap.Lookup(iKey, sr))
				{
					for (auto k = sr->m_rias.begin(); k != sr->m_rias.end(); k++)
					{
						F_RIAS* riasParent = *k;

						for (auto l = riasParent->m_arr.begin(); l != riasParent->m_arr.end(); l++)
						{
							RIAS* rias = *l;

							auto iKey = rias->m_name.GetName();
							if (rias->m_name.RCNM == 120)
							{
								R_CurveRecord* cr = nullptr;
								if (m_curMap.Lookup(iKey, cr))
								{
									GetFullCurveData(fe, cr);
									//GetFullSpatialData(cr, geoArr, rias->m_ornt);

									GetFullSpatialData(cr, vecPoint, rias->m_ornt);
								}
							}
							else if (rias->m_name.RCNM == 125)
							{
								R_CompositeRecord* ccr = nullptr;
								if (m_comMap.Lookup(iKey, ccr))
								{
									GetFullCurveData(fe, ccr);
									//GetFullSpatialData(ccr, geoArr, rias->m_ornt);

									GetFullSpatialData(ccr, vecPoint, rias->m_ornt);
								}
							}

							//int sizet = geoArr.GetCount();
							int sizet = (int)vecPoint.size();
							//geoArr.GetAt(sizet - 1);
							//GeoPoint t1 = geoArr[0];
							//GeoPoint t2 = geoArr[sizet - 1];

							//if (t1 == t2)
							//{
							//}
							//else
							//{
							//	geoArr.Add(geoArr[0]);
							//	sizet++;
							//}

							if (sizet == 0)
							{
								continue;
							}

							if (rias->m_usag == 1)
							{
								boundaryList.push_back(sizet);
							}
							else
							{
								boundaryList.push_back(sizet);
							}
						}
					}
				}
			}
		}

		int i = 0;

		if (fe->m_geometry)
		{
			delete fe->m_geometry;
		}

		fe->m_geometry = new SSurface(vecPoint, boundaryList);
		//auto surface = (SSurface*)fe->m_geometry;

		SSurface* geo = ((SSurface*)fe->m_geometry);
		//geo->m_numParts = boundaryList.GetCount();
		//geo->m_numPoints = geoArr.GetCount();

		//int cnt = boundaryList.GetCount();

		//if (boundaryList.GetCount() < 2)
		//{
		//	geo->m_pParts = new int[1];
		//	geo->m_pParts[0] = 0;
		//}
		//else
		//{
		//	geo->m_pParts = new int[boundaryList.GetCount()];
		//	geo->m_pParts[0] = 0;

		//	POSITION partPos = boundaryList.GetHeadPosition();
		//	for (i = 1; i < cnt; i++)
		//	{
		//		geo->m_pParts[i] = boundaryList.GetNext(partPos);
		//	}
		//}


		//cnt = (int)geoArr.GetCount();
		//geo->m_pPoints = new GeoPoint[geo->m_numPoints];

		//if (geo->m_numPoints > SGeometry::sizeOfPoint)
		//{
		//	SGeometry::sizeOfPoint = geo->m_numPoints;
		//	delete SGeometry::viewPoints; 
		//	SGeometry::viewPoints = new CPoint[int(SGeometry::sizeOfPoint * 1.5)];
		//}

		//for (i = 0; i < cnt; i++)
		//{
		//	geo->m_pPoints[i].SetPoint(geoArr[i].x, geoArr[i].y);
		//	geo->m_mbr.CalcMBR(geoArr[i].x, geoArr[i].y);
		//}

		fe->m_curveList.clear();

		R_CurveRecord* cr;
		R_CompositeRecord* ccr;

		for (auto i = fe->m_spas.begin(); i != fe->m_spas.end(); i++)
		{
			F_SPAS* spasParent = *i;

			for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
			{
				SPAS* spas = *j;
				iKey = spas->m_name.GetName();

				if (m_surMap.Lookup(iKey, sr))
				{
					GetFullCurveData(fe, sr);
				}
				else if (m_comMap.Lookup(iKey, ccr))
				{
					GetFullCurveData(fe, ccr);
				}
				else if (m_curMap.Lookup(iKey, cr))
				{
					GetFullCurveData(fe, cr);
				}
			}
		}

		SetSCurveList(&fe->m_curveList, &geo->m_listCurveLink);

		geoArr.RemoveAll();

		if (gisLib == nullptr)
		{
			return false;
		}
		geo->CreateD2Geometry(gisLib->D2.pD2Factory);

		geo->CalculateCenterPoint();
		return TRUE;
	}

	bool S101::Check()
	{
		if (GetCount_InformationRecord() != GetMetaCount_InformationRecord())
		{
			OutputDebugString(L"error - Information Record count\n");
			return false;
		}

		if (GetCount_PointRecord() != GetMetaCount_PointRecord())
		{
			OutputDebugString(L"error - Point Record count\n");
			return false;
		}

		if (GetCount_MultiPointRecord() != GetMetaCount_MultiPointRecord())
		{
			OutputDebugString(L"error - Multi Point Record count\n");
			return false;
		}

		if (GetCount_CurveRecord() != GetMetaCount_CurveRecord())
		{
			OutputDebugString(L"error - Curve Record count\n");
			return false;
		}

		if (GetCount_CompositeCurveRecord() != GetMetaCount_CompositeCurveRecord())
		{
			OutputDebugString(L"error - Composite Curve Record count\n");
			return false;
		}

		if (GetCount_SurfaceRecord() != GetMetaCount_SurfaceRecord())
		{
			OutputDebugString(L"error - Surface Record count\n");
			return false;
		}

		if (GetCount_FeatureTypeRecord() != GetMetaCount_FeatureTypeRecord())
		{
			OutputDebugString(L"error - Feature Type Record count\n");
			return false;
		}

		return true;
	}

	int S101::GetCount_InformationRecord()
	{
		return (int)vecInformation.size();
	}
	int S101::GetCount_PointRecord()
	{
		return (int)vecPoint.size();
	}
	int S101::GetCount_MultiPointRecord()
	{
		return (int)vecMultiPoint.size();
	}
	int S101::GetCount_CurveRecord()
	{
		return (int)vecCurve.size();
	}
	int S101::GetCount_CompositeCurveRecord()
	{
		return (int)vecComposite.size();
	}
	int S101::GetCount_SurfaceRecord()
	{
		return (int)vecSurface.size();
	}
	int S101::GetCount_FeatureTypeRecord()
	{
		return (int)vecFeature.size();
	}

	int S101::GetMetaCount_InformationRecord()
	{
		return m_dsgir.m_dssi.m_noir;
	}
	int S101::GetMetaCount_PointRecord()
	{
		return m_dsgir.m_dssi.m_nopn;
	}
	int S101::GetMetaCount_MultiPointRecord()
	{
		return m_dsgir.m_dssi.m_nomn;
	}
	int S101::GetMetaCount_CurveRecord()
	{
		return m_dsgir.m_dssi.m_nocn;
	}
	int S101::GetMetaCount_CompositeCurveRecord()
	{
		return m_dsgir.m_dssi.m_noxn;
	}
	int S101::GetMetaCount_SurfaceRecord()
	{
		return m_dsgir.m_dssi.m_nosn;
	}
	int S101::GetMetaCount_FeatureTypeRecord()
	{
		return m_dsgir.m_dssi.m_nofr;
	}
}