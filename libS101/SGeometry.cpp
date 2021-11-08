#include "stdafx.h"
#include "SGeometry.h"


int SGeometry::sizeOfPoint = 10;
// 메로리 할당과 회수를 최소화 하기 위해 CPoint배열도 가지고 있는다.
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