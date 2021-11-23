#pragma once

#include "SGeometry.h"
#include "GeoPoint.h"

class SPoint : public SGeometry
{
public:
	SPoint();
	SPoint(double _x, double _y);
	virtual ~SPoint();

public:
	GeoPoint m_point;	
	
	// 메로리 할당과 회수를 최소화 하기 위해 CPoint배열도 가지고 있는다.
	POINT   m_vPoint;

public:
	void SetPoint(double _x, double _y);
};