#include "stdafx.h"
#include "SCommonFuction.h"
#include "..\\GeoMetryLibrary\\Scaler.h"

#include "../S100Geometry/SPoint.h"
#include "../S100Geometry/SCurve.h"
#include "../S100Geometry/SSurface.h"


void SCommonFuction::CalculateCenterOfGravityOfSurface(std::vector<POINT> &vp, SSurface *_surface, CRect *_viewPort, Scaler *pScaler)
{
	std::vector<POINT> m_pvPoints;

	if (_surface->m_numPoints > SGeometry::sizeOfPoint)
	{
		SGeometry::sizeOfPoint = _surface->m_numPoints;

		delete SGeometry::viewPoints;
		SGeometry::viewPoints = new CPoint[int(SGeometry::sizeOfPoint * 1.5)];
	}

	for (int count = 0; count < _surface->m_numPoints; count++)
	{
		pScaler->WorldToDevice(_surface->m_pPoints[count].x, _surface->m_pPoints[count].y, &SGeometry::viewPoints[count].x, &SGeometry::viewPoints[count].y);
	}

	ClipperLib::Paths clipedPolygon = ClipSurface(_surface, _viewPort); // 인자로 넘오온 폴리곤을 클립핑한 결과

	if (clipedPolygon.size() == 0)
	{
		return;
	}

	//for (int i = 0; i < clipedPolygon->m_numParts; i++)
	//for (auto itor = clipedPolygon.begin(); itor != clipedPolygon.end(); itor++)
	//{
	//	vp.push_back(CalculateCenterOfGravityOfSurface(*itor));
	//}

	int indexOfMaxVertexPolygon = 0;
	int curIndex = 0;
	int maxVertex = 0;
	for (auto itor = clipedPolygon.begin(); itor != clipedPolygon.end(); itor++)
	{
		//ClipperLib::Paths 
		auto polygon = *itor;
		
		int curVertex = (int)polygon.size();

		if (curVertex > maxVertex)
		{
			indexOfMaxVertexPolygon = curIndex;
			maxVertex = curVertex;
		}
		curIndex++;
	}

	curIndex = 0;
	for (auto itor = clipedPolygon.begin(); itor != clipedPolygon.end(); itor++)
	{
		if (curIndex == indexOfMaxVertexPolygon)
		{
			vp.push_back(CalculateCenterOfGravityOfSurface(*itor));
		}
		curIndex++;
	}

	return;
}

