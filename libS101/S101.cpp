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
#include "ATTR.h"
#include "F_INAS.h"
#include "F_FASC.h"
#include "CodeWithNumericCode.h"
#include "F_CodeWithNumericCode.h"
#include "SCompositeCurve.h"
#include "SPoint.h"
#include "SSurface.h"
#include "SCurve.h"
#include "SMultiPoint.h"
#include "S100_CD_AttributeValueType.h"
#include "GeoCommonFuc.h"
#include "SCurveHasOrient.h"
#include "F_SPAS.h"
#include "SPAS.h"
#include "GeoPointZ.h"
#include "GeoPoint.h"
#include "RIAS.h"
#include "F_RIAS.h"
#include "F_CUCO.h"
#include "CUCO.h"
#include "F_C3IT.h"
#include "F_C2IT.h"
#include "F_C3IL.h"
#include "C3IL.h"
#include "F_C2IL.h"
#include "PTAS.h"
#include "F_PTAS.h"
#include "IC2D.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

#undef _WINDOWS_
#include <afxext.h>

#include <crtdbg.h>

#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s,__NORMAL_BLOCK,__FILE__,__LINE__)
#endif

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

	S101::S101() {

		_CrtSetBreakAlloc(266);
		_CrtDumpMemoryLeaks();
		//AfxSetAllocStop();
	}

	S101::~S101()
	{

		for (auto itor = vecInformation.begin(); itor != vecInformation.end(); itor++)
		{
			R_InformationRecord* ir = *(itor);
			delete ir;
			ir = nullptr;
		}
		vecInformation.clear();

		for (auto itor = vecPoint.begin(); itor != vecPoint.end(); itor++)
		{
			R_PointRecord* ir = *(itor);
			delete ir;
			ir = nullptr;
		}
		vecPoint.clear();

		for (auto itor = vecMultiPoint.begin(); itor != vecMultiPoint.end(); itor++)
		{
			R_MultiPointRecord* ir = *(itor);
			delete ir;
			ir = nullptr;
		}
		vecMultiPoint.clear();

		for (auto itor = vecCurve.begin(); itor != vecCurve.end(); itor++)
		{
			R_CurveRecord* ir = *(itor);
			delete ir;
			ir = nullptr;
		}
		vecCurve.clear();

		for (auto itor = vecComposite.begin(); itor != vecComposite.end(); itor++)
		{
			R_CompositeRecord* ir = *(itor);
			delete ir;
			ir = nullptr;
		}
		vecComposite.clear();

		for (auto itor = vecSurface.begin(); itor != vecSurface.end(); itor++)
		{
			R_SurfaceRecord* ir = *(itor);
			delete ir;
			ir = nullptr;
		}
		vecSurface.clear();

		for (auto itor = vecFeature.begin(); itor != vecFeature.end(); itor++)
		{
			R_FeatureRecord* ir = *(itor);
			delete ir;
			ir = nullptr;
		}
		vecFeature.clear();

		for (auto iter = m_curveMap.begin(); iter != m_curveMap.end(); iter++)
		{
			SCurve* c = iter->second;
			delete c;
			c = nullptr;
		}
		m_curveMap.clear();

	}



