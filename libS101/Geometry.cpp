#include "pch.h"
#include "Geometry.h"

//#include "..\\PLLibrary\\Symbol.h"
//#include "..\\PLLibrary\\Vector.h"

Geometry::Geometry() 
{
}

Geometry::~Geometry()
{
}

MBR& Geometry::GetMBRRef()
{
	return m_mbr;
}

//POINT Geometry::CalculateCenterOfGravityOfPolygon(POINT *_p, int _count)
//{
//	int j = 0;
//	double area = 0;
//	double centerX = 0.0;
//	double centerY = 0.0;
//
//	double x1, y1, x2, y2, tmpArea;
//
//	for (int i = 0; i < _count; i++)
//	{
//		j = (i + 1) % _count;
//
//		x1 = _p[i].x;
//		y1 = _p[i].y;
//		x2 = _p[j].x;
//		y2 = _p[j].y;
//
//		tmpArea = ((x1 * y2) - (x2 * y1));
//
//		centerX += ((x1 + x2) * tmpArea);
//		centerY += ((y1 + y2) * tmpArea);
//		area += tmpArea;
//	}
//
//	area *= 0.5;
//
//	centerX = centerX / (6.0 * area);
//	centerY = centerY / (6.0 * area);
//
//	POINT returnValue;
//	returnValue.x = (LONG)centerX;
//	returnValue.y = (LONG)centerY;
//
//	return returnValue;
//}
//
//double Geometry::GetDistance(POINT _p1, POINT _p2)
//{
//	POINT p;
//
//	p.x = _p2.x - _p1.x;
//	p.y = _p2.y - _p1.y;
//
//	return sqrt((double)((p.x * p.x) + (p.y * p.y)));
//}
//
//double Geometry::GetAngle(POINT _p1, POINT _p2)
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
//
//POINT Geometry::GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset)
//{
//	double angle = GetAngle(_p1, _p2);  // 라인이 x축의 양의 방향과 이루는 각도
//
//	POINT returnValue;
//	returnValue.x = (LONG)(_p1.x + offset * cos(angle));
//	returnValue.y = (LONG)(_p1.y + offset * sin(angle));
//
//	return returnValue;
//}
//
//void Geometry::DrawCompositeLineOnLine(CDC *pDC, POINT _p1, POINT _p2, Symbol* _symbol)
//{
//	int symbolWidth = _symbol->CoverXmax - _symbol->CoverXmin;  // 패턴 신볼 Cover의 폭   
//
//	POINT tmpP1, tmpP2;
//	tmpP1.x = _p1.x;
//	tmpP1.y = _p1.y;
//	tmpP2.x = _p2.x;
//	tmpP2.y = _p2.y;
//	int symbolCount = (int)(GetDistance(tmpP1, tmpP2) / symbolWidth);
//
//	POINT p;
//	for (int i = 0; i < symbolCount; i++)
//	{
//		p = GetOffsetPointOnLine(tmpP1, tmpP2, symbolWidth * i);
//
//		double angle = GetAngle(tmpP1, tmpP2);
//		double RAD2DEG = 180.0 / acos(-1.0);
//
//		angle *= RAD2DEG;
//		_symbol->Draw(pDC, CPoint(LONG(p.x), LONG(p.y)), angle);
//	}
//
//	// Composite line을 그리고 빈 공간을 Composite line색을 사용하여 라인으로 채운다.
//	p = GetOffsetPointOnLine(tmpP1, tmpP2, symbolWidth * symbolCount); // Composite line을 모두 그린 뒤의 위치
//
//	// p가 Line위에 있는 경우(x범위만 검사한다)
//	if ((p.x >= _p1.x) && (p.x <= _p2.x) || (p.x <= _p1.x) && (p.x >= _p2.x))
//	{
//		// 라인은 Symbol의 첫번째 색과 
//		// Symbol이 가지고 있는 첫번째 Vector의 두께로 그린다.
//		CPen newPen(PS_SOLID, _symbol->vector[0]->width, _symbol->color[0].color);
//		CPen *oldPen = pDC->SelectObject(&newPen);
//		pDC->MoveTo(p.x, p.y);
//		pDC->LineTo(_p2);
//		pDC->SelectObject(oldPen);
//		newPen.DeleteObject();
//	}
//}
//
//void Geometry::DrawCompositeLineOnPolyline(CDC *pDC, POINT* _p, int _count, Symbol* _symbol)
//{
//	for (int i = 1; i < _count; i++)
//	{
//		DrawCompositeLineOnLine(pDC, _p[i - 1], _p[i], _symbol);
//	}
//}
//
//double Geometry::GetDistanceOfPolyline(POINT *_p, int _count)
//{
//	double returnValue = 0;
//
//	for (int i = 1; i < _count; i++)
//	{
//		returnValue += GetDistance(_p[i - 1], _p[i]);
//	}
//
//	return returnValue;
//}

