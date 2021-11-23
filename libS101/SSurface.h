#pragma once
#include "SGeometry.h"
#include "SCurveHasOrient.h"

class Scaler;
class GeoPoint;
class MBR;

class SSurface : public SGeometry
{
public:
	SSurface();
	SSurface(MBR* mbr);
	SSurface(std::vector<POINT>& points, std::vector<int> parts);
	virtual ~SSurface();
	
	// Area ÁÂÇ¥
	int m_numParts = 0;
	int	m_numPoints = 0;
	int* m_pParts = nullptr;
	GeoPoint* m_pPoints = nullptr;
	GeoPoint* m_centerPoint = nullptr;
	
	// Curve ÁÂÇ¥
	std::list<SCurveHasOrient> m_listCurveLink;
};