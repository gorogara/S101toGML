#include "pch.h"
#include "S101.h"
#include "R_CompositeRecord.h"
#include "R_SurfaceRecord.h"
#include "R_InformationRecord.h"
#include "R_MultiPointRecord.h"
#include "R_CurveRecord.h"
#include "R_FeatureRecord.h"
#include "R_PointRecord.h"
#include "DRDirectory.h"
#include "DRDirectoryInfo.h"
#include "ISO8211Fuc.h"
//#include "S100Layer.h"
#include "ATTR.h"
#include "F_INAS.h"
#include "F_FASC.h"
#include "CodeWithNumericCode.h"
#include "F_CodeWithNumericCode.h"
//#include "Catalog.h"

//#include "SimpleAttribute.h"
//#include "ListedValues.h"
//#include "ListedValue.h"

#include "SCompositeCurve.h"
#include "SPoint.h"
#include "SSurface.h"
#include "SCurve.h"
#include "S100_CD_AttributeValueType.h"
#include "GeoCommonFuc.h"
#include "SCurveHasOrient.h"


#include "..\\LibMFCUtil\\LibMFCUtil.h"


#undef _WINDOWS_
#include <afxext.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>


namespace libS101
{
	std::unordered_map<std::string, pugi::xml_node*> objectPugiXmlElementMap;
	std::wstring get_feature_id(int id, int ENCODING = 0)
	{
		wchar_t numChar[5];
		wsprintf(numChar, L"%04d", id);
		std::wstringstream wss;
		wss << L"FEATURE_ID_" << std::setw(4) << numChar;
		return wss.str();
	}

	std::wstring get_information_id(int id, int ENCODING = 0)
	{
		wchar_t numChar[5];
		wsprintf(numChar, L"%04d", id);
		std::wstringstream wss;
		wss << L"INFO_ID_" << std::setw(4) << numChar;
		return wss.str();
	}

	std::string get_feature_id_string(int id)
	{
		char numChar[5];
		sprintf_s(numChar, "%04d", id);
		std::stringstream ss;
		ss << "FEATURE_ID_" << std::setw(4) << numChar;
		return ss.str();
	}

	std::string get_information_id_string(int id)
	{
		char numChar[5];
		sprintf_s(numChar, "%04d", id);
		std::stringstream ss;
		ss << "INFO_ID_" << std::setw(4) << numChar;
		return ss.str();
	}

	std::wstring get_wstring_from_coordinate_1(int value)
	{
		std::wstringstream wss;
		wss.setf(std::ios::fixed);
		wss.setf(std::ios::showpoint);
		wss.precision(7);

		wss << value / 10000000.;
		return wss.str();
	}


	void S101::Test()
	{
		std::cout << "이거 안나오는거 아니냐" << std::endl;
	}

#pragma warning(disable:4018)
	bool S101::Open(CString _filepath) // Dataset 시작, .000 파일읽음
	{
		//CString _filePath = CA2CT(path.c_str());
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

			//MakeFullSpatialData();

			//CalcMBR();
			//Check();

			return true;
		}