inline void swap(int &val1, int &val2)
{
	int t = val1;
	val1 = val2;
	val2 = t;
}

//bool Geometry::IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2)
//{
//	float x12 = (float)(_s1.x - _e1.x);
//	float x34 = (float)(_s2.x - _e2.x);
//	float y12 = (float)(_s1.y - _e1.y);
//	float y34 = (float)(_s2.y - _e2.y);
//
//	float c = x12 * y34 - y12 * x34;
//
//	if (fabs(c) < 0.01)
//	{
//		// No intersection
//		return false;
//	}
//	else
//		return true;
//
//	//if(_s1.x == _e1.x && _s1.y == _e1.y)
//	//{
//	//	return false;
//	//}
//	//else if(_s2.x == _e2.x && _s2.y == _e2.y)
//	//{
//	//	return false;
//	//}
//
//	//if(_s1.x > _e1.x) 	{	swap(_s1.x, _e1.x); 	}
//	//if(_s1.y > _e1.y) 	{	swap(_s1.y, _e1.y); 	}
//	//if(_s2.x > _e2.x) 	{	swap(_s2.x, _e2.x); 	}
//	//if(_s2.y > _e2.y) 	{	swap(_s2.y, _e2.y); 	}
//
//	//
//	//if(	_s1.y >= _s2.y && _s1.y <= _e2.y	||
//	//	_e1.y >= _s2.y && _e1.y <= _e2.y	
//	//	)
//	//{
//	//	if( _s1.x >= _s2.x && _s1.x <= _e2.x	||
//	//		_e1.x >= _s2.x && _e1.x <= _e2.x	
//	//	)
//	//	{
//	//		return true;
//	//	}
//	//}
//	//return false;
//}
//
//void Geometry::CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect *viewPort)
//{
//	// Store the values for fast access and easy
//
//	// equations-to-code conversion
//	float x1 = (float)_s1.x, x2 = (float)_e1.x, x3 = (float)_s2.x, x4 = (float)_e2.x;
//	float y1 = (float)_s1.y, y2 = (float)_e1.y, y3 = (float)_s2.y, y4 = (float)_e2.y;
//	float d = (float)(x1 - x2) * (float)(y3 - y4) - (float)(y1 - y2) * (float)(x3 - x4);
//	// If d is zero, there is no intersection
//	if (d == 0) return;
//
//	// Get the x and y
//	float pre = (float)(x1*y2 - y1 * x2), post = (float)(x3*y4 - y3 * x4);
//	float x = (float)(pre * (x3 - x4) - (x1 - x2) * post) / d;
//	float y = (float)(pre * (y3 - y4) - (y1 - y2) * post) / d;
//
//	if (_s2.x == _s1.x ||
//		_s2.x == _s1.x ||
//		_s2.y == _s1.y ||
//		_s2.y == _s1.y ||
//		_e2.x == _e1.x ||
//		_e2.x == _e1.x ||
//		_e2.y == _e1.y ||
//		_e2.y == _e1.y
//		)
//	{
//		return;
//	}
//	else if (
//		viewPort->left > _s1.x ||
//		viewPort->right < _s1.x ||
//		viewPort->bottom < _s1.y ||
//		viewPort->top	> _s1.y)
//	{
//		_s1.x = (LONG)x;
//		_s1.y = (LONG)y;
//	}
//	else
//	{
//		_e1.x = (LONG)x;
//		_e1.y = (LONG)y;
//	}
//}
//
//void Geometry::GetViewLineInPolyline(POINT *_p, int _count, CRect *viewPort)
//{
//	POINT p1, p2;
//	p1.x = viewPort->left;
//	p1.y = viewPort->top;
//	p2.x = viewPort->right;
//	p2.y = viewPort->top;
//	//	if(IsIntersect(_p[0], _p[1], p1, p2))
//	if (p1.y > _p[0].y)
//	{
//		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
//	}
//	//	if(IsIntersect(_p[_count], _p[_count-1], p1, p2))
//	else if (p1.y > _p[_count - 1].y)
//	{
//		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
//	}
//
//	p1.x = viewPort->right;
//	p1.y = viewPort->top;
//	p2.x = viewPort->right;
//	p2.y = viewPort->bottom;
//	//	if(IsIntersect(_p[0], _p[1], p1, p2))
//	if (p1.x < _p[0].x)
//	{
//		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
//	}
//	//	if(IsIntersect(_p[_count], _p[_count-1], p1, p2))
//	else if (p1.x < _p[_count - 1].x)
//	{
//		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
//	}
//	p1.x = viewPort->left;
//	p1.y = viewPort->bottom;
//	p2.x = viewPort->right;
//	p2.y = viewPort->bottom;
//	//	if(IsIntersect(_p[0], _p[1], p1, p2))
//	if (p1.y < _p[0].y)
//	{
//		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
//	}
//	//	if(IsIntersect(_p[_count], _p[_count-1], p1, p2))
//	else if (p1.y < _p[_count - 1].y)
//	{
//		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
//	}
//	p1.x = viewPort->left;
//	p1.y = viewPort->top;
//	p2.x = viewPort->left;
//	p2.y = viewPort->bottom;
//	//	if(IsIntersect(_p[0], _p[1], p1, p2))
//	if (p1.x > _p[0].x)
//	{
//		CutLineToIntersect(_p[0], _p[1], p1, p2, viewPort);
//	}
//	//	if(IsIntersect(_p[_count], _p[_count-1], p1, p2))
//	else if (p1.x > _p[_count - 1].x)
//	{
//		CutLineToIntersect(_p[_count - 1], _p[_count - 2], p1, p2, viewPort);
//	}
//
//	return;
//}
//
//POINT* Geometry::GetCenterPointOfPolyline(POINT *_p, int _count, CRect *viewPort)
//{
//	if (_count < 2)
//	{
//		return NULL;
//	}
//	else
//	{
//		GetViewLineInPolyline(_p, _count, viewPort);
//	}
//
//	double centerDistance = GetDistanceOfPolyline(_p, _count) / 2; // 폴리라인의 시작점부터 폴리라인의 중간지점 까지의 거리
//
//	if (centerDistance == 0)
//	{
//		return NULL;
//	}
//
//	double accumulatedDistance = 0;								   // 누적거리
//	POINT *returnValue = new POINT;
//
//	for (int i = 0; i < (_count - 1); i++)						   // 0번째 점부터 다음 점으로 이동하면서 누적거리를 측정한다
//	{
//		accumulatedDistance += GetDistance(_p[i], _p[i + 1]);      // 다은점으로 이동하면서 누적거리를 측정
//
//		if (accumulatedDistance >= centerDistance)				   // 누적거리가 중간거리보다 크다면 중간점을 지난것이다.
//		{
//			*returnValue = GetOffsetPointOnLine(_p[i + 1], _p[i], accumulatedDistance - centerDistance); // 지나온 거리만큼 돌아간다.
//			break;
//		}
//	}
//
//	return returnValue;
//}