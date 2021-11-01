#pragma once

#include "libS101.h"
#include "R_DSGIR.h"
#include "R_DSCRS.h"

#include "S100SpatialObject.h"

#include <string>
#include <vector>


class R_InformationRecord;
class R_PointRecord;
class R_MultiPointRecord;
class R_CurveRecord;
class R_CompositeRecord;
class R_SurfaceRecord;
class R_FeatureRecord;

namespace libS101
{
	
	// aka GISLibrary::S101Cell
	class LIBS101_API S101 : public S100SpatialObject
	{
	public:
		S101() {}
		virtual ~S101() {}

		bool Open(std::wstring path);

	public:
		
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
	};

}