ClipperLib::Paths SCommonFuction::ClipSurface(SSurface *_surface, CRect *_viewPort)
{
	ClipperLib::Path  polygon, view;
	ClipperLib::Paths polygonPaths, viewPaths;
	ClipperLib::Paths result;
	ClipperLib::Clipper clipper;

	ClipperLib::IntPoint tmp;

	// View port
	tmp.X = _viewPort->left;   // View port의 좌표 지정
	tmp.Y = _viewPort->top;    //
	view.push_back(tmp);	   //
	tmp.X = _viewPort->right;  //
	tmp.Y = _viewPort->top;	   //
	view.push_back(tmp);	   //
	tmp.X = _viewPort->right;  //
	tmp.Y = _viewPort->bottom; //
	view.push_back(tmp);	   //
	tmp.X = _viewPort->left;   //
	tmp.Y = _viewPort->bottom; //
	view.push_back(tmp);	   //
	tmp.X = _viewPort->left;   //
	tmp.Y = _viewPort->top; //
	view.push_back(tmp);	   //

	// Polygon
	int part1PointCount;											// 폴리곤 첫번째 파트의 포인트 개수 구하기
	//
	if (_surface->m_numParts > 1) 									//
	{																//
		part1PointCount = _surface->m_pParts[1] - _surface->m_pParts[0];	//
	}																//
	else 															//
	{																//
		part1PointCount = _surface->m_numPoints;						//
	}																//

	for (int i = 0; i < part1PointCount; i++)
	{
		tmp.X = _surface->SGeometry::viewPoints[i].x;
		tmp.Y = _surface->SGeometry::viewPoints[i].y;
		polygon.push_back(tmp);
	}

	polygonPaths.push_back(polygon);
	viewPaths.push_back(view);

	//clipper.AddPath(polygon, ClipperLib::ptSubject, true);
	//clipper.AddPath(view, ClipperLib::ptClip, true);
	clipper.AddPaths(polygonPaths, ClipperLib::ptSubject, true);
	clipper.AddPaths(viewPaths, ClipperLib::ptClip, true);

	clipper.Execute(ClipperLib::ctIntersection, result, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
	


	return result;
}

POINT SCommonFuction::CalculateCenterOfGravityOfSurface(POINT *_p, int _count)
{
	int j = 0;
	double area = 0;
	double centerX = 0.0;
	double centerY = 0.0;

	double x1, y1, x2, y2, tmpArea;

	for (int i = 0; i < _count; i++)
	{
		j = (i + 1) % _count;

		x1 = _p[i].x;
		y1 = _p[i].y;
		x2 = _p[j].x;
		y2 = _p[j].y;

		tmpArea = ((x1 * y2) - (x2 * y1));

		centerX += ((x1 + x2) * tmpArea);
		centerY += ((y1 + y2) * tmpArea);
		area += tmpArea;
	}

	area *= 0.5;

	centerX = centerX / (6.0 * area);
	centerY = centerY / (6.0 * area);

	POINT returnValue;
	returnValue.x = (LONG)centerX;
	returnValue.y = (LONG)centerY;

	return returnValue;
}

POINT SCommonFuction::CalculateCenterOfGravityOfSurface(ClipperLib::Path polygon)
{
	int j = 0;
	double area = 0;
	double centerX = 0.0;
	double centerY = 0.0;

	ClipperLib::long64 x1, y1, x2, y2, tmpArea;

	int _count = (int)polygon.size();

	//for (int i = 0; i < _count; i++)
	for (auto itor = polygon.begin(); itor != polygon.end(); itor++)
	{
		auto itor2 = itor + 1;
		if (itor2 == polygon.end())
			itor2 = polygon.begin();
		//j = (i + 1) % _count;

		//x1 = _p[i].x;
		//y1 = _p[i].y;
		//x2 = _p[j].x;
		//y2 = _p[j].y;
		x1 = (*itor).X;
		y1 = (*itor).Y;
		x2 = (*itor2).X;
		y2 = (*itor2).Y;

		tmpArea = ((x1 * y2) - (x2 * y1));

		centerX += ((x1 + x2) * tmpArea);
		centerY += ((y1 + y2) * tmpArea);
		area += tmpArea;
	}

	area *= 0.5;

	centerX = centerX / (6.0 * area);
	centerY = centerY / (6.0 * area);

	POINT returnValue;
	returnValue.x = (LONG)centerX;
	returnValue.y = (LONG)centerY;

	return returnValue;
}

double SCommonFuction::GetDistance(POINT _p1, POINT _p2)
{
	POINT p;

	p.x = _p2.x - _p1.x;
	p.y = _p2.y - _p1.y;

	return sqrt((double)((p.x * p.x) + (p.y * p.y)));
}

//double SCommonFuction::GetAngle(POINT _p1, POINT _p2)
//{
//	POINT p;
//	p.x = _p2.x - _p1.x;
//	p.y = _p2.y - _p1.y;
//
//	double d = sqrt(((double)(p.x) * p.x) + (p.y * p.y));
//	double temp = p.x / d;
//
//	if (p.y < 0)
//	{
//		return (acos(-1.0) * 2) - acos(temp);
//	}
//
//	return acos(temp);
//}

POINT SCommonFuction::GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset)
{
	double angle = GetAngle(_p1, _p2);  // 라인이 x축의 양의 방향과 이루는 각도

	POINT returnValue;
	returnValue.x = (LONG)(_p1.x + offset * cos(angle));
	returnValue.y = (LONG)(_p1.y + offset * sin(angle));

	return returnValue;
}

