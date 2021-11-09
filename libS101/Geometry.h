#pragma once
#include "MBR.h"

class Scaler;

class Symbol;

class Geometry
{
public:
	Geometry();
	virtual ~Geometry();

public:
	MBR m_mbr;

public:
	MBR& GetMBRRef();

public:
	//virtual void DrawGeometry(HDC &hDC, Scaler *scaler, double offset = 0) {};
	//virtual void DrawTextInfo(HDC &hDC, Scaler *scaler, double offset = 0) {};

	//virtual void DrawGeometry(CDCRenderTarget* pRenderTarget, Scaler *scaler, double offsetX = 0, double offsetY = 0) {};
	//virtual void DrawTextInfo(CDCRenderTarget* pRenderTarget, Scaler *scaler, double offsetX = 0, double offsetY = 0) {};

	//virtual void DrawGeometry(ID2D1GeometrySink* pSink, Scaler *scaler, double offsetX = 0, double offsetY = 0) {};
//
//public:
//	/*
//** 기능   : 주어진 폴리곤의 무게중심점을 찾아준다.
//** 인자   :
//			POINT* _p     - 폴리곤의 좌표를 가지고있는 배열
//			int    _count - 폴리곤 좌표의 개수
//** 반환값 : 무게중심점
//*/
//	POINT CalculateCenterOfGravityOfPolygon(POINT *_p, int _count);
//
//
//	/*
//	** 기능   : 두 점 사이의 거리를 구한다.
//	** 인자   :
//				POINT _p1 - 첫번째 점
//				POINT _p2 - 두번째 점
//	** 반환값 : 두 점 사이의 거리
//	*/
//	double GetDistance(POINT _p1, POINT _p2);
//
//
//	/*
//	** 기능   : 폴리라인의 길이를 구한다.
//	** 인자   :
//				POINT* _p     - 폴리라인의 좌표 배열
//				int    _count - 폴리라인 좌표  개수
//	** 반환값 : 폴리라인의 길이
//	*/
//	double GetDistanceOfPolyline(POINT *_p, int _count);
//
//	/*
//	** 기능   : 폴리라인 중 화면에 보이는 선을 구한다.
//	** 인자   :
//				POINT* _p     - 폴리라인의 좌표 배열
//				int    _count - 폴리라인 좌표  개수
//	** 반환값 : 폴리라인 중 화면에 보이는 선
//	*/
//	void GetViewLineInPolyline(POINT *_p, int _count, CRect *viewPort);
//
//	/*
//	** 기능   : 폴리라인의 거리상 중간지점을 구한다.
//	** 인자   :
//				POINT* _p     - 폴리라인의 좌표 배열
//				int    _count - 폴리라인 좌표  개수
//	** 반환값 : 폴리라인의 거리상 중간지점
//	*/
//	POINT* GetCenterPointOfPolyline(POINT *_p, int _count, CRect *viewPort);
//
//
//
//	/*
//	** 기능   : 두 선분이 교차하는지 확인한다.
//	** 인자   :
//				POINT _s1     - 라인1의 시작점
//				POINT _e1     - 라인1의 끝점
//				POINT _s2     - 라인2의 시작점
//				POINT _e2     - 라인2의 끝점
//	** 반환값 : 두 선분의 교차 여부
//	*/
//	bool IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2);
//
//	/*
//	** 기능   :
//	** 인자   : 교차점에서 선의 시작점을 이동
//				POINT _s1     - 라인1의 시작점
//				POINT _e1     - 라인1의 끝점
//				POINT _s2     - 라인2의 시작점
//				POINT _e2     - 라인2의 끝점
//	** 반환값 :
//	*/
//	void CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect *viewPort);
//
//
//	/*
//	** 기능   : 2차원 벡터와 x축 사이의 양의 방향으로의 각도를 구한다.
//	** 인자   :
//				POINT _p1 - 벡터의 시작점
//				POINT _p2 - 벡터의 끝점
//	** 반환값 : 결과값(단위 : radian, 범위 : 0 ~ 2 * pi)
//	*/
//	double GetAngle(POINT _p1, POINT _p2);
//
//
//	/*
//	** 기능   : 라인의 시작점에서 offset만큼(거리) 떨어진 점을 구한다.
//	** 인자   :
//				POINT _p1     - 라인의 시작점
//				POINT _p2     - 라인의 끝점
//				double offset - offset
//	** 반환값 : 라인의 시작점에서 offset만큼 떨어진 점
//	*/
//	POINT GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset);
//
//
//	/*
//	** 기능 : CompositeLine을 Line위에 그린다.
//	** 인자 :
//			  CDC*   pDC     - 그리기 위한 dc
//			  POINT _p1      - 라인의 시작점
//			  POINT _p2      - 라인의 끝점
//			  Symbol _symbol - CompositeLine
//	*/
//	void DrawCompositeLineOnLine(CDC *pDC, POINT _p1, POINT _p2, Symbol* _symbol);
//
//
//	/*
//	** 기능 : CompositeLine을 Polyline위에 그린다.
//	** 인자 :
//			  CDC*   pDC     - 그리기 위한 dc
//			  POINT  _p1     - 라인의 시작점
//			  POINT  _p2     - 라인의 끝점
//			  Symbol _symbol - CompositeLine
//	*/
//	void DrawCompositeLineOnPolyline(CDC *pDC, POINT *_p, int _count, Symbol* _symbol);
};