#pragma once

#include "SPoint.h"
#include "SCompositeCurve.h"
#include "SSurface.h"

#include "..\\extlibs\\Clipper\\include\\clipper.hpp"

class SCommonFuction
{
public:
	/*
	** 기능   : 주어진 Surface를 화면 영역에 맞게 클리핑하고, 그 결과의 무게중심점을 구해준다.
	** 인자   :
	SSurface *_surface  - 무게중심점을 찾을 AREA
	CRect*   _viewPort - 화면 영역
	** 반환값 :
	NULL  - 화면과 Surface가 전혀 오버랩되지 않는 경우(무게 중심점이 존재할 수 없다)
	!NULL - 무게중심점들이 들어있는 Surface (무조건 파트는 1개이고, 모든 포인트는 무게중심점이다/)
	*/
	//static SSurface* CalculateCenterOfGravityOfSurface(SSurface *_surface, CRect *_viewPort);
	static void CalculateCenterOfGravityOfSurface(std::vector<POINT> &vp, SSurface *_surface, CRect *_viewPort, Scaler *pScaler);

	/*
	** 기능   : 주어진 AREA를 화면 영역에 맞게 클리핑한다.
	** 인자   :
	ENCArea* _polygon  - 클리핑할 AREA
	CRect*   _viewPort - 화면영역
	** 반환값 :
	NULL  - 화면영역과 AREA가 전혀 오버랩되지 않는 경우
	!NULL - 클리핑한 결과, 멀티 파트일 수 있다.
	*/
	static ClipperLib::Paths ClipSurface(SSurface *_polygon, CRect *_viewPort);


	/*
	** 기능   : 주어진 폴리곤의 무게중심점을 찾아준다.
	** 인자   :
	POINT* _p     - 폴리곤의 좌표를 가지고있는 배열
	int    _count - 폴리곤 좌표의 개수
	** 반환값 : 무게중심점
	*/
	static POINT CalculateCenterOfGravityOfSurface(POINT *_p, int _count);


	/*
	** 기능   : 주어진 폴리곤의 무게중심점을 찾아준다.
	** 인자   :
	POINT* _p     - 폴리곤의 좌표를 가지고있는 배열
	int    _count - 폴리곤 좌표의 개수
	** 반환값 : 무게중심점
	*/
	static POINT CalculateCenterOfGravityOfSurface(ClipperLib::Path polygon);


	/*
	** 기능   : 두 점 사이의 거리를 구한다.
	** 인자   :
	POINT _p1 - 첫번째 점
	POINT _p2 - 두번째 점
	** 반환값 : 두 점 사이의 거리
	*/
	static double GetDistance(POINT _p1, POINT _p2);


	/*
	** 기능   : 폴리라인의 길이를 구한다.
	** 인자   :
	POINT* _p     - 폴리라인의 좌표 배열
	int    _count - 폴리라인 좌표  개수
	** 반환값 : 폴리라인의 길이
	*/
	static double GetDistanceOfCurve(POINT *_p, int _count);

	/*
	** 기능   : 폴리라인 중 화면에 보이는 선을 구한다.
	** 인자   :
	POINT* _p     - 폴리라인의 좌표 배열
	int    _count - 폴리라인 좌표  개수
	** 반환값 : 폴리라인 중 화면에 보이는 선
	*/
	static void GetViewLineInCurve(POINT *_p, int _count, CRect* viewPort);

	/*
	** 기능   : 폴리라인의 거리상 중간지점을 구한다.
	** 인자   :
	POINT* _p     - 폴리라인의 좌표 배열
	int    _count - 폴리라인 좌표  개수
	** 반환값 : 폴리라인의 거리상 중간지점
	*/
	static POINT* GetCenterPointOfCurve(POINT *_p, int _count, CRect* viewPort);

	/*
	** 기능   : 두 선분이 교차하는지 확인한다.
	** 인자   :
	POINT _s1     - 라인1의 시작점
	POINT _e1     - 라인1의 끝점
	POINT _s2     - 라인2의 시작점
	POINT _e2     - 라인2의 끝점
	** 반환값 : 두 선분의 교차 여부
	*/
	static bool IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2);

	/*
	** 기능   :
	** 인자   : 교차점에서 선의 시작점을 이동
	POINT _s1     - 라인1의 시작점
	POINT _e1     - 라인1의 끝점
	POINT _s2     - 라인2의 시작점
	POINT _e2     - 라인2의 끝점
	** 반환값 :
	*/
	static void CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect* viewPort);


	/*
	** 기능   : 2차원 벡터와 x축 사이의 양의 방향으로의 각도를 구한다.
	** 인자   :
	POINT _p1 - 벡터의 시작점
	POINT _p2 - 벡터의 끝점
	** 반환값 : 결과값(단위 : radian, 범위 : 0 ~ 2 * pi)
	*/
	static double GetAngle(POINT _p1, POINT _p2);


	/*
	** 기능   : 라인의 시작점에서 offset만큼(거리) 떨어진 점을 구한다.
	** 인자   :
	POINT _p1     - 라인의 시작점
	POINT _p2     - 라인의 끝점
	double offset - offset
	** 반환값 : 라인의 시작점에서 offset만큼 떨어진 점
	*/
	static POINT GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset);


	///*
	//** 기능 : CompositeLine을 Line위에 그린다.
	//** 인자 :
	//CDC*   pDC     - 그리기 위한 dc
	//POINT _p1      - 라인의 시작점
	//POINT _p2      - 라인의 끝점
	//Symbol _symbol - CompositeLine
	//*/
	//static void DrawCompositeLineOnLine(CDC *pDC, POINT _p1, POINT _p2, Symbol _symbol);


	///*
	//** 기능 : CompositeLine을 Curve위에 그린다.
	//** 인자 :
	//CDC*   pDC     - 그리기 위한 dc
	//POINT  _p1     - 라인의 시작점
	//POINT  _p2     - 라인의 끝점
	//Symbol _symbol - CompositeLine
	//*/
	//static void DrawCompositeLineOnCurve(CDC *pDC, POINT *_p, int _count, Symbol _symbol);

	// line & line
	static bool Intersect(
		float x1, float y1, float x2, float y2,
		float x3, float y3, float x4, float y4,
		float* intersectionX, float* intersectionY);

	// line & rect
	static bool Intersect(
		float xmin, float ymin, float xmax, float ymax,
		float x1, float y1, float x2, float y2,
		float& intersectionX1, float& intersectionY1,
		float& intersectionX2, float& intersectionY2);

	//static const int NewGeometryScale = 5000;
	static const int NewGeometryScale = 70000000;



	static int intersect_ccw(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y);

	// -1 : 반시계 방향
	//  0 : 일직선 상 
	//  1 : 시계 방향
	static int ccw(double ax, double ay, double bx, double by, double cx, double cy);

	static int overlap(SCurve* objPoly, SSurface* comPoly, bool applyOption = false);
	static int overlap(SCompositeCurve* objPoly, SSurface* comPoly, bool applyOption = false);
	static int overlap(SSurface* objPoly, SSurface* comPoly, bool applyOption = false);
	// for S101
	static int inside(SPoint* point, SSurface* poly, bool applyOption = false);
	static int inside(double x, double y, SSurface* poly, bool applyOption = false);
	//static double GetAngle(POINT _p1, POINT _p2);
	static double GetAngle(double p1x, double p1y, double p2x, double p2y);
};