double SCommonFuction::GetDistanceOfCurve(POINT *_p, int _count)
{
	double returnValue = 0;

	for (int i = 1; i < _count; i++)
	{
		returnValue += GetDistance(_p[i - 1], _p[i]);
	}

	return returnValue;
}

inline void swap(int &val1, int &val2)
{
	int t = val1;
	val1 = val2;
	val2 = t;
}

bool SCommonFuction::IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2)
{
	float x12 = (float)(_s1.x - _e1.x);
	float x34 = (float)(_s2.x - _e2.x);
	float y12 = (float)(_s1.y - _e1.y);
	float y34 = (float)(_s2.y - _e2.y);

	float c = x12 * y34 - y12 * x34;

	if (fabs(c) < 0.01)
	{
		// No intersection
		return false;
	}
	else
		return true;

	//if(_s1.x == _e1.x && _s1.y == _e1.y)
	//{
	//	return false;
	//}
	//else if(_s2.x == _e2.x && _s2.y == _e2.y)
	//{
	//	return false;
	//}

	//if(_s1.x > _e1.x) 	{	swap(_s1.x, _e1.x); 	}
	//if(_s1.y > _e1.y) 	{	swap(_s1.y, _e1.y); 	}
	//if(_s2.x > _e2.x) 	{	swap(_s2.x, _e2.x); 	}
	//if(_s2.y > _e2.y) 	{	swap(_s2.y, _e2.y); 	}

	//
	//if(	_s1.y >= _s2.y && _s1.y <= _e2.y	||
	//	_e1.y >= _s2.y && _e1.y <= _e2.y	
	//	)
	//{
	//	if( _s1.x >= _s2.x && _s1.x <= _e2.x	||
	//		_e1.x >= _s2.x && _e1.x <= _e2.x	
	//	)
	//	{
	//		return true;
	//	}
	//}
	//return false;
}

void SCommonFuction::CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect* viewPort)
{
	// Store the values for fast access and easy

	// equations-to-code conversion
	float x1 = (float)_s1.x, x2 = (float)_e1.x, x3 = (float)_s2.x, x4 = (float)_e2.x;
	float y1 = (float)_s1.y, y2 = (float)_e1.y, y3 = (float)_s2.y, y4 = (float)_e2.y;
	float d = (float)(x1 - x2) * (float)(y3 - y4) - (float)(y1 - y2) * (float)(x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return;

	// Get the x and y
	float pre = (float)(x1*y2 - y1*x2), post = (float)(x3*y4 - y3*x4);
	float x = (float)(pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (float)(pre * (y3 - y4) - (y1 - y2) * post) / d;

	if (_s2.x == _s1.x ||
		_s2.x == _s1.x ||
		_s2.y == _s1.y ||
		_s2.y == _s1.y ||
		_e2.x == _e1.x ||
		_e2.x == _e1.x ||
		_e2.y == _e1.y ||
		_e2.y == _e1.y
		)
	{
		return;
	}
	else if (
		viewPort->left	> _s1.x ||
		viewPort->right	< _s1.x ||
		viewPort->bottom	< _s1.y ||
		viewPort->top	> _s1.y)
	{
		_s1.x = (LONG)x;
		_s1.y = (LONG)y;
	}
	else
	{
		_e1.x = (LONG)x;
		_e1.y = (LONG)y;
	}
	// Return the point of intersection
	//Point* ret = new Point();
	//ret->x = x;
	//ret->y = y;

	//return ret;
}

void SCommonFuction::GetViewLineInCurve(POINT *_p, int _count, CRect* viewPort)
{
	POINT p1, p2;
	p1.x =viewPort->left;
	p1.y =viewPort->top;
	p2.x =viewPort->right;
	p2.y =viewPort->top;
	//	if(IsIntersect(_p[0], _p[1], p1, p2))
	if (p1.y > _p[0].y)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	//	if(IsIntersect(_p[_count], _p[_count-1], p1, p2))
	else if (p1.y > _p[_count - 1].y)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}

	p1.x = viewPort->right;
	p1.y = viewPort->top;
	p2.x = viewPort->right;
	p2.y = viewPort->bottom;
	//	if(IsIntersect(_p[0], _p[1], p1, p2))
	if (p1.x < _p[0].x)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	//	if(IsIntersect(_p[_count], _p[_count-1], p1, p2))
	else if (p1.x < _p[_count - 1].x)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}
	p1.x = viewPort->left;
	p1.y = viewPort->bottom;
	p2.x = viewPort->right;
	p2.y = viewPort->bottom;
	//	if(IsIntersect(_p[0], _p[1], p1, p2))
	if (p1.y < _p[0].y)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	//	if(IsIntersect(_p[_count], _p[_count-1], p1, p2))
	else if (p1.y < _p[_count - 1].y)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}
	p1.x = viewPort->left;
	p1.y = viewPort->top;
	p2.x = viewPort->left;
	p2.y = viewPort->bottom;
	//	if(IsIntersect(_p[0], _p[1], p1, p2))
	if (p1.x > _p[0].x)
	{
		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
	}
	//	if(IsIntersect(_p[_count], _p[_count-1], p1, p2))
	else if (p1.x > _p[_count - 1].x)
	{
		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
	}

	return;
}