#pragma warning(disable:4018)
	bool S101::Open(CString _filepath) // Dataset 시작, .000 파일읽음
	{

		SetFilePath(_filepath);

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
					continue;
				}

				drDir.ReAllocateDirectory(subFieldCount);

				drDir.ReadDir(drReader, pBuf);

				if (*(pBuf++) != 0x1E)
				{

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
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"MRID"))
				{
					R_MultiPointRecord* r = new R_MultiPointRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertMultiPointRecord(r->m_mrid.m_name.GetName(), r);
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CRID"))
				{
					R_CurveRecord* r = new R_CurveRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertCurveRecord(r->m_crid.m_name.GetName(), r);
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"CCID"))
				{
					R_CompositeRecord* r = new R_CompositeRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertCompositeCurveRecord(r->m_ccid.m_name.GetName(), r);
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"SRID"))
				{
					R_SurfaceRecord* r = new R_SurfaceRecord();
					r->ReadRecord(&drDir, pBuf);

					InsertSurfaceRecord(r->m_srid.m_name.GetName(), r);
				}
				else if (drDir.GetDirectory(0)->tag == *((unsigned int*)"FRID"))
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


			return true;
		}
		else {
			return false;
		}
		return true;
	}

	void S101::Save(CString _filepath, CString extend)
	{
		return GmlifileMakeByPugi(_filepath);
	}

	void S101::GmlifileMakeByPugi(CString _filePath) //PugiXml을 사용하여 파일을 저장합니다.
	{
		pugi::xml_document* doc = new pugi::xml_document();
		auto declarationNode = doc->append_child(pugi::node_declaration);
		declarationNode.append_attribute("version") = "1.0";
		declarationNode.append_attribute("encoding") = "UTF-8";

		pugi::xml_node root;
		std::string productNamespace; // ex) S101, S124, S201 ...

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

		std::string fileName(CStringToString(GetFileName()));
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

		SetInformationsType(doc, root, productNamespace);
		SetFeaturesType(doc, root, productNamespace);

#pragma region RELATION
		SetFeaturesTypeRelation_v2(root);
		SetInformationsTypeRelation_v2(root);
#pragma endregion 

		try {
			auto xmlSaveError = doc->save_file(_filePath);
		}
		catch (int e)
		{
			std::cout << "Save Fail" << std::endl;
		}

		delete doc;
	}

	void S101::SetInformationsType(pugi::xml_document* doc, pugi::xml_node parentNode, std::string productNamespace)
	{
		for (auto itor = vecInformation.begin(); itor != vecInformation.end(); itor++)
		{
			R_InformationRecord* ir = *(itor);
			CString informationAcronym = m_dsgir.m_itcs->m_arr.find(ir->m_irid.m_nitc)->second->m_code;
			pugi::xml_node imember = parentNode.append_child("imember");

			auto featureElementName = productNamespace + ":" + CStringToString(informationAcronym);
			pugi::xml_node informationNode = imember.append_child(featureElementName.c_str());

			std::string iid = get_information_id_string(ir->m_irid.m_name.RCID);

			informationNode.append_attribute("gml:id") = iid.c_str();

			objectPugiXmlElementMap.insert(std::unordered_map<std::string, pugi::xml_node*>::value_type(iid, &informationNode));
			SetAttributeType(doc, informationNode, &ir->m_attr);
		}
	}

	void S101::SetFeaturesType(pugi::xml_document* document, pugi::xml_node parentNode, std::string productNamespace)
	{
		for (auto itor = vecFeature.begin(); itor != vecFeature.end(); itor++)
		{
			R_FeatureRecord* fr = *(itor);
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
	}

	void S101::SetFeaturesTypeRelation_v2(pugi::xml_node rootNode)
	{
		pugi::xml_node* pFeatureNode = nullptr;
		for (auto itor = vecFeature.begin(); itor != vecFeature.end(); itor++)
		{
			R_FeatureRecord* fr = *(itor);
			std::string srcFeatureID = get_feature_id_string(fr->m_frid.m_name.RCID);

			auto srcFeatureXpath = "/S201:DataSet/member/S201:*[@gml:id='" + srcFeatureID + "']";
			auto srcFeatureElement = rootNode.select_node(srcFeatureXpath.c_str());


			if (srcFeatureElement.node())
			{
				auto featureNode = srcFeatureElement.node();
				pFeatureNode = &featureNode;
			}
			else
			{
				continue;
			}

			for (auto i = fr->m_fasc.begin(); i != fr->m_fasc.end(); i++)
			{
				F_FASC* f_fasc = *i;
				_int64 key = ((_int64)100) << 32 | f_fasc->m_name.RCID;
				std::string iid = get_feature_id_string(fr->m_frid.m_name.RCID);

				auto dstFeatureXpath = "/S201:DataSet/member/S201:*[@gml:id='" + iid + "']";

				auto dstFeatureElement = rootNode.select_node(dstFeatureXpath.c_str());
				if (dstFeatureElement.node() == nullptr)
				{
					continue;
				}

				auto node = dstFeatureElement.node();

				pugi::xml_node* dstFeatureNode = &node;
				std::string dstFeatureID = "#" + iid;


				auto ritor = m_dsgir.m_arcs->m_arr.find(f_fasc->m_nfac);

				auto roleName = ritor->second->m_code;

				auto roleNode = pFeatureNode->append_child(pugi::as_utf8(std::wstring(roleName)).c_str());
				dstFeatureNode = &roleNode;
				dstFeatureNode->append_attribute("xlink:href") = dstFeatureID.c_str();


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

					auto node = pFeatureNode->append_child("S100:informationAssociation");
					pElement = &node;
					pElement->append_attribute("xlink:role") = roleName;
					pElement->append_attribute("xlink:href") = eid.c_str();
				}
			}

		}
	}

	void S101::SetInformationsTypeRelation_v2(pugi::xml_node parentNode)
	{
		for (auto itor = vecInformation.begin(); itor != vecInformation.end(); itor++)
		{
			R_InformationRecord* ir = *(itor);

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
				auto node = pElement->first_attribute();
				pAttrib = &node;

				while (pAttrib)
				{
					std::string attrName = pAttrib->name();

					if (attrName.compare("gml:id") == 0)
					{
						eid.append(pAttrib->value());
						break;
					}

					auto node = pAttrib->next_attribute();
					pAttrib = &node;
				}

				auto asitor = m_dsgir.m_iacs->m_arr.find(f_inas->m_niac);
				auto ritor = m_dsgir.m_arcs->m_arr.find(f_inas->m_narc);
				auto roleName = ritor->second->m_code;

				auto infoAsscoiNode = pInformationNode->append_child("S100:informationAssocination");
				pElement = &infoAsscoiNode;
				pElement->append_attribute("xlink:role") = roleName;
				pElement->append_attribute("xlink:href") = eid.c_str();
			}
		}
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



				pugi::xml_node pElement = doc->append_child(CStringToString(attributeName.c_str()).c_str());
				//pugi::xml_node pElement= .append_child(LibMFCUtil::CStringToString(attributeName.c_str()).c_str());

				attrXmlNodeMap.insert(std::unordered_map<int, pugi::xml_node>::value_type(index, pElement));
				//attributeVector.insert(std::unordered_map<std::wstring,int>::value_type(attributeName, index)); //테스트

				if (attr->m_atvl.IsEmpty()) //true 일 경우 complex
				{
					if (attr->m_paix == 0)
					{
						parentNode.append_move(pElement);
					}
					else
					{
						auto itor = attrXmlNodeMap.find(attr->m_paix); //지정된 부모를 찾습니다.
						pugi::xml_node parent = itor->second; //그것의 자식값으로 넣습니다.
						parent.append_move(pElement);
					}
				}
				else //false 일 경우 simple
				{
					std::string inputText = "";
					inputText = pugi::as_utf8(attr->m_atvl);

					if (attr->m_paix == 0) //심플 그냥 추가할경우
					{
						parentNode.append_move(pElement);
						pElement.append_child(pugi::node_pcdata).set_value(inputText.c_str());
					}
					else
					{
						auto itor = attrXmlNodeMap.find(attr->m_paix);
						if (itor != attrXmlNodeMap.end())
						{
							pElement.append_child(pugi::node_pcdata).set_value(inputText.c_str());
							pugi::xml_node parent = itor->second;
							auto ischild = parent.append_move(pElement);
							if (!ischild)
							{
								OutputDebugString(L"자식화에 실패했습니다.");
							}
						}

					}
				}
				//if (attr->m_paix == 0) //부모의 경우
				//{
				//	parentNode.append_move(pElement);
				//	auto inputText = pugi::as_utf8(attr->m_atvl);

				//	int i = 0;
				//	//pElement.set_value()
				//}
				//else //자식의 값인경우
				//{
				//	auto itor = attrXmlNodeMap.find(attr->m_paix); //지정된 부모를 찾습니다.
				//	pugi::xml_node parent = itor->second; //그것의 자식값으로 넣습니다.
				//	parent.append_move(pElement);
				//}
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
		point.append_attribute("gml:id").set_value((poiontID++));

		pugi::xml_node pos = point.append_child("gml:pos");
		pos.append_child(pugi::node_pcdata).set_value(WStringToString(coordinateString).c_str());
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
		pPosList.append_child(pugi::node_pcdata).set_value(WStringToString(coordinateString).c_str());
	}

	void S101::SetVectorCompositeCurvesType(pugi::xml_node parentNode, SCompositeCurve* p)
	{
		auto curvelist = &p->m_listCurveLink;
		for (auto iter = curvelist->begin(); iter != curvelist->end(); iter++)
		{
			SCurveHasOrient* curveOrient = &(*iter);

			pugi::xml_node pGeometry = parentNode.append_child("geometry");
			pugi::xml_node pCurveProperty = pGeometry.append_child("S100:curveProperty");
			pugi::xml_node pCurve = pCurveProperty.append_child("gml:Curve");
			pCurve.append_attribute("srsName") = "http://www.opengis.net/gml/srs/epsg.xml#4326";
			static int curveID = 1;
			std::wstring coordinateString;


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
			pPosList.append_child(pugi::node_pcdata).set_value(WStringToString(coordinateString).c_str());
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
		pPosList.append_child(pugi::node_pcdata).set_value(WStringToString(outboundCoordinateString).c_str());

		for (unsigned i = 0; i < inboundCoorinateStringList.size(); i++)
		{
			std::wstring ibc = inboundCoorinateStringList[i];

			pugi::xml_node pIterior = pPolygonPatch.append_child("gml:interior");
			pugi::xml_node pLinearRing = pIterior.append_child("gml:LinearRing");
			pugi::xml_node posList = pLinearRing.append_child("gml:posList");
			posList.append_child(pugi::node_pcdata).set_value(WStringToString(ibc).c_str());
		}
	}

	void  S101::InsertInformationRecord(__int64 key, R_InformationRecord* record)
	{
		vecInformation.push_back(record);
	}

	void  S101::InsertPointRecord(__int64 key, R_PointRecord* record)
	{
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

	bool S101::MakeFullSpatialData()
	{
		ClearCurveMap();

		POSITION spasPos = NULL;
		R_FeatureRecord* fr;

		for (auto itor = vecFeature.begin(); itor != vecFeature.end(); itor++)
		{
			fr = *(itor);
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
		}
		return true;
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

				r = findPointRecord(iKey);

				if (r != nullptr) {
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

				r = findMultiPointRecord(iKey);
				if (r != nullptr) {
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

		//여기서 메모리 누수
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

				ccr = findCompositeRecord(iKey);
				if (ccr != nullptr)
				{
					GetFullCurveData(fe, ccr, spas->m_ornt);
				}

				cr = findCurveRecord(iKey);
				if (cr != nullptr)
				{
					GetFullCurveData(fe, cr, spas->m_ornt);
				}
			}
		}

		SCompositeCurve* scc = new SCompositeCurve();
		fe->m_geometry = scc;

		SetSCurveList(&fe->m_curveList, &scc->m_listCurveLink);

		scc->SetMBR();

		return TRUE;
	}

	bool S101::SetSCurveList(std::list<OrientedCurveRecord>* inCurveRecordList, std::list<SCurveHasOrient>* outSCurveList)
	{
		for (auto c = inCurveRecordList->begin(); c != inCurveRecordList->end(); c++)
		{
			OrientedCurveRecord* ocr = &(*c);

			__int64 iKey = ((__int64)ocr->m_pCurveRecord->m_crid.m_name.RCNM) << 32 | ocr->m_pCurveRecord->m_crid.m_name.RCID;
			auto curveIter = m_curveMap.find(iKey);

			bool bOrnt = ocr->m_orient == 1 ? true : false;

			if (curveIter != m_curveMap.end())
			{
				SCurveHasOrient curveHasOrient(bOrnt, curveIter->second);
				outSCurveList->push_back(curveHasOrient);
			}
			else
			{
				SCurve* pCurve = GetCurveGeometry(ocr->m_pCurveRecord/*, geoArr, ocr->m_orient*/);
				pCurve->m_id = iKey;
				SCurveHasOrient curveHasOrient(bOrnt, pCurve);
				outSCurveList->push_back(curveHasOrient);

				m_curveMap.insert({ iKey, pCurve });
			}
		}
		return true;
	}

	SCurve* S101::GetCurveGeometry(R_CurveRecord* r/*, CArray<GeoPoint> &geoArr, unsigned ORNT*/)
	{
		POSITION ptasPos = NULL;
		PTAS* ptas = NULL;
		IC2D* c2di = NULL;
		R_PointRecord* spr = nullptr, * epr = nullptr;
		GeoPoint gp;
		__int64 iKey;
		int coordinateIndex = 0;

		for (auto i = r->m_ptas->m_arr.begin(); i != r->m_ptas->m_arr.end(); i++)
		{
			auto ptas = *i;

			iKey = ((__int64)ptas->m_name.RCNM) << 32 | ptas->m_name.RCID;
			if (ptas->m_topi == 1 /*&& ORNT == 1 ||*/	// Beginning node , forward
				/*ptas->m_topi == 2 && ORNT == 2*/		// End node, reverse
				)
			{
				spr = findPointRecord(iKey);
			}
			else if (/*ptas->m_topi == 1 && ORNT == 2 ||*/	// Beginning node , reverse
				ptas->m_topi == 2 /*&& ORNT == 1*/		// End node, forward
				)
			{
				epr = findPointRecord(iKey);
			}
			else if (ptas->m_topi == 3)
			{
				spr = findPointRecord(iKey);
				epr = spr;
			}
		}

		SCurve* retCurve = new SCurve();

		int totalCoordinateCount = 2;
		for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
		{
			totalCoordinateCount += (int)(*itorParent)->m_arr.size();
		}

		if (totalCoordinateCount > SGeometry::sizeOfPoint)
		{
			SGeometry::sizeOfPoint = totalCoordinateCount;
			delete SGeometry::viewPoints;
			SGeometry::viewPoints = new CPoint[int(SGeometry::sizeOfPoint * 1.5)];
		}

		retCurve->m_numPoints = totalCoordinateCount;
		retCurve->m_pPoints = new GeoPoint[totalCoordinateCount];

		double x = spr->m_c2it->m_xcoo;// > double(180. * m_dsgir.m_dssi.m_cmfx) ? double(180. * m_dsgir.m_dssi.m_cmfx) - spr->m_c2it->m_xcoo : spr->m_c2it->m_xcoo;
		double y = spr->m_c2it->m_ycoo;// > double(180. * m_dsgir.m_dssi.m_cmfy) ? double(180. * m_dsgir.m_dssi.m_cmfy) - spr->m_c2it->m_ycoo : spr->m_c2it->m_ycoo;

		gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
			y / (double)m_dsgir.m_dssi.m_cmfy);
		projection(gp.x, gp.y);

		retCurve->m_pPoints[coordinateIndex++].SetPoint(gp.x, gp.y);
		retCurve->m_mbr.CalcMBR(gp.x, gp.y);


		for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
		{
			for (auto itor = (*itorParent)->m_arr.begin(); itor != (*itorParent)->m_arr.end(); itor++)
			{
				IC2D* pIC2D = *itor;

				x = pIC2D->m_xcoo;// > double(180. * m_dsgir.m_dssi.m_cmfx) ? double(180. * m_dsgir.m_dssi.m_cmfx) - pIC2D->m_xcoo : pIC2D->m_xcoo;
				y = pIC2D->m_ycoo;// > double(180. * m_dsgir.m_dssi.m_cmfy) ? double(180. * m_dsgir.m_dssi.m_cmfy) - pIC2D->m_ycoo : pIC2D->m_ycoo;

				gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
					y / (double)m_dsgir.m_dssi.m_cmfy);

				projection(gp.x, gp.y);

				retCurve->m_pPoints[coordinateIndex++].SetPoint(gp.x, gp.y);
				retCurve->m_mbr.CalcMBR(gp.x, gp.y);
			}
		}


		x = epr->m_c2it->m_xcoo;// > double(180. * m_dsgir.m_dssi.m_cmfx) ? double(180. * m_dsgir.m_dssi.m_cmfx) - spr->m_c2it->m_xcoo : spr->m_c2it->m_xcoo;
		y = epr->m_c2it->m_ycoo;// > double(180. * m_dsgir.m_dssi.m_cmfy) ? double(180. * m_dsgir.m_dssi.m_cmfy) - spr->m_c2it->m_ycoo : spr->m_c2it->m_ycoo;

		gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
			y / (double)m_dsgir.m_dssi.m_cmfy);
		projection(gp.x, gp.y);

		retCurve->m_pPoints[coordinateIndex].SetPoint(gp.x, gp.y);
		retCurve->m_mbr.CalcMBR(gp.x, gp.y);

		return retCurve;
	}


	bool S101::MakeAreaData(R_FeatureRecord* fe)
	{
		fe->m_curveList.clear();

		R_SurfaceRecord* sr;
		__int64 iKey;
		std::vector<POINT> vecPoint;
		std::vector<int> boundaryList;

		for (auto i = fe->m_spas.begin(); i != fe->m_spas.end(); i++)
		{
			F_SPAS* spasParent = *i;

			for (auto j = spasParent->m_arr.begin(); j != spasParent->m_arr.end(); j++)
			{
				SPAS* spas = *j;

				iKey = spas->m_name.GetName();

				sr = findSurfaceRecord(iKey);
				if (sr != nullptr)
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
								cr = findCurveRecord(iKey);
								if (cr != nullptr)
								{
									GetFullCurveData(fe, cr);
									GetFullSpatialData(cr, vecPoint, rias->m_ornt);
								}
							}
							else if (rias->m_name.RCNM == 125)
							{
								R_CompositeRecord* ccr = nullptr;
								ccr = findCompositeRecord(iKey);
								if (ccr != nullptr)
								{
									GetFullCurveData(fe, ccr);
									GetFullSpatialData(ccr, vecPoint, rias->m_ornt);
								}
							}

							int sizet = (int)vecPoint.size();
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

		SSurface* geo = ((SSurface*)fe->m_geometry);
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

				sr = findSurfaceRecord(iKey);
				if (sr != nullptr)
				{
					GetFullCurveData(fe, sr);
				}
				ccr = findCompositeRecord(iKey);
				if (ccr != nullptr) {
					GetFullCurveData(fe, ccr);
				}
				cr = findCurveRecord(iKey);
				if (cr != nullptr)
				{
					GetFullCurveData(fe, cr);
				}
			}
		}
		return TRUE;
	}

	void S101::CalcMBR()
	{
		for (auto itor = vecFeature.begin(); itor != vecFeature.end(); itor++)
		{
			R_FeatureRecord* fe = *(itor);
			if (fe->m_geometry)
			{
				if (fe->m_geometry->type == 3) {
					SSurface* pSr = (SSurface*)fe->m_geometry;
					mbr.ReMBR(pSr->m_mbr);
				}
				else if (fe->m_geometry->type == 2) {
					auto geo = (SCompositeCurve*)fe->m_geometry;
					mbr.ReMBR(geo->m_mbr);
				}
				else if (fe->m_geometry->type == 1) {
					SPoint* geo = (SPoint*)fe->m_geometry;
					mbr.ReMBR(geo->m_mbr);
				}
			}
		}
	}

	void S101::ClearCurveMap()
	{
		for (auto iter = m_curveMap.begin(); iter != m_curveMap.end(); iter++)
		{
			SCurve* c = iter->second;
			delete c;
		}
		m_curveMap.clear();
	}

	bool S101::GetFullCurveData(R_FeatureRecord* fe, R_PointRecord* r, int ornt)
	{
		fe->m_pointList.push_back(r);
		return TRUE;
	}

	bool S101::GetFullCurveData(R_FeatureRecord* fe, R_MultiPointRecord* r, int ornt)
	{
		return TRUE;
	}

	bool S101::GetFullCurveData(R_FeatureRecord* fe, R_CurveRecord* r, int ornt)
	{
		OrientedCurveRecord ocr;
		ocr.m_pCurveRecord = r;
		ocr.m_orient = ornt;
		fe->m_curveList.push_back(ocr);
		return TRUE;
	}

	bool S101::GetFullCurveData(R_FeatureRecord* fe, R_CompositeRecord* r, int ornt)
	{
		R_CurveRecord* cr = NULL;
		R_CompositeRecord* ccr = NULL;
		__int64 iKey;

		// forward
		if (1 == ornt)
		{
			for (auto i = r->m_cuco.begin(); i != r->m_cuco.end(); i++)
			{
				F_CUCO* cucoParent = *i;

				for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
				{
					auto cuco = *itor;

					if (cuco->m_ornt == 2)
						ornt = (ornt == 2) ? 1 : 2;

					if (cuco->m_name.RCNM == 120)
					{
						iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
						cr = findCurveRecord(iKey);
						if (cr != nullptr)
						{
							GetFullCurveData(fe, cr, cuco->m_ornt);
						}
					}
					else if (cuco->m_name.RCNM == 125)
					{
						iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
						ccr = findCompositeRecord(iKey);
						if (ccr != nullptr) {
							GetFullCurveData(fe, ccr, cuco->m_ornt);
						}
					}
				}
			}
		}
		else if (2 == ornt)
		{
			for (auto i = r->m_cuco.rbegin(); i != r->m_cuco.rend(); i++)
			{
				F_CUCO* cucoParent = *i;

				for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
				{
					auto cuco = *itor;

					if (cuco->m_ornt == 2)
						ornt = (ornt == 2) ? 1 : 2;

					if (cuco->m_name.RCNM == 120)
					{
						iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
						cr = findCurveRecord(iKey);
						if (cr != nullptr)
						{
							GetFullCurveData(fe, cr, cuco->m_ornt);
						}
					}
					else if (cuco->m_name.RCNM == 125)
					{
						iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
						ccr = findCompositeRecord(iKey);
						if (ccr != nullptr) {
							GetFullCurveData(fe, ccr, cuco->m_ornt);
						}
					}
				}
			}
		}

		return TRUE;
	}

	bool S101::GetFullCurveData(R_FeatureRecord* fe, R_SurfaceRecord* r, int ornt)
	{
		R_CurveRecord* cr = NULL;
		R_CompositeRecord* ccr = NULL;
		__int64 iKey;

		for (auto itorParent = r->m_rias.begin(); itorParent != r->m_rias.end(); itorParent++)
		{
			F_RIAS* riasParent = *itorParent;

			for (auto itor = riasParent->m_arr.begin(); itor != riasParent->m_arr.end(); itor++)
			{
				RIAS* rias = *itor;

				if (rias->m_ornt == 2)
					ornt = (ornt == 2) ? 1 : 2;

				iKey = ((__int64)rias->m_name.RCNM) << 32 | rias->m_name.RCID;
				if (rias->m_name.RCNM == 120)
				{
					cr = findCurveRecord(iKey);
					if (cr != nullptr)
					{
						GetFullCurveData(fe, cr, ornt);
					}
				}
				else if (rias->m_name.RCNM == 125)
				{
					ccr = findCompositeRecord(iKey);
					if (ccr != nullptr) {
						GetFullCurveData(fe, ccr, ornt);
					}
				}
			}
		}
		return TRUE;
	}

	bool S101::GetFullSpatialData(R_PointRecord* r, GeoPointZ& geo)
	{
		double x = r->m_c3it->m_xcoo;
		double y = r->m_c3it->m_ycoo;
		double z = r->m_c3it->m_zcoo;

		if (m_dsgir.m_dssi.m_cmfy && m_dsgir.m_dssi.m_cmfx)
		{
			geo.SetPoint(
				x,
				y,
				z);
		}
		else
		{
			geo.SetPoint(
				x / 10000000.0,
				y / 10000000.0,
				z / 100.);
		}

		return TRUE;
	}

	bool S101::GetFullSpatialData(R_PointRecord* r, GeoPoint& geo)
	{
		double x = r->m_c2it->m_xcoo;
		double y = r->m_c2it->m_ycoo;

		if (m_dsgir.m_dssi.m_cmfy && m_dsgir.m_dssi.m_cmfx)
		{
			geo.SetPoint(
				x / (double)m_dsgir.m_dssi.m_cmfx,
				y / (double)m_dsgir.m_dssi.m_cmfy);
		}
		else
		{
			geo.SetPoint(
				x / 10000000.0,
				y / 10000000.0);
		}
		projection(geo.x, geo.y);
		return TRUE;
	}

	bool S101::GetFullSpatialData(R_MultiPointRecord* r, CArray<GeoPointZ>& geoArr)
	{

		for (auto itor = r->m_c3il.begin(); itor != r->m_c3il.end(); itor++)
		{
			F_C3IL* c3il = *itor;
			for (auto itor = c3il->m_arr.begin(); itor != c3il->m_arr.end(); itor++)
			{
				C3IL* unitC3IL = *itor;

				double x = unitC3IL->m_xcoo;
				double y = unitC3IL->m_ycoo;

				GeoPointZ gpz;
				gpz.SetPoint(
					x / (double)m_dsgir.m_dssi.m_cmfx,
					y / (double)m_dsgir.m_dssi.m_cmfy,
					(unitC3IL->m_zcoo > 0 ? unitC3IL->m_zcoo + 0.5 : unitC3IL->m_zcoo - 0.5) / (double)m_dsgir.m_dssi.m_cmfz);
				projection(gpz.x, gpz.y);

				geoArr.Add(gpz);
			}
		}

		return TRUE;
	}

	bool S101::GetFullSpatialData(R_CurveRecord* r, CArray<GeoPoint>& geoArr, int ORNT)
	{
		POSITION ptasPos = NULL;
		PTAS* ptas = NULL;
		IC2D* c2di = NULL;
		R_PointRecord* spr = nullptr, * epr = nullptr;
		GeoPoint gp;
		__int64 iKey;

		for (auto i = r->m_ptas->m_arr.begin(); i != r->m_ptas->m_arr.end(); i++)
		{
			auto ptas = *i;

			iKey = ((__int64)ptas->m_name.RCNM) << 32 | ptas->m_name.RCID;
			if (ptas->m_topi == 1 && ORNT == 1 ||	// Beginning node , forward
				ptas->m_topi == 2 && ORNT == 2		// End node, reverse
				)
			{
				spr = findPointRecord(iKey);
			}
			else if (ptas->m_topi == 1 && ORNT == 2 ||	// Beginning node , reverse
				ptas->m_topi == 2 && ORNT == 1		// End node, forward
				)
			{
				epr = findPointRecord(iKey);
			}
			else if (ptas->m_topi == 3)
			{
				spr = findPointRecord(iKey);
				epr = spr;
			}
		}

		double x = spr->m_c2it->m_xcoo;
		double y = spr->m_c2it->m_ycoo;

		gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
			y / (double)m_dsgir.m_dssi.m_cmfy);
		projection(gp.x, gp.y);
		geoArr.Add(gp);

		if (ORNT == 1)
		{
			for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
			{
				for (auto itor = (*itorParent)->m_arr.begin(); itor != (*itorParent)->m_arr.end(); itor++)
				{
					IC2D* pIC2D = *itor;

					x = pIC2D->m_xcoo;
					y = pIC2D->m_ycoo;

					gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
						y / (double)m_dsgir.m_dssi.m_cmfy);

					projection(gp.x, gp.y);
					geoArr.Add(gp);
				}
			}
		}
		else if (ORNT == 2)
		{
			for (auto itorParent = r->m_c2il.begin(); itorParent != r->m_c2il.end(); itorParent++)
			{
				for (auto itor = (*itorParent)->m_arr.rbegin(); itor != (*itorParent)->m_arr.rend(); itor++)
				{
					IC2D* pIC2D = *itor;

					x = pIC2D->m_xcoo;
					y = pIC2D->m_ycoo;

					gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
						y / (double)m_dsgir.m_dssi.m_cmfy);

					projection(gp.x, gp.y);
					geoArr.Add(gp);
				}
			}
		}
		x = epr->m_c2it->m_xcoo;
		y = epr->m_c2it->m_ycoo;

		gp.SetPoint(x / (double)m_dsgir.m_dssi.m_cmfx,
			y / (double)m_dsgir.m_dssi.m_cmfy);
		projection(gp.x, gp.y);
		geoArr.Add(gp);

		return TRUE;
	}

	bool S101::GetFullSpatialData(R_CurveRecord* r, std::vector<POINT>& geoArr, int ORNT)
	{
		if (nullptr != r->m_ptas)
		{
			auto countPTAS = r->m_ptas->m_arr.size();

			if (countPTAS != 1 && countPTAS != 2)
			{
				OutputDebugString(L"Invalid count of PTAS of Curve Record\n");
			}

			auto beginPointKey = r->m_ptas->m_arr.front()->m_name.GetName();
			auto endPointKey = r->m_ptas->m_arr.back()->m_name.GetName();

			auto beginPointRecord = findPointRecord(beginPointKey);
			if (nullptr == beginPointRecord || nullptr == beginPointRecord->m_c2it)
			{
				return FALSE;
			}


			auto endPointRecord = findPointRecord(endPointKey);
			if (nullptr == endPointRecord || nullptr == endPointRecord->m_c2it)
			{
				return FALSE;
			}

			// PTAS
			if (1 == ORNT)
			{
				geoArr.push_back({ beginPointRecord->m_c2it->m_xcoo, beginPointRecord->m_c2it->m_ycoo });
			}
			else if (2 == ORNT)
			{
				geoArr.push_back({ endPointRecord->m_c2it->m_xcoo, endPointRecord->m_c2it->m_ycoo });
			}
			else
			{
				OutputDebugString(L"Invalied ORNT\n");
			}

			// C2IL
			if (r->m_c2il.size() == 1)
			{
				// Forward
				if (1 == ORNT)
				{
					for (auto i = r->m_c2il.front()->m_arr.begin(); i != r->m_c2il.front()->m_arr.end(); i++)
					{
						auto segC2IL = *i;
						geoArr.push_back({ segC2IL->m_xcoo, segC2IL->m_ycoo });
					}
				}
				// Reverse
				else if (2 == ORNT)
				{
					for (auto i = r->m_c2il.front()->m_arr.rbegin(); i != r->m_c2il.front()->m_arr.rend(); i++)
					{
						auto segC2IL = *i;
						geoArr.push_back({ segC2IL->m_xcoo, segC2IL->m_ycoo });
					}
				}
				else
				{
					OutputDebugString(L"Invalied ORNT\n");
				}
			}
			else
			{
				OutputDebugString(L"Invalied C2IL count\n");
			}

			// PTAS
			if (1 == ORNT)
			{
				geoArr.push_back({ endPointRecord->m_c2it->m_xcoo, endPointRecord->m_c2it->m_ycoo });
			}
			else if (2 == ORNT)
			{
				geoArr.push_back({ beginPointRecord->m_c2it->m_xcoo, beginPointRecord->m_c2it->m_ycoo });
			}
			else
			{
				OutputDebugString(L"Invalied ORNT\n");
			}
		}

		return TRUE;
	}

	bool S101::GetFullSpatialData(R_CompositeRecord* r, CArray<GeoPoint>& geoArr, int ORNT)
	{
		POSITION cucoPos = NULL;
		CUCO* cuco = NULL;
		R_CurveRecord* cr = NULL;
		R_CompositeRecord* ccr = NULL;
		__int64 iKey;

		if (ORNT == 1)
		{
			for (auto itorParent = r->m_cuco.begin(); itorParent != r->m_cuco.end(); itorParent++)
			{
				F_CUCO* cucoParent = *itorParent;

				for (auto itor = cucoParent->m_arr.begin(); itor != cucoParent->m_arr.end(); itor++)
				{
					cuco = *itor;
					if (cuco->m_name.RCNM == 120)
					{
						iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
						cr = findCurveRecord(iKey);
						if (cr != nullptr)
						{
							GetFullSpatialData(cr, geoArr, cuco->m_ornt);
						}
					}
					else if (cuco->m_name.RCNM == 125)
					{
						iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;
						ccr = findCompositeRecord(iKey);
						if (ccr != nullptr) {
							GetFullSpatialData(ccr, geoArr, cuco->m_ornt);
						}
					}
				}
			}
		}
		else if (ORNT == 2)
		{
			for (auto itorParent = r->m_cuco.begin(); itorParent != r->m_cuco.end(); itorParent++)
			{
				F_CUCO* cucoParent = *itorParent;

				for (auto itor = cucoParent->m_arr.rbegin(); itor != cucoParent->m_arr.rend(); itor++)
				{
					cuco = *itor;
					iKey = ((__int64)cuco->m_name.RCNM) << 32 | cuco->m_name.RCID;

					unsigned ornt = 1;
					if (cuco->m_ornt == 2)
						ornt = 1;
					else
						ornt = 2;

					if (cuco->m_name.RCNM == 120)
					{
						cr = findCurveRecord(iKey);
						if (cr != nullptr)
						{
							GetFullSpatialData(cr, geoArr, ornt);
						}
					}
					else if (cuco->m_name.RCNM == 125)
					{
						ccr = findCompositeRecord(iKey);
						if (ccr != nullptr) {
							GetFullSpatialData(ccr, geoArr, ornt);
						}
					}
				}
			}
		}

		return TRUE;
	}

	bool S101::GetFullSpatialData(R_CompositeRecord* r, std::vector<POINT>& geoArr, int ORNT)
	{
		for (auto i = r->m_cuco.begin(); i != r->m_cuco.end(); i++)
		{
			auto CUCO = *i;
			for (auto j = CUCO->m_arr.begin(); j != CUCO->m_arr.end(); j++)
			{
				auto segCUCO = *j;

				if (segCUCO->m_name.RCNM == 120)
				{

					auto curveRecord = findCurveRecord(segCUCO->m_name.GetName());
					if (nullptr != curveRecord)
					{
						GetFullSpatialData(curveRecord, geoArr, segCUCO->m_ornt);
					}
				}
				else if (segCUCO->m_name.RCNM == 125)
				{

					auto compositeCurveRecord = findCompositeRecord(segCUCO->m_name.GetName());
					if (nullptr != compositeCurveRecord)
					{
						GetFullSpatialData(compositeCurveRecord, geoArr, segCUCO->m_ornt);
					}
				}
				else
				{
					OutputDebugString(L"Invalid RCNM in CUCO\n");
					return FALSE;
				}
			}
		}

		return TRUE;
	}

	bool S101::GetFullSpatialData(R_SurfaceRecord* r, CArray<GeoPoint>& geoArr)
	{
		RIAS* rias = NULL;
		R_CurveRecord* cr = NULL;
		R_CompositeRecord* ccr = NULL;
		__int64 iKey;

		///////////////////////////////
		// for blank interior area
		int sp = 0;
		CList<int> boundaryList;
		BOOL isExtrior = TRUE;
		///////////////////////////////

		for (auto itorParent = r->m_rias.begin(); itorParent != r->m_rias.end(); itorParent++)
		{
			F_RIAS* riasParent = *itorParent;

			for (auto itor = riasParent->m_arr.begin(); itor != riasParent->m_arr.end(); itor++)
			{
				RIAS* rias = *itor;
				if (sp == -1)
				{
					sp = (int)geoArr.GetCount();
				}

				iKey = ((__int64)rias->m_name.RCNM) << 32 | rias->m_name.RCID;
				if (rias->m_name.RCNM == 120)
				{
					cr = findCurveRecord(iKey);
					if (cr != nullptr)
					{
						GetFullSpatialData(cr, geoArr, rias->m_ornt);
					}
				}
				else if (rias->m_name.RCNM == 125)
				{
					ccr = findCompositeRecord(iKey);
					if (ccr != nullptr) {
						GetFullSpatialData(ccr, geoArr, rias->m_ornt);
					}
				}
				// for blank interior area
				int sizet = (int)geoArr.GetCount() - 1;
				if (geoArr[sp].x == geoArr[sizet].x &&
					geoArr[sp].y == geoArr[sizet].y)
				{
					if (isExtrior)
					{
						sp = -1;
						isExtrior = false;
					}
					else
					{
						sp = -1;
						boundaryList.AddTail(sizet + 1);
					}
				}
			}
		}
		// for blank interior area
		int arrIndex = 0;
		int i = 0;
		POSITION boundaryPos = boundaryList.GetHeadPosition();
		int index;
		int count = 0;
		while (boundaryPos != NULL)
		{
			index = boundaryList.GetNext(boundaryPos) + count;
			GeoPoint p(geoArr[0].x, geoArr[0].y);
			geoArr.InsertAt(index, p);
			count++;
		}
		return TRUE;
	}

	R_MultiPointRecord* S101::findMultiPointRecord(long long value) {

		for (auto itor = vecMultiPoint.begin(); itor != vecMultiPoint.end(); itor++) {
			R_MultiPointRecord* r = *(itor);
			if (r->m_mrid.m_name.GetName() == value)
			{
				return r;
			}

		}
		return nullptr;
	}

	R_PointRecord* S101::findPointRecord(long long value)
	{
		for (auto itor = vecPoint.begin(); itor != vecPoint.end(); itor++) {
			R_PointRecord* r = *(itor);
			if (r->m_prid.m_name.GetName() == value)
			{
				return r;
			}

		}
		return nullptr;

	}

	R_CurveRecord* S101::findCurveRecord(long long value)
	{
		for (auto itor = vecCurve.begin(); itor != vecCurve.end(); itor++) {
			R_CurveRecord* r = *(itor);
			if (r->m_crid.m_name.GetName() == value)
			{
				return r;
			}

		}
		return nullptr;
	}

	R_CompositeRecord* S101::findCompositeRecord(long long value)
	{
		for (auto itor = vecComposite.begin(); itor != vecComposite.end(); itor++) {
			R_CompositeRecord* r = *(itor);
			if (r->m_ccid.m_name.GetName() == value)
			{
				return r;
			}

		}
		return nullptr;

	}

	R_SurfaceRecord* S101::findSurfaceRecord(long long value) {

		for (auto itor = vecSurface.begin(); itor != vecSurface.end(); itor++) {
			R_SurfaceRecord* r = *(itor);
			if (r->m_srid.m_name.GetName() == value)
			{
				return r;
			}

		}
		return nullptr;
	}

	std::string S101::CStringToString(CString str)
	{
		CT2CA convertedString(str);
		return std::string(convertedString);
	}

	std::string S101::WStringToString(std::wstring str)
	{
		std::string Value;
		Value.assign(str.begin(), str.end());

		return Value;
	}
}