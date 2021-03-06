#pragma once

#include "Geometry.h"

class GeoPoint : public Geometry
{
public:
	GeoPoint();
	GeoPoint(double _x, double _y);
	virtual ~GeoPoint();

public:
	double x = 0;
	double y = 0;

public:
	void SetPoint(double _x, double _y);
	double GetX();
	double GetY();

	bool operator == (GeoPoint& p);
};