POINT* SCommonFuction::GetCenterPointOfCurve(POINT *_p, int _count, CRect* viewPort)
{
	if (_count < 2)
	{
		return NULL;
	}
	else
	{
		GetViewLineInCurve(_p, _count, viewPort);
	}

	double centerDistance = GetDistanceOfCurve(_p, _count) / 2; // 폴리라인의 시작점부터 폴리라인의 중간지점 까지의 거리

	if (centerDistance == 0)
	{
		return NULL;
	}

	double accumulatedDistance = 0;								   // 누적거리
	POINT *returnValue = new POINT;

	for (int i = 0; i < (_count - 1); i++)						   // 0번째 점부터 다음 점으로 이동하면서 누적거리를 측정한다
	{
		accumulatedDistance += GetDistance(_p[i], _p[i + 1]);      // 다은점으로 이동하면서 누적거리를 측정

		if (accumulatedDistance >= centerDistance)				   // 누적거리가 중간거리보다 크다면 중간점을 지난것이다.
		{
			*returnValue = GetOffsetPointOnLine(_p[i + 1], _p[i], accumulatedDistance - centerDistance); // 지나온 거리만큼 돌아간다.
			break;
		}
	}

	return returnValue;
}

bool SCommonFuction::Intersect(
	float x1, float y1, float x2, float y2,
	float x3, float y3, float x4, float y4,
	float* intersectionX, float* intersectionY)
{
	// calculate the direction of the lines
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

		// optionally, draw a circle where the lines meet
		*intersectionX = x1 + (uA * (x2 - x1));
		*intersectionY = y1 + (uA * (y2 - y1));
		return true;
	}
	return false;
}


bool SCommonFuction::Intersect(
	float xmin, float ymin, float xmax, float ymax,
	float x1, float y1, float x2, float y2,
	float& intersectionX1, float& intersectionY1,
	float& intersectionX2, float& intersectionY2)
{
	float intersectionX[4] = { 0 };
	float intersectionY[4] = { 0 };
	bool intersect[4] = { false };

	intersect[0] = Intersect(
		xmin, ymin, xmin, ymax, 
		x1, y1, x2, y2,
		&intersectionX[0], &intersectionY[0]);

	intersect[1] = Intersect(
		xmin, ymin, xmax, ymin,
		x1, y1, x2, y2,
		&intersectionX[1], &intersectionY[1]);

	intersect[2] = Intersect(
		xmin, ymax, xmax, ymax,
		x1, y1, x2, y2,
		&intersectionX[2], &intersectionY[2]);

	intersect[3] = Intersect(
		xmax, ymin, xmax, ymax,
		x1, y1, x2, y2,
		&intersectionX[3], &intersectionY[3]);

	if (intersect[0] || intersect[1] || intersect[2] || intersect[3])
	{
		return true;
	}

	return false;
}


