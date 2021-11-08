#pragma once

#include "..\\GeoMetryLibrary\\Geometry.h"

#include <unordered_map>

class SSurface;
class Symbol;
class R_FeatureRecord;
class SENC_SymbolFill;

class SGeometry : public Geometry
{
public:
	SGeometry();
	virtual ~SGeometry();

public:
	static int sizeOfPoint;

	// 메로리 할당과 회수를 최소화 하기 위해 CPoint배열도 가지고 있는다.
	static POINT   *viewPoints;

	// 1 : Point 
	// 2 : Curve
	// 3 : Surface
	// 4 : Multi Point (Sounding)
	int type;

	R_FeatureRecord* m_pFeature = nullptr;

public:
	// 1 : Point 
	// 2 : Curve
	// 3 : Surface
	// 4 : Multi Point (Sounding)
	virtual int GetType() {	return 0; }

	bool IsPoint();
	bool IsCurve();
	bool IsSurface();
	bool IsMultiPoint();
};
