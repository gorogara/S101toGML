#pragma once

#include "libS101.h"
#include "R_DSGIR.h"
#include "R_DSCRS.h"
#include "S100SpatialObject.h"
#include "DDR.h"

#include <string>
#include <vector>

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class R_InformationRecord;
class R_PointRecord;
class R_MultiPointRecord;
class R_CurveRecord;
class R_CompositeRecord;
class R_SurfaceRecord;
class R_FeatureRecord;


class SPoint;
class SCurve;
class SSurface;
class SMultiPoint;
class SCompositeCurve;

namespace libS101
{
	// aka GISLibrary::S101Cell
	class LIBS101_API S101 : public S100SpatialObject
	{
	public:
		S101() {}
		virtual ~S101() {}

		bool Open(CString _filepath);
		void Save(CString _filepath, CString extend);
		//void Save(CString _filepath);

		//bool Open(std::string _filepath);

	public:
		DDR m_S101DDR;
		// Dataset General Information Record 
		R_DSGIR m_dsgir;

		// Dataset Coordinate Reference System Record 
		// Base : O
		// Update : X
		// Comment : 업데이트 파일일 땐 없는 레코드 이므로 포인터 등으로 변경 필요
		R_DSCRS m_dscrs;

		std::vector<R_InformationRecord*> vecInformation;
		std::vector<R_PointRecord*> vecPoint;
		std::vector<R_MultiPointRecord*> vecMultiPoint;
		std::vector<R_CurveRecord*> vecCurve;
		std::vector<R_CompositeRecord*> vecComposite;
		std::vector<R_SurfaceRecord*> vecSurface;
		std::vector<R_FeatureRecord*> vecFeature;
		

	public:
		void Test();
		void InsertInformationRecord(__int64 key, R_InformationRecord* record);
		void InsertPointRecord(__int64 key, R_PointRecord* record);
		void InsertMultiPointRecord(__int64 key, R_MultiPointRecord* record);
		void InsertCurveRecord(__int64 key, R_CurveRecord* record);
		void InsertCompositeCurveRecord(__int64 key, R_CompositeRecord* record);
		void InsertSurfaceRecord(__int64 key, R_SurfaceRecord* record);
		void InsertFeatureRecord(__int64 key, R_FeatureRecord* record);

		BOOL ReadDDR(BYTE*& buf);
		void SetInformationsType(pugi::xml_document* doc, pugi::xml_node parentNode, std::string productNamespace);
		void GmlifileMakeByPugi(CString _filePath);
		void SetFeaturesType(pugi::xml_document* document, pugi::xml_node parentNode, std::string productNamespace);
		void SetVector(pugi::xml_node parentNode, R_FeatureRecord* fr);
		void SetVectorPointsType(pugi::xml_node parentNode, SPoint* p);
		void SetVectorMultiPointsType(pugi::xml_node parentNode, SMultiPoint* p);
		void SetVectorCurvesType(pugi::xml_node parentNode, SCurve* p);
		void SetVectorCompositeCurvesType(pugi::xml_node parentNode, SCompositeCurve* p);
		void SetVectorSurfacesType(pugi::xml_node parentNode, SSurface* p);
		void SetFeaturesTypeRelation_v2(pugi::xml_node rootNode);
		void SetInformationsTypeRelation_v2(pugi::xml_node parentNode);
		void SetAttributeType(pugi::xml_document* doc, pugi::xml_node parentNode, std::list<F_ATTR*>* f_attrList);
	
		std::string GetEncodingSpecificationToString();
		CString GetEncodingSpecification();

		std::string GetDatasetTitleToString();
		CString GetDatasetTitle();

		std::string GetDatasetEditionToString();
		CString GetEncodingSpecificationEdition();
		CString GetDatasetEdition();

		CString GetDatasetReferenceDate();
		std::string GetEncodingSpecificationEditionToString();

		CString GetProductIdentifier();
		std::string GetProductIdentifierToString();
		std::string GetDatasetReferenceDataToString();

		CString GetApplicationProfile();
		std::string GetApplicationProfileToString();

		CString GetDatasetFileIdentifier();
		std::string GetDatasetFileIdentifierToString();

		CString GetProductEdition();
		std::string GetProductEditionToString();

		CString GetDatasetLanguage();
		std::string GetDatasetLanguageToString();

		CString GetDatasetAbstract();
		std::string GetDatasetAbstractToString();

	private :
		std::string CStringToString(CString str);
		std::string WStringToString(std::wstring str);
	};
}