//  -1 : 외부
//  1 : 내부
int SCommonFuction::inside(SPoint* point, SSurface* poly, bool applyOption)
{
	unsigned i = 0, ret = 1;

	if (inside(point->m_point.x, point->m_point.y, poly) == -1)
	{
		ret = -1;
	}
	return ret;
}
//  -1 : 외부
//  1 : 내부
int SCommonFuction::inside(double x, double y, SSurface* poly, bool applyOption)
{
	if (poly == NULL)
		return -1;
	//x = 323;
	//y = 200;
	int i;
	int count = 0;
	int ic;
	count = 0;

	int lastExteriorIdx = 0;
	if (poly->m_numParts > 1)
	{
		lastExteriorIdx = poly->m_pParts[1] - 1;
	}
	else
	{
		lastExteriorIdx = poly->m_numPoints - 1;
	}

	int obj2PartIndex = 0;

	int entryIc = -1;


	//
	int* icList = NULL;
	static int ticList[100000];
	if (poly->m_numPoints > 100000)
	{
		int* ticList = new int[poly->m_numPoints];
		icList = ticList;
	}
	else
	{
		icList = ticList;
	}


	int listIndex = 0;
	//
	for (i = 0; i < poly->m_numPoints - 1; i++)
	{
		if (poly->m_numParts > obj2PartIndex + 1)
		{
			if (poly->m_pParts[obj2PartIndex + 1] == i + 1)
			{
				obj2PartIndex++;
				continue;
			}
		}

		double max_px = max(max(poly->m_pPoints[i].x, poly->m_pPoints[i + 1].x) + 1, x + 1);

		ic = intersect_ccw(x, y, max_px, y, poly->m_pPoints[i].x, poly->m_pPoints[i].y, poly->m_pPoints[i + 1].x, poly->m_pPoints[i + 1].y);
		icList[listIndex++] = ic;

		if (ic == -1)
		{
			//entryIc = ic;
		}
		if (ic == 0)
		{
			if (x > min(poly->m_pPoints[i].x, poly->m_pPoints[i + 1].x))
			{
				count = 0;
				break;
			}
		}
		else if (ic == 1)
		{
			count++;
		}
		else if (ic >= 20 || ic == 12)
		{
			count = 0;
			break;
		}
		else if (ic == 10) // 반 시계방향으로 진행(진입/나감)
		{
			if (entryIc == 11)
			{
				count++;
				entryIc = -1;
			}
			else if (lastExteriorIdx < i)
			{
				entryIc = ic;
			}
			else
				entryIc = ic;
		}
		else if (ic == 11) // 시계방향으로 진행(진입/나감)
		{
			if (entryIc == 10)
			{
				count++;
				entryIc = -1;
			}
			else if (lastExteriorIdx < i)
			{
				entryIc = ic;
			}
			else
				entryIc = ic;
		}
	}
	if (count & 1)
		return 1;
	else
		return -1;
}

double SCommonFuction::GetAngle(POINT _p1, POINT _p2)
{
	POINT p;
	p.x = _p2.x - _p1.x;
	p.y = _p2.y - _p1.y;

	double d = sqrt(((double)(p.x) * p.x) + (p.y * p.y));
	double temp = p.x / d;

	if (p.y < 0)
	{
		return (acos(-1.0) * 2) - acos(temp);
	}

	return acos(temp);
}

double SCommonFuction::GetAngle(double p1x, double p1y, double p2x, double p2y)
{

	double x = p2x - p1x;
	double y = p2y - p1y;

	double d = sqrt(((double)(x)*x) + (y * y));
	double temp = x / d;

	if (y < 0)
	{
		return (acos(-1.0) * 2) - acos(temp);
	}

	return acos(temp);
}

int SCommonFuction::overlap(SCompositeCurve* objPoly, SSurface* comPoly, bool applyOption)
{
	for (auto i = objPoly->m_listCurveLink.begin(); i != objPoly->m_listCurveLink.end(); i++)
	{
		SCurve* c = i->GetCurve();

		int result = overlap(c, comPoly, applyOption);
		if (result)
		{
			return result;
		}
	}
	return -1;
}

