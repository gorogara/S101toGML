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
	
	// �޷θ� �Ҵ�� ȸ���� �ּ�ȭ �ϱ� ���� CPoint�迭�� ������ �ִ´�.
	POINT   m_vPoint;

public:
	void SetPoint(double _x, double _y);
};