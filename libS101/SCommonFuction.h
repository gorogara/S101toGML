#pragma once

#include "SPoint.h"
#include "SCompositeCurve.h"
#include "SSurface.h"

#include "..\\extlibs\\Clipper\\include\\clipper.hpp"

class SCommonFuction
{
public:
	/*
	** ���   : �־��� Surface�� ȭ�� ������ �°� Ŭ�����ϰ�, �� ����� �����߽����� �����ش�.
	** ����   :
	SSurface *_surface  - �����߽����� ã�� AREA
	CRect*   _viewPort - ȭ�� ����
	** ��ȯ�� :
	NULL  - ȭ��� Surface�� ���� ���������� �ʴ� ���(���� �߽����� ������ �� ����)
	!NULL - �����߽������� ����ִ� Surface (������ ��Ʈ�� 1���̰�, ��� ����Ʈ�� �����߽����̴�/)
	*/
	//static SSurface* CalculateCenterOfGravityOfSurface(SSurface *_surface, CRect *_viewPort);
	static void CalculateCenterOfGravityOfSurface(std::vector<POINT> &vp, SSurface *_surface, CRect *_viewPort, Scaler *pScaler);

	/*
	** ���   : �־��� AREA�� ȭ�� ������ �°� Ŭ�����Ѵ�.
	** ����   :
	ENCArea* _polygon  - Ŭ������ AREA
	CRect*   _viewPort - ȭ�鿵��
	** ��ȯ�� :
	NULL  - ȭ�鿵���� AREA�� ���� ���������� �ʴ� ���
	!NULL - Ŭ������ ���, ��Ƽ ��Ʈ�� �� �ִ�.
	*/
	static ClipperLib::Paths ClipSurface(SSurface *_polygon, CRect *_viewPort);


	/*
	** ���   : �־��� �������� �����߽����� ã���ش�.
	** ����   :
	POINT* _p     - �������� ��ǥ�� �������ִ� �迭
	int    _count - ������ ��ǥ�� ����
	** ��ȯ�� : �����߽���
	*/
	static POINT CalculateCenterOfGravityOfSurface(POINT *_p, int _count);


	/*
	** ���   : �־��� �������� �����߽����� ã���ش�.
	** ����   :
	POINT* _p     - �������� ��ǥ�� �������ִ� �迭
	int    _count - ������ ��ǥ�� ����
	** ��ȯ�� : �����߽���
	*/
	static POINT CalculateCenterOfGravityOfSurface(ClipperLib::Path polygon);


	/*
	** ���   : �� �� ������ �Ÿ��� ���Ѵ�.
	** ����   :
	POINT _p1 - ù��° ��
	POINT _p2 - �ι�° ��
	** ��ȯ�� : �� �� ������ �Ÿ�
	*/
	static double GetDistance(POINT _p1, POINT _p2);


	/*
	** ���   : ���������� ���̸� ���Ѵ�.
	** ����   :
	POINT* _p     - ���������� ��ǥ �迭
	int    _count - �������� ��ǥ  ����
	** ��ȯ�� : ���������� ����
	*/
	static double GetDistanceOfCurve(POINT *_p, int _count);

	/*
	** ���   : �������� �� ȭ�鿡 ���̴� ���� ���Ѵ�.
	** ����   :
	POINT* _p     - ���������� ��ǥ �迭
	int    _count - �������� ��ǥ  ����
	** ��ȯ�� : �������� �� ȭ�鿡 ���̴� ��
	*/
	static void GetViewLineInCurve(POINT *_p, int _count, CRect* viewPort);

	/*
	** ���   : ���������� �Ÿ��� �߰������� ���Ѵ�.
	** ����   :
	POINT* _p     - ���������� ��ǥ �迭
	int    _count - �������� ��ǥ  ����
	** ��ȯ�� : ���������� �Ÿ��� �߰�����
	*/
	static POINT* GetCenterPointOfCurve(POINT *_p, int _count, CRect* viewPort);

	/*
	** ���   : �� ������ �����ϴ��� Ȯ���Ѵ�.
	** ����   :
	POINT _s1     - ����1�� ������
	POINT _e1     - ����1�� ����
	POINT _s2     - ����2�� ������
	POINT _e2     - ����2�� ����
	** ��ȯ�� : �� ������ ���� ����
	*/
	static bool IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2);

	/*
	** ���   :
	** ����   : ���������� ���� �������� �̵�
	POINT _s1     - ����1�� ������
	POINT _e1     - ����1�� ����
	POINT _s2     - ����2�� ������
	POINT _e2     - ����2�� ����
	** ��ȯ�� :
	*/
	static void CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect* viewPort);


	/*
	** ���   : 2���� ���Ϳ� x�� ������ ���� ���������� ������ ���Ѵ�.
	** ����   :
	POINT _p1 - ������ ������
	POINT _p2 - ������ ����
	** ��ȯ�� : �����(���� : radian, ���� : 0 ~ 2 * pi)
	*/
	static double GetAngle(POINT _p1, POINT _p2);


	/*
	** ���   : ������ ���������� offset��ŭ(�Ÿ�) ������ ���� ���Ѵ�.
	** ����   :
	POINT _p1     - ������ ������
	POINT _p2     - ������ ����
	double offset - offset
	** ��ȯ�� : ������ ���������� offset��ŭ ������ ��
	*/
	static POINT GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset);


	///*
	//** ��� : CompositeLine�� Line���� �׸���.
	//** ���� :
	//CDC*   pDC     - �׸��� ���� dc
	//POINT _p1      - ������ ������
	//POINT _p2      - ������ ����
	//Symbol _symbol - CompositeLine
	//*/
	//static void DrawCompositeLineOnLine(CDC *pDC, POINT _p1, POINT _p2, Symbol _symbol);


	///*
	//** ��� : CompositeLine�� Curve���� �׸���.
	//** ���� :
	//CDC*   pDC     - �׸��� ���� dc
	//POINT  _p1     - ������ ������
	//POINT  _p2     - ������ ����
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

	// -1 : �ݽð� ����
	//  0 : ������ �� 
	//  1 : �ð� ����
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