int SCommonFuction::overlap(SCurve* objPoly, SSurface* comPoly, bool applyOption)
{

	int ret = -1;

	if (!MBR::CheckOverlap(objPoly->m_mbr, comPoly->m_mbr))
	{
		return ret;
	}

	int i = 0, j = 0;
	for (i = 0; i < objPoly->m_numPoints - 1 && ret != 1; i++)
	{
		for (j = 0; j < comPoly->m_numPoints - 1 && ret != 1; j++)
		{
			ret = intersect_ccw(objPoly->m_pPoints[i].x, objPoly->m_pPoints[i].y, objPoly->m_pPoints[i + 1].x, objPoly->m_pPoints[i + 1].y,
				comPoly->m_pPoints[j].x, comPoly->m_pPoints[j].y, comPoly->m_pPoints[j + 1].x, comPoly->m_pPoints[j + 1].y);
		}
	}

	if (ret <= 0)
	{
		for (j = 0; j < comPoly->m_numPoints - 1 && ret <= 0; j++)
		{
			ret = inside(objPoly->m_pPoints[j].x, objPoly->m_pPoints[j].y, comPoly);
		}
	}

	return ret;
}

int SCommonFuction::overlap(SSurface* objPoly, SSurface* comPoly, bool applyOption)
{
	int ret = -1;

	if (!MBR::CheckOverlap(objPoly->m_mbr, comPoly->m_mbr))
	{
		return ret;
	}

	int i = 0, j = 0;
	for (i = 0; i < objPoly->m_numPoints - 1 && ret != 1; i++)
	{
		for (j = 0; j < comPoly->m_numPoints - 1 && ret != 1; j++)
		{
			ret = intersect_ccw(objPoly->m_pPoints[i].x, objPoly->m_pPoints[i].y, objPoly->m_pPoints[i + 1].x, objPoly->m_pPoints[i + 1].y,
				comPoly->m_pPoints[j].x, comPoly->m_pPoints[j].y, comPoly->m_pPoints[j + 1].x, comPoly->m_pPoints[j + 1].y);
		}
	}

	if (ret <= 0)
	{
		for (j = 0; j < comPoly->m_numPoints - 1 && ret <= 0; j++)
		{
			ret = inside(objPoly->m_pPoints[j].x, objPoly->m_pPoints[j].y, comPoly);
		}
	}

	return ret;
}

