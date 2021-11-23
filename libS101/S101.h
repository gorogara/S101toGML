#pragma once

#include "libS101.h"
#include "R_DSGIR.h"
#include "R_DSCRS.h"
#include "S100SpatialObject.h"
#include "DDR.h"
#include "MBR.h"
#include "OrientedCurveRecord.h"
#include "SCurveHasOrient.h"

#include <string>
#include <vector>
#include <list>

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
class GeoPointZ;
class GeoPoint;

namespace libS101
{
	// aka GISLibrary::S101Cell
	class LIBS101_API S101 : public S100SpatialObject
	{
	public:
		S101();
		virtual ~S101();

	public:
		MBR mbr;
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

	private:
		std::unordered_map<__int64, SCurve*> m_curveMap;

	public:
		bool Open(CString _filepath);
		void Save(CString _filepath, CString extend);

	private:
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

		bool MakeFullSpatialData();
		bool MakePointData(R_FeatureRecord* fe);
		bool MakeSoundingData(R_FeatureRecord* fe);
		bool MakeLineData(R_FeatureRecord* fe);
		bool MakeAreaData(R_FeatureRecord* fe);

		SCurve* GetCurveGeometry(R_CurveRecord* r/*, CArray<GeoPoint> &geoArr, unsigned ORNT = 1*/);
		bool SetSCurveList(std::list<OrientedCurveRecord>* inCurveRecordList, std::list<SCurveHasOrient>* outSCurveList);

		bool GetFullCurveData(R_FeatureRecord* fe, R_PointRecord* r, int ornt = 1);
		bool GetFullCurveData(R_FeatureRecord* fe, R_MultiPointRecord* r, int ornt = 1);
		bool GetFullCurveData(R_FeatureRecord* fe, R_CurveRecord* r, int ornt = 1);
		bool GetFullCurveData(R_FeatureRecord* fe, R_CompositeRecord* r, int ornt = 1);
		bool GetFullCurveData(R_FeatureRecord* fe, R_SurfaceRecord* r, int ornt = 1);

		bool GetFullSpatialData(R_PointRecord* r, GeoPoint& geo);
		bool GetFullSpatialData(R_PointRecord* r, GeoPointZ& geo);
		bool GetFullSpatialData(R_MultiPointRecord* r, CArray<GeoPointZ>& geoArr);
		bool GetFullSpatialData(R_CurveRecord* r, CArray<GeoPoint>& geoArr, int ORNT = 1);
		bool GetFullSpatialData(R_CurveRecord* r, std::vector<POINT>& geoArr, int ORNT = 1);
		bool GetFullSpatialData(R_CompositeRecord* r, CArray<GeoPoint>& geoArr, int ORNT = 1);
		bool GetFullSpatialData(R_CompositeRecord* r, std::vector<POINT>& geoArr, int ORNT = 1);
		bool GetFullSpatialData(R_SurfaceRecord* r, CArray<GeoPoint>& geoArr);

		R_MultiPointRecord* findMultiPointRecord(long long value);
		R_PointRecord* findPointRecord(long long value);
		R_CurveRecord* findCurveRecord(long long value);
		R_CompositeRecord* findCompositeRecord(long long value);
		R_SurfaceRecord* findSurfaceRecord(long long value);

		void CalcMBR();
		void ClearCurveMap();

		std::string CStringToString(CString str);
		std::string WStringToString(std::wstring str);
	};
}