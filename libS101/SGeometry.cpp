#include "pch.h"
#include "SGeometry.h"


int SGeometry::sizeOfPoint = 10;
POINT   *SGeometry::viewPoints = new POINT[10];

SGeometry::SGeometry() 
{
}

SGeometry::~SGeometry()
{
}

bool SGeometry::IsPoint()
{
	return type == 1;
}

bool SGeometry::IsCurve()
{
	return type == 2;
}

bool SGeometry::IsSurface()
{
	return type == 3;
}

bool SGeometry::IsMultiPoint()
{
	return type == 4;
}