void swap_double_value(double* var1, double* var2)
{
	double temp = *var1;
	*var1 = *var2;
	*var2 = temp;
}
int SCommonFuction::intersect_ccw(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y)
{
	int r123, r124, r341, r342;

	if (p1x > p2x)
	{
		swap_double_value(&p1x, &p2x);
		swap_double_value(&p1y, &p2y);
	}
	if (p3x > p4x)
	{
		swap_double_value(&p3x, &p4x);
		swap_double_value(&p3y, &p4y);
	}

	r123 = ccw(p1x, p1y, p2x, p2y, p3x, p3y);
	r124 = ccw(p1x, p1y, p2x, p2y, p4x, p4y);
	r341 = ccw(p3x, p3y, p4x, p4y, p1x, p1y);
	r342 = ccw(p3x, p3y, p4x, p4y, p2x, p2y);

	if (p1x == p2x && p1y == p2y)
	{
		if (341 * 342 == 0)
			return 1;
		else
			return -1;
	}
	else if (p3x == p4x && p3y == p4y)
	{
		if (123 * 124 == 0)
			return 1;
		else
			return -1;
	}

	if ((r123 * r124 < 0) && (r341 * r342 < 0))
		return 1; // 교차

	if (r123 == 0 && r124 == 0) // 방향이 같은 경우
	{
		if (p3x > p2x || p1x > p4x)
			return -1;	// 만나지 않는 경우
		//else 
		//	return 0;	// 겹치는 경우
	}

	if (r341 == 0) // 한점이 선분진행 방향으로의 연장선 혹은 선분 내부에 있는 경우
	{
		if ((p3x == p1x && p3y == p1y)
			|| (p4x == p1x && p4y == p1y))
		{
			return 22;
		}
		else if (p3x <= p1x && p1x <= p4x && min(p3y, p4y) <= p1y && max(p3y, p4y) >= p1y)
		{
			if (r342 == 1) // 시계방향
			{
				return 20;
			}
			else if (r342 == -1) // 반시계 방향
			{
				return 21;
			}
			else  // 겹침
				return 0;
		}
		else
		{
			if (r342 == 1) // 시계방향
			{
				return -20;
			}
			else if (r342 == -1) // 반시계 방향
			{
				return -21;
			}
			else// 겹침
				return 0;
		}
	}

	if (r342 == 0) // 한점이 선분진행 방향으로의 연장선 혹은 선분 내부에 있는 경우
	{
		if ((p3x == p2x && p3y == p2y)
			|| (p4x == p2x && p4y == p2y))
		{
			return 22;
		}
		else if (p3x <= p2x && p2x <= p4x && min(p3y, p4y) <= p2y && max(p3y, p4y) >= p2y)
		{
			if (r341 == 1) // 시계방향
			{
				return 20;
			}
			else if (r341 == -1) // 반시계 방향
			{
				return 21;
			}
			else// 겹침
				return 0;
		}
		else  // 연장선 상에 끝점이 있는 경우
		{
			if (r341 == 1) // 시계방향
			{
				return -20;
			}
			else if (r341 == -1) // 반시계 방향
			{
				return -21;
			}
			else// 겹침
				return 0;
		}
	}
	if (r123 == 0) // 한점이 선분시작 방향으로의 연장선 혹은 선분 내부에 있는 경우
	{
		if ((p1x == p3x && p1y == p3y)
			|| (p2x == p3x && p2y == p3y))
		{
			return 12;
		}
		else if (p1x <= p3x && p3x <= p2x && min(p1y, p2y) <= p3y && max(p1y, p2y) >= p3y)
		{
			if (r124 == 1) // 시계방향
			{
				return 10;
			}
			else if (r124 == -1) // 반시계 방향
			{
				return 11;
			}
			else// 겹침
				return 0;
		}
		else
		{
			if (r124 == 1) // 시계방향
			{
				return -10;
			}
			else if (r124 == -1) // 반시계 방향
			{
				return -11;
			}
			else// 겹침
				return 0;
		}
	}
	if (r124 == 0) // 한점이 선분진행 방향으로의 연장선 혹은 선분 내부에 있는 경우
	{
		if ((p1x == p4x && p1y == p4y)
			|| (p2x == p4x && p2y == p4y))
		{
			return 12;
		}
		else if (p1x <= p4x && p4x <= p2x && min(p1y, p2y) <= p4y && max(p1y, p2y) >= p4y)
		{
			if (r123 == 1) // 시계방향
			{
				return 10;
			}
			else if (r123 == -1) // 반시계 방향 
			{
				return 11;
			}
			else// 겹침
				return 0;
		}
		else // 연장선
		{
			if (r123 == 1) // 시계방향
			{
				return -10;
			}
			else if (r123 == -1) // 반시계 방향 
			{
				return -11;
			}
			else// 겹침
				return 0;
		}
	}

	return -1; // 교차하지 않는 경우
}


// -1 : 반시계 방향
//  0 : 일직선 상 
//  1 : 시계 방향
int SCommonFuction::ccw(double ax, double ay, double bx, double by, double cx, double cy)
{
	double l;
	l = bx * cy - ay * bx - ax * cy
		- by * cx + ax * by + ay * cx;

	//l = bx*cy;
	//l -= ay*bx;
	//l -= ax*cy;
	//l -= by*cx;
	//l += ax*by;
	//l += ay*cx;

	if (l > 0)
		return 1; // cw 방향
	else if (l < 0)
		return -1; // ccw 방향
	else
		return 0; // 일직선상에 있는 경우
}