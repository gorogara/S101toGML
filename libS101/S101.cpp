#include "pch.h"
#include "S101.h"

#include "R_InformationRecord.h"
#include "R_FeatureRecord.h"



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

		CFile file;
		if (file.Open(_filepath, CFile::modeRead))
		{
			//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, KRS_MSG_PROCESS::AddLayerMessage(_filepath), KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::DataSet);
			BYTE* pBuf = nullptr;
			BYTE* sBuf = nullptr;
			BYTE* endOfBuf = nullptr;

			LONGLONG fileLength = file.GetLength();

			pBuf = new BYTE[(unsigned int)fileLength];
			sBuf = pBuf;

			file.Read(pBuf, (unsigned)fileLength);

			m_FileType = FILE_S_100_VECTOR;
			S100Product = S100::S100_DataProduct::S_101;

			endOfBuf = pBuf + fileLength - 1;

			file.Close();

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
					m_vecMap.insert({ r->m_prid.m_name.GetName(), r });
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"MRID"))
				{
					R_MultiPointRecord* r = new R_MultiPointRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertMultiPointRecord(r->m_mrid.m_name.GetName(), r);
					m_vecMap.insert({ r->m_mrid.m_name.GetName(), r });
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CRID"))  //경고와 에러 메세지 위주의 출력
				{
					R_CurveRecord* r = new R_CurveRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertCurveRecord(r->m_crid.m_name.GetName(), r);
					m_vecMap.insert({ r->m_crid.m_name.GetName(), r });
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CCID"))
				{
					R_CompositeRecord* r = new R_CompositeRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertCompositeCurveRecord(r->m_ccid.m_name.GetName(), r);
					m_vecMap.insert({ r->m_ccid.m_name.GetName(), r });
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"SRID"))
				{
					R_SurfaceRecord* r = new R_SurfaceRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertSurfaceRecord(r->m_srid.m_name.GetName(), r);
					m_vecMap.insert({ r->m_srid.m_name.GetName(), r });
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

			CalcMBR();
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

} 