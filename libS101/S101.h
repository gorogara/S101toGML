#pragma once

#include "libS101.h"

#include <string>
#include <vector>

namespace libS101
{

	// aka GISLibrary::S101Cell
	class LIBS101_API S101
	{
	public:
		S101() {}
		virtual ~S101() {}

		bool Open(std::wstring path) { return true; }

	public:
		/*
		// Dataset General Information Record 
		R_DSGIR m_dsgir;

		// Dataset Coordinate Reference System Record 
		// Base : O
		// Update : X
		// Comment : ������Ʈ ������ �� ���� ���ڵ� �̹Ƿ� ������ ������ ���� �ʿ�
		R_DSCRS m_dscrs;

		std::vector<R_InformationRecord*> vecInformation;
		std::vector<R_PointRecord*> vecPoint;
		std::vector<R_MultiPointRecord*> vecMultiPoint;
		std::vector<R_CurveRecord*> vecCurve;
		std::vector<R_CompositeRecord*> vecComposite;
		std::vector<R_SurfaceRecord*> vecSurface;
		std::vector<R_FeatureRecord*> vecFeature;
		*/

	public:
		void Test();
	};

}