		return false;
	}

	void S101::Save(CString _filepath, CString extend)
	{
		//GML 저장시 이곳으로 넘어오게 됩니다.
		//objectXmlElementMap.clear();

		POSITION pos = NULL;
		__int64 iKey;
		CString saveFileName = _filepath; //파일의 최종경로를 불러옵니다.

		return GmlifileMakeByPugi(saveFileName);


	}

	void S101::GmlifileMakeByPugi(CString _filePath) //PugiXml을 사용하여 파일을 저장합니다.
	{
		pugi::xml_document* doc = new pugi::xml_document();
		auto declarationNode = doc->append_child(pugi::node_declaration);
		declarationNode.append_attribute("version") = "1.0";
		declarationNode.append_attribute("encoding") = "UTF-8";

		pugi::xml_node root;
		std::string productNamespace; // ex) S101, S124, S201 ...


		//Layer* layer = GetS100Layer();
		//auto s100layer = (S100Layer*)layer;

		//auto feature = s100layer->GetFC();
		//std::wstring  featureType = feature->GetFeatureCatalogueName();

		//if (featureType.find(L"S-101") != std::wstring::npos)
		//{
		//	productNamespace = "S101";
		//	root = doc->append_child("S101:DataSet");
		//	root.append_attribute("xmlns:S101") = "http://www.iho.int/S101/gml/1.0";
		//	root.append_attribute("xsi:schemaLocation") = "http://www.iho.int/S-101/gml/1.0 ../../../schemas/S101/1.0/20170430/S101.xsd";
		//	root.append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";
		//	root.append_attribute("xmlns:xlink") = "http://www.w3.org/1999/xlink";
		//	root.append_attribute("xmlns:gml") = "http://www.opengis.net/gml/3.2";
		//	root.append_attribute("xmlns:S100") = "http://www.iho.int/s100gml/1.0";
		//	//root.append_attribute("xmlns:s100") = "http://www.iho.int/S-100/profile/s100_gmlProfile";
		//	root.append_attribute("xmlns:s100_profile") = "http://www.iho.int/S-100/profile/s100_gmlProfile";
		//	root.append_attribute("gml:id") = "KRNPI101_TEST002_001";
		//}

		productNamespace = "S101";
		root = doc->append_child("S101:DataSet");
		root.append_attribute("xmlns:S101") = "http://www.iho.int/S101/gml/1.0";
		root.append_attribute("xsi:schemaLocation") = "http://www.iho.int/S-101/gml/1.0 ../../../schemas/S101/1.0/20170430/S101.xsd";
		root.append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";
		root.append_attribute("xmlns:xlink") = "http://www.w3.org/1999/xlink";
		root.append_attribute("xmlns:gml") = "http://www.opengis.net/gml/3.2";
		root.append_attribute("xmlns:S100") = "http://www.iho.int/s100gml/1.0";
		//root.append_attribute("xmlns:s100") = "http://www.iho.int/S-100/profile/s100_gmlProfile";
		root.append_attribute("xmlns:s100_profile") = "http://www.iho.int/S-100/profile/s100_gmlProfile";
		root.append_attribute("gml:id") = "KRNPI101_TEST002_001";




		auto boundedBy = root.append_child("gml:boundedBy");
		auto Envelope = boundedBy.append_child("gml:Envelope");
		Envelope.append_attribute("srsName") = "EPSG:4326";
		Envelope.append_attribute("srsDimension") = "2";

		auto mbr = GetMBR();

		inverseProjection(mbr.xmin, mbr.ymin);
		inverseProjection(mbr.xmax, mbr.ymax);

		std::wstring strX = get_wstring_from_coordinate_1((int)(mbr.xmin * m_dsgir.m_dssi.m_cmfx));
		std::wstring strY = get_wstring_from_coordinate_1((int)(mbr.ymin * m_dsgir.m_dssi.m_cmfy));
		std::wstring strCoordi = strX + L" " + strY;
		std::string cStrCoordi = pugi::as_utf8(strCoordi);

		pugi::xml_node lowerCorner = Envelope.append_child("gml:lowerCorner");
		lowerCorner.append_child(pugi::node_pcdata).set_value(cStrCoordi.c_str());

		strX = get_wstring_from_coordinate_1((int)(mbr.xmax * m_dsgir.m_dssi.m_cmfx));
		strY = get_wstring_from_coordinate_1((int)(mbr.ymax * m_dsgir.m_dssi.m_cmfy));

		strCoordi = strX + L" " + strY;
		cStrCoordi = pugi::as_utf8(strCoordi);

		pugi::xml_node upperCorner = Envelope.append_child("gml:upperCorner");
		upperCorner.append_child(pugi::node_pcdata).set_value(cStrCoordi.c_str());

#pragma region DatasetIdentificationInformation

		pugi::xml_node DatasetIdentificationInformation = root.append_child("DatasetIdentificationInformation");

		pugi::xml_node EncodingSpecification = DatasetIdentificationInformation.append_child("S100:encodingSpecification");
		EncodingSpecification.append_child(pugi::node_pcdata).set_value(GetEncodingSpecificationToString().c_str());

		pugi::xml_node EncodingSpecificationEdication = DatasetIdentificationInformation.append_child("S100:encodingSpecificationEdition");
		EncodingSpecificationEdication.append_child(pugi::node_pcdata).set_value(GetEncodingSpecificationEditionToString().c_str());

		pugi::xml_node productIdentifier = DatasetIdentificationInformation.append_child("S100:productIdentifier");
		productIdentifier.append_child(pugi::node_pcdata).set_value(GetProductIdentifierToString().c_str());

		pugi::xml_node productEdition = DatasetIdentificationInformation.append_child("S100:productEdition");
		productEdition.append_child(pugi::node_pcdata).set_value(GetProductEditionToString().c_str());

		pugi::xml_node applicationProfile = DatasetIdentificationInformation.append_child("S100:applicationProfile");
		applicationProfile.append_child(pugi::node_pcdata).set_value(GetApplicationProfileToString().c_str());

		std::string fileName(LibMFCUtil::CStringToString(GetFileName()));
		pugi::xml_node datasetFileIdentifier = DatasetIdentificationInformation.append_child("S100:datasetFileIdentifier");
		datasetFileIdentifier.append_child(pugi::node_pcdata).set_value(GetDatasetFileIdentifierToString().c_str());


		pugi::xml_node datasetTitle = DatasetIdentificationInformation.append_child("S100:datasetTitle");
		datasetTitle.append_child(pugi::node_pcdata).set_value(GetDatasetTitleToString().c_str());


		pugi::xml_node datasetReferenceDate = DatasetIdentificationInformation.append_child("S100:datasetReferenceDate");
		datasetReferenceDate.append_child(pugi::node_pcdata).set_value(GetDatasetReferenceDataToString().c_str());

		pugi::xml_node datasetLanguage = DatasetIdentificationInformation.append_child("S100:datasetLanguage");
		datasetLanguage.append_child(pugi::node_pcdata).set_value(GetDatasetLanguageToString().c_str());

		//문서보고 추가함
		pugi::xml_node datasetAbstract = DatasetIdentificationInformation.append_child("S100:datasetAbstract");
		datasetAbstract.append_child(pugi::node_pcdata).set_value(GetDatasetAbstractToString().c_str());

		pugi::xml_node datasetEdition = DatasetIdentificationInformation.append_child("S100:datasetEdition");
		datasetEdition.append_child(pugi::node_pcdata).set_value(GetDatasetEditionToString().c_str());

		pugi::xml_node datasetTopicCategory = DatasetIdentificationInformation.append_child("S100:datasetTopicCategory");
		datasetTopicCategory.append_child(pugi::node_pcdata).set_value("utilitiesCommunication");


#pragma endregion 
#pragma region DatasetStructureInformation

		pugi::xml_node DatasetStructureInformation = root.append_child("DatasetStructureInformation");

		pugi::xml_node DatasetCoordOriginX = DatasetStructureInformation.append_child("S100:datasetCoordOriginX");
		DatasetCoordOriginX.append_child(pugi::node_pcdata).set_value("0.0");

		pugi::xml_node DatasetCoordOriginY = DatasetStructureInformation.append_child("S100:datasetCoordOriginY");
		DatasetCoordOriginY.append_child(pugi::node_pcdata).set_value("0.0");

		pugi::xml_node DatasetCoordOriginZ = DatasetStructureInformation.append_child("S100:datasetCoordOriginZ");
		DatasetCoordOriginZ.append_child(pugi::node_pcdata).set_value("0.0");

		pugi::xml_node coordMultFactorX = DatasetStructureInformation.append_child("S100:coordMultFactorX");
		coordMultFactorX.append_child(pugi::node_pcdata).set_value("1");

		pugi::xml_node coordMultFatcorY = DatasetStructureInformation.append_child("S100:coordMultFactorY");
		coordMultFatcorY.append_child(pugi::node_pcdata).set_value("1");

		pugi::xml_node coordMultFatcorZ = DatasetStructureInformation.append_child("S100:coordMultFactorZ");
		coordMultFatcorZ.append_child(pugi::node_pcdata).set_value("1");
#pragma endregion

		SetInformationsType(doc, root, productNamespace); //임의로 부모의 값을 집어넣었습니다.
		SetFeaturesType(doc, root, productNamespace); //임의로 부모의 값을 집어넣었습니다.

#pragma region RELATION
		SetFeaturesTypeRelation_v2(root);
		SetInformationsTypeRelation_v2(root);
#pragma endregion 

		auto xmlSaveError = doc->save_file(_filePath);
		if (!xmlSaveError)
		{
			//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_ERROR, L"GML File Save Fail", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::None);
		}
		else
		{
			//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"GML File Create Success", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::None);
		}
	}

	void S101::SetInformationsType(pugi::xml_document* doc, pugi::xml_node parentNode, std::string productNamespace)
	{
		//__int64 key = 0;
		//R_InformationRecord* ir = NULL;


		//POSITION pos = vecInformation.GetStartPosition();
		//POSITION pos = m_infMap.GetStartPosition();

		/*auto s100Layer = (S100Layer*)m_pLayer;
		auto catalog = s100Layer->GetFC();
		if (nullptr == catalog)
		{
			return;
		}*/

		//auto fc = catalog->GetFC();
		for (auto itor = vecInformation.begin(); itor != vecInformation.end(); itor++)
		{
			R_InformationRecord* ir = *(itor);
			CString informationAcronym = m_dsgir.m_itcs->m_arr.find(ir->m_irid.m_nitc)->second->m_code;
			pugi::xml_node imember = parentNode.append_child("imember");

			auto featureElementName = productNamespace + ":" + LibMFCUtil::CStringToString(informationAcronym);
			pugi::xml_node informationNode = imember.append_child(featureElementName.c_str());

			std::string iid = get_information_id_string(ir->m_irid.m_name.RCID);

			informationNode.append_attribute("gml:id") = iid.c_str();

			objectPugiXmlElementMap.insert(std::unordered_map<std::string, pugi::xml_node*>::value_type(iid, &informationNode));
			SetAttributeType(doc, informationNode, &ir->m_attr);
		}


		//if (pos != nullptr)
		//{
		//	while (pos != nullptr)
		//	{
		//		m_infMap.GetNextAssoc(pos, key, ir);
		//		CString informationAcronym = m_dsgir.m_itcs->m_arr.find(ir->m_irid.m_nitc)->second->m_code;
		//		pugi::xml_node imember = parentNode.append_child("imember");

		//		auto featureElementName = productNamespace + ":" + LibMFCUtil::CStringToString(informationAcronym);
		//		pugi::xml_node informationNode = imember.append_child(featureElementName.c_str());

		//		std::string iid = get_information_id_string(ir->m_irid.m_name.RCID);

		//		informationNode.append_attribute("gml:id") = iid.c_str();

		//		objectPugiXmlElementMap.insert(std::unordered_map<std::string, pugi::xml_node*>::value_type(iid, &informationNode));
		//		SetAttributeType(doc, informationNode, &ir->m_attr);
		//	}
		//}
		//else
		//{
		//	//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_WARNING, L"map start position is null = imember is not making", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::DataSet);
		//}


#pragma region 이전코드
		//while (pos != nullptr)
		//{
		//	m_infMap.GetNextAssoc(pos, key, ir);
		//	CString informationAcronym = m_dsgir.m_itcs->m_arr.find(ir->m_irid.m_nitc)->second->m_code;

		//	pugi::xml_node pInformationNode = parentNode.append_child("");

		//	wstring informationid = get_information_id(ir->m_irid.m_name.RCID);
		//	pInformationNode.append_attribute("") = "";

		//	objectXmlElementMapbyPugi.insert(std::unordered_map<wstring, pugi::xml_node>::value_type(informationid, pInformationNode));

		//	for (auto itorParent = ir->m_attr.begin(); itorParent != ir->m_attr.end(); itorParent++)
		//	{
		//		F_ATTR* attr = *itorParent;
		//		int index = 1;
		//		attrXmlElementMap.clear();

		//		for (auto aitor = attr->m_arr.begin(); aitor != attr->m_arr.end(); aitor++)
		//		{
		//			index++;
		//			ATTR* attr = *aitor;

		//			auto itor = m_dsgir.m_atcs->m_arr.find(attr->m_natc);
		//			if (itor != m_dsgir.m_atcs->m_arr.end())
		//			{
		//				continue;
		//			}

		//			wstring attributeName = itor->second->m_code;
		//			//pugi::xml_node attributeNode= parentNode.append_child(attributeName); //형변환 문제때문에 잠시 보류
		//			 //형변환 문제때문에 잠시 보류

		//			if (attr->m_paix == 0) //가장 기본 index에 들어갑니다.?
		//			{
		//				pugi::xml_node attributeNode = parentNode.append_child("");
		//				//attributeNode.set_value(attr->m_atvl);
		//				attributeNode.append_child(pugi::node_pcdata).set_value("");
		//			}
		//			else
		//			{
		//				auto itor = attrXmlElementMap.find(attr->m_paix);
		//				pugi::xml_node parent = itor->second;
		//				pugi::xml_node attributeNode = parent.append_child("");
		//				//attributeNode.set_value();
		//			}

		//		}
		//	}


		//	for (auto itorParent = ir->m_inas.begin(); itorParent != ir->m_inas.end(); itorParent++)
		//	{
		//		F_INAS* f_fasc = *itorParent;

		//		_int64 key = ((_int64)100) << 32 | f_fasc->m_name.RCID;
		//		wstring informationid = get_information_id(ir->m_irid.m_name.RCID);

		//		auto itor = objectXmlElementMapbyPugi.find(informationid);

		//		pugi::xml_node attributeNode = itor->second;
		//		wstring eid = L"#";
		//		//eid.append(attributeNode.attribute("id")); //형변환 문제때문에 잠시 보류

		//		auto asitor = m_dsgir.m_iacs->m_arr.find(f_fasc->m_niac);
		//		auto ritor = m_dsgir.m_iacs->m_arr.find(f_fasc->m_narc);

		//		wstring asName = asitor->second->m_code;
		//		wstring roleName = ritor->second->m_code;

		//		attributeNode = parentNode.append_child("S100:informationAssociation");
		//		attributeNode.append_attribute("xlink:role") = roleName.c_str();
		//		attributeNode.append_attribute("xlink:href") = eid.c_str();
		//	}
		//}
#pragma endregion

	}

	void S101::SetFeaturesType(pugi::xml_document* document, pugi::xml_node parentNode, std::string productNamespace)
	{
		//_int64 key = 0;
		//R_FeatureRecord* fr = nullptr;

		for (auto itor = vecFeature.begin(); itor != vecFeature.end(); itor++)
		{
			R_FeatureRecord* fr = *(itor);
			//map의 가장 첫번째 값을 가지고옵니다.
		//	m_feaMap.GetNextAssoc(pos, key, fr);

			//if (!fr->m_geometry) //NogeoMetry??아님미까
			//{
			//	continue;
			//}
			auto what = m_dsgir.m_ftcs->m_arr.find(fr->m_frid.m_nftc);

			pugi::xml_node member = parentNode.append_child("member");

			CString featureAcronym = m_dsgir.m_ftcs->m_arr.find(fr->m_frid.m_nftc)->second->m_code;

			auto featureElementName = productNamespace + ":" + pugi::as_utf8(std::wstring(featureAcronym));

			pugi::xml_node pFeatureNode = member.append_child(featureElementName.c_str());

			std::string iid = get_feature_id_string(fr->m_frid.m_name.RCID);
			pFeatureNode.append_attribute("gml:id") = iid.c_str();
			objectPugiXmlElementMap.insert({ iid, &pFeatureNode });

			SetAttributeType(document, pFeatureNode, &fr->m_attr);
			SetVector(pFeatureNode, fr);
		}




		//POSITION pos = m_feaMap.GetStartPosition();
		//if (pos != nullptr)
		//{
		//	while (pos != nullptr)
		//	{
		//		//map의 가장 첫번째 값을 가지고옵니다.
		//		m_feaMap.GetNextAssoc(pos, key, fr);
		//		//if (!fr->m_geometry) //NogeoMetry??아님미까
		//		//{
		//		//	continue;
		//		//}
		//		auto what = m_dsgir.m_ftcs->m_arr.find(fr->m_frid.m_nftc);
		//		pugi::xml_node member = parentNode.append_child("member");
		//		CString featureAcronym = m_dsgir.m_ftcs->m_arr.find(fr->m_frid.m_nftc)->second->m_code;
		//		auto featureElementName = productNamespace + ":" + pugi::as_utf8(std::wstring(featureAcronym));
		//		pugi::xml_node pFeatureNode = member.append_child(featureElementName.c_str());
		//		std::string iid = get_feature_id_string(fr->m_frid.m_name.RCID);
		//		pFeatureNode.append_attribute("gml:id") = iid.c_str();
		//		objectPugiXmlElementMap.insert({ iid, &pFeatureNode });
		//		SetAttributeType(document, pFeatureNode, &fr->m_attr);
		//		SetVector(pFeatureNode, fr);
		//	}
		//}
		//else {}
	}

	void S101::SetFeaturesTypeRelation_v2(pugi::xml_node rootNode)
	{
		for (auto itor = vecFeature.begin(); itor != vecFeature.end(); itor++)
		{
			R_FeatureRecord* fr = *(itor);
			std::string srcFeatureID = get_feature_id_string(fr->m_frid.m_name.RCID);

			pugi::xml_node* pFeatureNode = nullptr;
			auto srcFeatureXpath = "/S201:DataSet/member/S201:*[@gml:id='" + srcFeatureID + "']";
			auto srcFeatureElement = rootNode.select_node(srcFeatureXpath.c_str());

			// Source Feature를 찾은 경우
			if (srcFeatureElement.node())
			{
				pFeatureNode = &srcFeatureElement.node();
			}
			// 못 찾은 경우
			else
			{
				continue;
			}

			// Source Feature와 관계있는 모든 피처 연결
			for (auto i = fr->m_fasc.begin(); i != fr->m_fasc.end(); i++)
			{
				F_FASC* f_fasc = *i;
				_int64 key = ((_int64)100) << 32 | f_fasc->m_name.RCID;
				std::string iid = get_feature_id_string(fr->m_frid.m_name.RCID);

				auto dstFeatureXpath = "/S201:DataSet/member/S201:*[@gml:id='" + iid + "']";
				//BaekIS ( select_single_node 는 deprecated 오류가 나서 select_node 사용 )
				auto dstFeatureElement = rootNode.select_node(dstFeatureXpath.c_str());

				//auto itor = objectPugiXmlElementMap.find(iid);

				//if (itor == objectPugiXmlElementMap.end())
				//{
				//	continue;
				//}
				// Destination Feature를 못 찾은 경우
				if (dstFeatureElement.node() == nullptr)
				{
					continue;
				}

				pugi::xml_node* dstFeatureNode = &dstFeatureElement.node();
				std::string dstFeatureID = "#" + iid;

				//pugi::xml_attribute attr = pElement->attribute("gml:id");
				//if (attr)
				//{
				//	eid.append(attr.value());
				//}

				//auto asitor = m_dsgir.m_facs->m_arr.find(f_fasc->m_nfac);
				auto ritor = m_dsgir.m_arcs->m_arr.find(f_fasc->m_nfac);

				auto roleName = ritor->second->m_code;

				//dstFeatureNode = &pFeatureNode->append_child("S100:featureAssociation");
				dstFeatureNode = &pFeatureNode->append_child(pugi::as_utf8(std::wstring(roleName)).c_str());
				//dstFeatureNode->append_attribute("xlink:role") = roleName;
				dstFeatureNode->append_attribute("xlink:href") = dstFeatureID.c_str();
				//오류 발생 가능성있음

				for (auto itorParent = fr->m_inas.begin(); itorParent != fr->m_inas.end(); itorParent++)
				{
					F_INAS* f_inas = *itorParent;
					std::string iid = get_information_id_string(f_inas->m_name.RCID);
					auto itor = objectPugiXmlElementMap.find(iid);
					pugi::xml_node* pElement = itor->second;

					std::string eid = "#";

					pugi::xml_attribute pAttrib;
					pAttrib = pElement->first_attribute();

					if (pAttrib == nullptr)
					{
						return;
					}

					while (pAttrib)
					{
						std::string attrName = pAttrib.name();
						if (attrName.compare("gml:id"))
						{
							eid.append(pAttrib.value());
							break;
						}
						pAttrib = pAttrib.next_attribute();
					}

					auto asitor = m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
					auto ritor = m_dsgir.m_arcs->m_arr.find(f_inas->m_niac);
					auto roleName = ritor->second->m_code;

					pElement = &pFeatureNode->append_child("S100:informationAssociation");
					pElement->append_attribute("xlink:role") = roleName;
					pElement->append_attribute("xlink:href") = eid.c_str();
				}
			}

		}

		//_int64 key = 0;
		//R_FeatureRecord* fr = nullptr;
		//POSITION pos = m_feaMap.GetStartPosition();
		//while (pos != nullptr)
		//{
		//	m_feaMap.GetNextAssoc(pos, key, fr);
		//	//if (!fr->m_geometry)
		//	//{
		//	//	continue;
		//	//}
		//	//CString featureAcronym = GetFeatureCode(fr->GetNumericCode());
		//	std::string srcFeatureID = get_feature_id_string(fr->m_frid.m_name.RCID);
		//	pugi::xml_node* pFeatureNode = nullptr;
		//	auto srcFeatureXpath = "/S201:DataSet/member/S201:*[@gml:id='" + srcFeatureID + "']";
		//	auto srcFeatureElement = rootNode.select_node(srcFeatureXpath.c_str());
		//	// Source Feature를 찾은 경우
		//	if (srcFeatureElement.node())
		//	{
		//		pFeatureNode = &srcFeatureElement.node();
		//	}
		//	// 못 찾은 경우
		//	else
		//	{
		//		continue;
		//	}
		//	// Source Feature와 관계있는 모든 피처 연결
		//	for (auto i = fr->m_fasc.begin(); i != fr->m_fasc.end(); i++)
		//	{
		//		F_FASC* f_fasc = *i;
		//		_int64 key = ((_int64)100) << 32 | f_fasc->m_name.RCID;
		//		std::string iid = get_feature_id_string(fr->m_frid.m_name.RCID);
		//		auto dstFeatureXpath = "/S201:DataSet/member/S201:*[@gml:id='" + iid + "']";
		//		//BaekIS ( select_single_node 는 deprecated 오류가 나서 select_node 사용 )
		//		auto dstFeatureElement = rootNode.select_node(dstFeatureXpath.c_str());
		//		//auto itor = objectPugiXmlElementMap.find(iid);
		//		//if (itor == objectPugiXmlElementMap.end())
		//		//{
		//		//	continue;
		//		//}
		//		// Destination Feature를 못 찾은 경우
		//		if (dstFeatureElement.node() == nullptr)
		//		{
		//			continue;
		//		}
		//		pugi::xml_node* dstFeatureNode = &dstFeatureElement.node();
		//		std::string dstFeatureID = "#" + iid;
		//		//pugi::xml_attribute attr = pElement->attribute("gml:id");
		//		//if (attr)
		//		//{
		//		//	eid.append(attr.value());
		//		//}
		//		//auto asitor = m_dsgir.m_facs->m_arr.find(f_fasc->m_nfac);
		//		auto ritor = m_dsgir.m_arcs->m_arr.find(f_fasc->m_nfac);
		//		auto roleName = ritor->second->m_code;
		//		//dstFeatureNode = &pFeatureNode->append_child("S100:featureAssociation");
		//		dstFeatureNode = &pFeatureNode->append_child(pugi::as_utf8(std::wstring(roleName)).c_str());
		//		//dstFeatureNode->append_attribute("xlink:role") = roleName;
		//		dstFeatureNode->append_attribute("xlink:href") = dstFeatureID.c_str();
		//		//오류 발생 가능성있음
		//		for (auto itorParent = fr->m_inas.begin(); itorParent != fr->m_inas.end(); itorParent++)
		//		{
		//			F_INAS* f_inas = *itorParent;
		//			std::string iid = get_information_id_string(f_inas->m_name.RCID);
		//			auto itor = objectPugiXmlElementMap.find(iid);
		//			pugi::xml_node* pElement = itor->second;
		//			std::string eid = "#";
		//			pugi::xml_attribute pAttrib;
		//			pAttrib = pElement->first_attribute();
		//			if (pAttrib == nullptr)
		//			{
		//				return;
		//			}
		//			while (pAttrib)
		//			{
		//				std::string attrName = pAttrib.name();
		//				if (attrName.compare("gml:id"))
		//				{
		//					eid.append(pAttrib.value());
		//					break;
		//				}
		//				pAttrib = pAttrib.next_attribute();
		//			}
		//			auto asitor = m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
		//			auto ritor = m_dsgir.m_arcs->m_arr.find(f_inas->m_niac);
		//			auto roleName = ritor->second->m_code;
		//			pElement = &pFeatureNode->append_child("S100:informationAssociation");
		//			pElement->append_attribute("xlink:role") = roleName;
		//			pElement->append_attribute("xlink:href") = eid.c_str();
		//		}
		//	}
		//}
	}

	void S101::SetInformationsTypeRelation_v2(pugi::xml_node parentNode)
	{
		//std::unordered_map<int, pugi::xml_node*> attriNodeMap;
		for (auto itor = vecInformation.begin(); itor != vecInformation.end(); itor++)
		{
			R_InformationRecord* ir = *(itor);
			//m_infMap.GetNextAssoc(pos, key, ir);

			std::string iid;
			auto it = objectPugiXmlElementMap.find(iid);
			if (it == objectPugiXmlElementMap.end())
			{
				continue;
			}

			pugi::xml_node* pInformationNode = it->second;

			for (auto itorParent = ir->m_inas.begin(); itorParent != ir->m_inas.end(); itorParent++)
			{
				F_INAS* f_inas = *itorParent;

				std::string iid = get_information_id_string(f_inas->m_name.RCID);

				auto itor = objectPugiXmlElementMap.find(iid);

				pugi::xml_node* pElement = itor->second;
				std::string eid = "#";

				pugi::xml_attribute* pAttrib;
				pAttrib = &pElement->first_attribute();

				while (pAttrib)
				{
					std::string attrName = pAttrib->name();

					if (attrName.compare("gml:id") == 0)
					{
						eid.append(pAttrib->value());
						break;
					}

					pAttrib = &pAttrib->next_attribute();
				}

				auto asitor = m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
				auto ritor = m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);
				auto roleName = ritor->second->m_code;

				pElement = &pInformationNode->append_child("S100:informationAssocination");
				pElement->append_attribute("xlink:role") = roleName;
				pElement->append_attribute("xlink:href") = eid.c_str();
			}
		}




		//_int64 key;
		//R_InformationRecord* ir = nullptr;
		//POSITION pos = m_infMap.GetStartPosition();
		//if (pos != nullptr)
		//{
		//	while (pos != nullptr)
		//	{
		//		m_infMap.GetNextAssoc(pos, key, ir);

		//		std::string iid;
		//		auto itor = objectPugiXmlElementMap.find(iid);
		//		if (itor == objectPugiXmlElementMap.end())
		//		{
		//			continue;
		//		}

		//		pugi::xml_node* pInformationNode = itor->second;

		//		for (auto itorParent = ir->m_inas.begin(); itorParent != ir->m_inas.end(); itorParent++)
		//		{
		//			F_INAS* f_inas = *itorParent;

		//			std::string iid = get_information_id_string(f_inas->m_name.RCID);

		//			auto itor = objectPugiXmlElementMap.find(iid);

		//			pugi::xml_node* pElement = itor->second;
		//			std::string eid = "#";

		//			pugi::xml_attribute* pAttrib;
		//			pAttrib = &pElement->first_attribute();

		//			while (pAttrib)
		//			{
		//				std::string attrName = pAttrib->name();

		//				if (attrName.compare("gml:id") == 0)
		//				{
		//					eid.append(pAttrib->value());
		//					break;
		//				}

		//				pAttrib = &pAttrib->next_attribute();
		//			}

		//			auto asitor = m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
		//			auto ritor = m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);
		//			auto roleName = ritor->second->m_code;

		//			pElement = &pInformationNode->append_child("S100:informationAssocination");
		//			pElement->append_attribute("xlink:role") = roleName;
		//			pElement->append_attribute("xlink:href") = eid.c_str();
		//		}
		//	}
		//}
		//else
		//{
		////	KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_WARNING, L"SetInformation map start position is null", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::None);
		//}
	}

	std::string S101::GetEncodingSpecificationToString()
	{
		return pugi::as_utf8(GetEncodingSpecification());
	}

	CString S101::GetEncodingSpecification()
	{
		return m_dsgir.m_dsid.m_ensp;
	}

	void S101::SetAttributeType(pugi::xml_document* doc, pugi::xml_node parentNode, std::list<F_ATTR*>* f_attrList)
	{
		std::unordered_map<int, pugi::xml_node> attrXmlNodeMap;

		/*auto s100Layer = (S100Layer*)m_pLayer;
		auto catalog = s100Layer->GetFC();
		if (nullptr == catalog)
		{
			return;
		}*/

		//auto fc = catalog->GetFC();

		for (auto itorParent = f_attrList->begin(); itorParent != f_attrList->end(); itorParent++)
		{
			F_ATTR* attr = *itorParent;
			int index = 0;
			attrXmlNodeMap.clear();

			for (auto aitor = attr->m_arr.begin(); aitor != attr->m_arr.end(); aitor++)
			{
				index++;
				ATTR* attr = *aitor;

				auto itor = m_dsgir.m_atcs->m_arr.find(attr->m_natc);
				if (itor == m_dsgir.m_atcs->m_arr.end())
				{
					continue;
				}

				std::wstring attributeName = std::wstring(itor->second->m_code);
				pugi::xml_node pElement = doc->append_child(LibMFCUtil::CStringToString(attributeName.c_str()).c_str());
				//pugi::xml_node pElement= .append_child(LibMFCUtil::CStringToString(attributeName.c_str()).c_str());

				attrXmlNodeMap.insert(std::unordered_map<int, pugi::xml_node>::value_type(index, pElement));
				//attributeVector.insert(std::unordered_map<std::wstring,int>::value_type(attributeName, index)); //테스트

				if (attr->m_paix == 0) //부모의 경우
				{
					parentNode.append_move(pElement);
				}
				else //자식의 값인경우
				{
					auto itor = attrXmlNodeMap.find(attr->m_paix); //지정된 부모를 찾습니다.
					pugi::xml_node parent = itor->second; //그것의 자식값으로 넣습니다.
					parent.append_move(pElement);
				}




				//auto cit = fc->GetComplexAttributesPointer().GetComplexAttributePointer().find(attributeName);
				//auto sit = fc->GetSimpleAttributesPointer().GetSimpleAttributePointer().find(attributeName);

				//if (cit != fc->GetComplexAttributesPointer().GetComplexAttributePointer().end()) //컴플랙스 값일경우
				//{
				//	if (attr->m_paix == 0) //부모의 경우
				//	{
				//		parentNode.append_move(pElement);
				//	}
				//	else //자식의 값인경우
				//	{
				//		auto itor = attrXmlNodeMap.find(attr->m_paix); //지정된 부모를 찾습니다.
				//		pugi::xml_node parent = itor->second; //그것의 자식값으로 넣습니다.
				//		parent.append_move(pElement);
				//	}
				//}
				//else if (sit != fc->GetSimpleAttributesPointer().GetSimpleAttributePointer().end()) //심플값일경우
				//{
				//	std::string inputText = "";
				//	SimpleAttribute* sa = sit->second;

				//	//enum 타입인경우
				//	if (sa->GetValueType() == FCD::S100_CD_AttributeValueType::enumeration)  //text를 고르는 심플값일경우
				//	{
				//		for (auto itor = sa->GetListedValuesPointer().begin(); itor != sa->GetListedValuesPointer().end(); itor++)
				//		{
				//			ListedValues* lvs = &(*itor);
				//			for (auto lvitor = lvs->GetListedValuePointer().begin(); lvitor != lvs->GetListedValuePointer().end(); lvitor++)
				//			{
				//				ListedValue* lv = &(lvitor->second);
				//				int value = _wtoi(std::wstring(attr->m_atvl).c_str());

				//				if (value == lv->GetCode().GetvalueInteger())
				//				{
				//					inputText = CT2CA(lv->GetLabel().c_str());
				//					break;
				//				}
				//			}
				//		}
				//	}
				//	else //아닌경우
				//	{
				//		inputText = pugi::as_utf8(attr->m_atvl);
				//	}


				//	if (attr->m_paix == 0) //심플 그냥 추가할경우
				//	{
				//		parentNode.append_move(pElement);
				//		pElement.append_child(pugi::node_pcdata).set_value(inputText.c_str());
				//	}
				//	else
				//	{
				//		auto itor = attrXmlNodeMap.find(attr->m_paix);
				//		if (itor != attrXmlNodeMap.end())
				//		{
				//			pElement.append_child(pugi::node_pcdata).set_value(inputText.c_str());
				//			pugi::xml_node parent = itor->second;
				//			auto ischild = parent.append_move(pElement);
				//			if (!ischild)
				//			{
				//				OutputDebugString(L"자식화에 실패했습니다.");
				//			}
				//		}

				//	}
				//}
				//else //심플도 컴플랙스 값도 아닐경우...???
				//{
				//	if (attr->m_paix == 0)
				//	{
				//		parentNode.append_move(pElement);
				//		pElement.append_child(pugi::node_pcdata).set_value(LibMFCUtil::CStringToString(attributeName.c_str()).c_str());
				//	}
				//	else
				//	{
				//		/*pElement.append_child(pugi::node_pcdata).set_value(LibMFCUtil::CStringToString(attr->m_atvl).c_str());
				//		auto itor = attrXmlNodeMap.find(attr->m_paix);
				//		pugi::xml_node *parent = itor->second;
				//		pElement = parent->append_child(LibMFCUtil::CStringToString(attributeName.c_str()).c_str());*/
				//	}
				//}
			}
		}
	}

	void S101::SetVector(pugi::xml_node parentNode, R_FeatureRecord* fr)
	{
		if (fr->m_geometry == nullptr)
		{
			return;
		}

		auto type = fr->m_geometry->type;
		if (type == 1)
		{
			SetVectorPointsType(parentNode, (SPoint*)fr->m_geometry);
		}
		else if (type == 2)
		{
			//SetVectorCurvesType(parentNode, (SCurve*)fr->m_geometry);
			SetVectorCompositeCurvesType(parentNode, (SCompositeCurve*)fr->m_geometry);
		}
		else if (type == 3)
		{
			SetVectorSurfacesType(parentNode, (SSurface*)fr->m_geometry);
		}
		else if (type == 4)
		{
			SetVectorMultiPointsType(parentNode, (SMultiPoint*)fr->m_geometry);
		}
	}

	void S101::SetVectorPointsType(pugi::xml_node parentNode, SPoint* p)
	{
		std::wstring coordinateString;
		double x, y;
		x = p->m_point.x;
		y = p->m_point.y;
		inverseProjection(x, y);

		std::wstring strX = get_wstring_from_coordinate_1((int)(x * m_dsgir.m_dssi.m_cmfx));
		std::wstring strY = get_wstring_from_coordinate_1((int)(y * m_dsgir.m_dssi.m_cmfy));

		coordinateString.append(strX);
		coordinateString.append(L" ");
		coordinateString.append(strY);

		pugi::xml_node geometry = parentNode.append_child("geometry");
		pugi::xml_node pointProperty = geometry.append_child("S100:pointProperty");
		static int poiontID = 1;
		pugi::xml_node point = pointProperty.append_child("S100:Point");
		point.append_attribute("gml:id") = poiontID++;
		pugi::xml_node pos = point.append_child("gml:pos");
		pos.append_child(pugi::node_pcdata).set_value(LibMFCUtil::WStringToString(coordinateString).c_str());
	}

	void S101::SetVectorMultiPointsType(pugi::xml_node parentNode, SMultiPoint* p)
	{

	}

	void S101::SetVectorCurvesType(pugi::xml_node parentNode, SCurve* p)
	{
		std::wstring coordinateString;
		for (int i = 0; i < p->m_numPoints; i++)
		{
			double x, y;

			x = p->m_pPoints[i].x;
			y = p->m_pPoints[i].y;
			inverseProjection(x, y);

			std::wstring strX = get_wstring_from_coordinate_1((int)(x * m_dsgir.m_dssi.m_cmfx));
			std::wstring strY = get_wstring_from_coordinate_1((int)(y * m_dsgir.m_dssi.m_cmfx));

			coordinateString.append(strX);
			coordinateString.append(L" ");
			coordinateString.append(strY);

			if (i = !p->m_numPoints)
			{
				coordinateString.append(L" ");
			}
		}

		pugi::xml_node pGeometry = parentNode.append_child("geometry");
		pugi::xml_node pCurveProperty = pGeometry.append_child("S100:curveProperty");
		pugi::xml_node pCurve = pCurveProperty.append_child("gml:Curve");
		pCurve.append_attribute("srsName") = "http://www.opengis.net/gml/srs/epsg.xml#4326";

		static int curveID = 1;

		pCurve.append_attribute("gml:id") = curveID++;

		pugi::xml_node pSegments = pCurve.append_child("gml:segments");
		pugi::xml_node pLineString = pSegments.append_child("gml:LineString");
		pugi::xml_node pPosList = pLineString.append_child("gml:posList");
		pPosList.append_child(pugi::node_pcdata).set_value(LibMFCUtil::WStringToString(coordinateString).c_str());
	}

	void S101::SetVectorCompositeCurvesType(pugi::xml_node parentNode, SCompositeCurve* p)
	{
		auto curvelist = &p->m_listCurveLink;
		//auto curveListCount = curvelist.size();

		//for (size_t i = 0; i < curveListCount; i++)
		for (auto iter = curvelist->begin(); iter != curvelist->end(); iter++)
		{
			SCurveHasOrient* curveOrient = &(*iter);

			pugi::xml_node pGeometry = parentNode.append_child("geometry");
			pugi::xml_node pCurveProperty = pGeometry.append_child("S100:curveProperty");
			pugi::xml_node pCurve = pCurveProperty.append_child("gml:Curve");
			pCurve.append_attribute("srsName") = "http://www.opengis.net/gml/srs/epsg.xml#4326";
			static int curveID = 1;
			std::wstring coordinateString;

			//int numPoint = curve->GetNumPoints();
			//여기서 무한루프가 돕니다.
			auto curve = curveOrient->GetCurve();
			if (curveOrient->GetMasking() == true)
			{
				//for (int j = 0; j < numPoint; j++)
				//{
				//	double x = curve->GetX(j);
				//	double y = curve->GetY(j);
				//	inverseProjection(x, y);

				//	std::wstring strX = get_wstring_from_coordinate_1((int)(x*m_dsgir.m_dssi.m_cmfx));
				//	std::wstring strY = get_wstring_from_coordinate_1((int)(y*m_dsgir.m_dssi.m_cmfx));

				//	coordinateString.append(strX);
				//	coordinateString.append(L" ");
				//	coordinateString.append(strY);

				//	if (j != curve->m_numPoints -1)
				//	{
				//		coordinateString.append(L" ");
				//	}
				//}
			}
			else if (curveOrient->GetMasking() == false)
			{
				bool curOrient = true;
				bool parentOrient = curveOrient->GetParentOrient();

				if (parentOrient == false)
				{
					curOrient = !curOrient;
				}

				int numPoint = curve->GetNumPoints();
				if (!curOrient)
				{
					for (int j = numPoint - 1; j >= 0; j--)
					{
						double x = curve->GetX(j);
						double y = curve->GetY(j);
						inverseProjection(x, y);

						std::wstring strX = get_wstring_from_coordinate_1((int)(x * m_dsgir.m_dssi.m_cmfx));
						std::wstring strY = get_wstring_from_coordinate_1((int)(y * m_dsgir.m_dssi.m_cmfx));

						coordinateString.append(strX);
						coordinateString.append(L" ");
						coordinateString.append(strY);

						if (j != 0)
						{
							coordinateString.append(L" ");
						}
					}
				}
				else
				{
					for (int j = 0; j < numPoint - 1; j++)
					{
						double x = curve->GetX(j);
						double y = curve->GetY(j);
						inverseProjection(x, y);

						std::wstring strX = get_wstring_from_coordinate_1((int)(x * m_dsgir.m_dssi.m_cmfx));
						std::wstring strY = get_wstring_from_coordinate_1((int)(y * m_dsgir.m_dssi.m_cmfx));

						coordinateString.append(strX);
						coordinateString.append(L" ");
						coordinateString.append(strY);

						if (j != numPoint - 1)
						{
							coordinateString.append(L" ");
						}
					}
				}
			}


			pCurve.append_attribute("gml:id") = curveID++;

			pugi::xml_node pSegments = pCurve.append_child("gml:segments");
			pugi::xml_node pLineString = pSegments.append_child("gml:LineString");
			pugi::xml_node pPosList = pLineString.append_child("gml:posList");
			pPosList.append_child(pugi::node_pcdata).set_value(LibMFCUtil::WStringToString(coordinateString).c_str());
		}
	}

	void S101::SetVectorSurfacesType(pugi::xml_node parentnode, SSurface* p)
	{
		std::wstring outboundCoordinateString;
		std::vector<std::wstring> inboundCoorinateStringList;

#pragma region 이전코드
		//for (int i = 0; i < p->m_numPoints; i++)
		//{
		//	double x, y;
		//	x = p->m_pPoints[i].x;
		//	y = p->m_pPoints[i].y;

		//	inverseProjection(x, y);

		//	std::wstring strX = get_wstring_from_coordinate_1((int)(x*m_dsgir.m_dssi.m_cmfx));
		//	std::wstring strY = get_wstring_from_coordinate_1((int)(y*m_dsgir.m_dssi.m_cmfy));

		//	coordinateString.append(strX);
		//	coordinateString.append(L" ");
		//	coordinateString.append(strY);

		//	if (i != p->m_numPoints)
		//	{
		//		coordinateString.append(L" ");
		//	}
		//}
#pragma endregion

		for (int pi = 0; pi < p->m_numParts; pi++)
		{
			int curMaxIndex = 0;

			if (pi + 1 == p->m_numParts)
			{
				curMaxIndex = p->m_numPoints;
			}
			else
			{
				curMaxIndex = p->m_pParts[pi + 1];
			}

			if (pi == 0)
			{
				for (int i = 0; i < curMaxIndex; i++)
				{
					double x, y;
					x = p->m_pPoints[i].x;
					y = p->m_pPoints[i].y;

					inverseProjection(x, y);
					std::wstring strX = get_wstring_from_coordinate_1((int)(x * m_dsgir.m_dssi.m_cmfx));
					std::wstring strY = get_wstring_from_coordinate_1((int)(y * m_dsgir.m_dssi.m_cmfy));

					outboundCoordinateString.append(strX);
					outboundCoordinateString.append(L" ");
					outboundCoordinateString.append(strY);

					if (i != p->m_numPoints)
					{
						outboundCoordinateString.append(L" ");
					}
				}
			}
			else
			{
				std::wstring inboundCoordinateString;
				if (pi == p->m_numParts - 1)
				{
					curMaxIndex++;
				}

				for (int i = p->m_pParts[pi]; i < curMaxIndex - 1; i++)
				{
					double x, y;
					x = p->m_pPoints[i].x;
					y = p->m_pPoints[i].y;

					inverseProjection(x, y);

					std::wstring strX = get_wstring_from_coordinate_1((int)(x * m_dsgir.m_dssi.m_cmfx));
					std::wstring strY = get_wstring_from_coordinate_1((int)(y * m_dsgir.m_dssi.m_cmfy));

					inboundCoordinateString.append(strX);
					inboundCoordinateString.append(L" ");
					inboundCoordinateString.append(strY);

					if (i != p->m_numPoints)
					{
						inboundCoordinateString.append(L" ");
					}
				}
				inboundCoorinateStringList.push_back(inboundCoordinateString);
			}
		}


		pugi::xml_node pGeometry = parentnode.append_child("geometry");
		pugi::xml_node pSurfaceProperty = pGeometry.append_child("S100:surfaceProperty");

		static int surfaceID = 1;
		pugi::xml_node pSurface = pSurfaceProperty.append_child("S100:Surface");
		pSurface.append_attribute("gml:id") = surfaceID++;

		pugi::xml_node pPatches = pSurface.append_child("gml:patches");
		pugi::xml_node pPolygonPatch = pPatches.append_child("gml:PolygonPatch");
		pugi::xml_node pExterior = pPolygonPatch.append_child("gml:exterior");
		pugi::xml_node pLinerRing = pExterior.append_child("gml:LinearRing");
		pugi::xml_node pPosList = pLinerRing.append_child("gml:posList");
		pPosList.append_child(pugi::node_pcdata).set_value(LibMFCUtil::WStringToString(outboundCoordinateString).c_str());

		for (unsigned i = 0; i < inboundCoorinateStringList.size(); i++)
		{
			std::wstring ibc = inboundCoorinateStringList[i];

			pugi::xml_node pIterior = pPolygonPatch.append_child("gml:interior");
			pugi::xml_node pLinearRing = pIterior.append_child("gml:LinearRing");
			pugi::xml_node posList = pLinearRing.append_child("gml:posList");
			posList.append_child(pugi::node_pcdata).set_value(LibMFCUtil::WStringToString(ibc).c_str());
		}
	}

	void  S101::InsertInformationRecord(__int64 key, R_InformationRecord* record)
	{
		vecInformation.push_back(record);
	}

	void  S101::InsertPointRecord(__int64 key, R_PointRecord* record)
	{
		;
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

	BOOL S101::ReadDDR(BYTE*& buf)
	{
		int size = atoi(buf, 5);
		buf -= 5;

		m_S101DDR.SetBytes(size, buf);

		buf += m_S101DDR.GetSize();

		return TRUE;
	}

	std::string S101::GetEncodingSpecificationEditionToString()
	{
		return pugi::as_utf8(GetEncodingSpecificationEdition());
	}

	CString S101::GetProductIdentifier()
	{
		return m_dsgir.m_dsid.m_prsp;
	}

	std::string S101::GetProductIdentifierToString()
	{
		return pugi::as_utf8(GetProductIdentifier());
	}

	CString S101::GetProductEdition()
	{
		return m_dsgir.m_dsid.m_pred;
	}

	std::string S101::GetProductEditionToString()
	{
		return pugi::as_utf8(GetProductEdition());
	}

	std::string S101::GetDatasetReferenceDataToString()
	{
		return pugi::as_utf8(GetDatasetReferenceDate());
	}

	CString S101::GetApplicationProfile()
	{
		return m_dsgir.m_dsid.m_prof;
	}

	std::string S101::GetApplicationProfileToString()
	{
		return pugi::as_utf8(GetApplicationProfile());
	}

	std::string S101::GetDatasetEditionToString()
	{
		return  pugi::as_utf8(GetDatasetEdition());
	}

	std::string S101::GetDatasetFileIdentifierToString()
	{
		return pugi::as_utf8(std::wstring(GetDatasetFileIdentifier()));
	}


	CString S101::GetDatasetFileIdentifier()
	{
		return m_dsgir.m_dsid.m_dsnm;
	}

	CString S101::GetDatasetReferenceDate()
	{
		return m_dsgir.m_dsid.m_dsrd;
	}

	//std::string S101::GetDatasetEditionToString()
	//{
	//	return  pugi::as_utf8(GetDatasetEdition());
	//}

	CString S101::GetDatasetEdition()
	{
		return m_dsgir.m_dsid.m_dsed;
	}


	CString S101::GetEncodingSpecificationEdition()
	{
		return m_dsgir.m_dsid.m_ened;
	}

	CString S101::GetDatasetTitle()
	{
		return m_dsgir.m_dsid.m_dstl;
	}
	std::string S101::GetDatasetTitleToString()
	{
		return pugi::as_utf8(GetDatasetTitle());
	}

	std::string S101::GetDatasetLanguageToString()
	{
		return pugi::as_utf8(GetDatasetLanguage());
	}

	CString S101::GetDatasetLanguage()
	{
		return m_dsgir.m_dsid.m_dslg;
	}


	CString S101::GetDatasetAbstract()
	{
		return m_dsgir.m_dsid.m_dsab;
	}

	std::string S101::GetDatasetAbstractToString()
	{
		return pugi::as_utf8(GetDatasetAbstract());
	}

}