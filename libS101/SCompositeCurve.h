#pragma once

#include "SGeometry.h"
#include "SCurveHasOrient.h"

class SCompositeCurve : public SGeometry
{
public:
	SCompositeCurve();
	virtual ~SCompositeCurve();

public:
	std::list<SCurveHasOrient> m_listCurveLink;

public:
	void SetMBR();
};