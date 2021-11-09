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
//** ���   : �־��� �������� �����߽����� ã���ش�.
//** ����   :
//			POINT* _p     - �������� ��ǥ�� �������ִ� �迭
//			int    _count - ������ ��ǥ�� ����
//** ��ȯ�� : �����߽���
//*/
//	POINT CalculateCenterOfGravityOfPolygon(POINT *_p, int _count);
//
//
//	/*
//	** ���   : �� �� ������ �Ÿ��� ���Ѵ�.
//	** ����   :
//				POINT _p1 - ù��° ��
//				POINT _p2 - �ι�° ��
//	** ��ȯ�� : �� �� ������ �Ÿ�
//	*/
//	double GetDistance(POINT _p1, POINT _p2);
//
//
//	/*
//	** ���   : ���������� ���̸� ���Ѵ�.
//	** ����   :
//				POINT* _p     - ���������� ��ǥ �迭
//				int    _count - �������� ��ǥ  ����
//	** ��ȯ�� : ���������� ����
//	*/
//	double GetDistanceOfPolyline(POINT *_p, int _count);
//
//	/*
//	** ���   : �������� �� ȭ�鿡 ���̴� ���� ���Ѵ�.
//	** ����   :
//				POINT* _p     - ���������� ��ǥ �迭
//				int    _count - �������� ��ǥ  ����
//	** ��ȯ�� : �������� �� ȭ�鿡 ���̴� ��
//	*/
//	void GetViewLineInPolyline(POINT *_p, int _count, CRect *viewPort);
//
//	/*
//	** ���   : ���������� �Ÿ��� �߰������� ���Ѵ�.
//	** ����   :
//				POINT* _p     - ���������� ��ǥ �迭
//				int    _count - �������� ��ǥ  ����
//	** ��ȯ�� : ���������� �Ÿ��� �߰�����
//	*/
//	POINT* GetCenterPointOfPolyline(POINT *_p, int _count, CRect *viewPort);
//
//
//
//	/*
//	** ���   : �� ������ �����ϴ��� Ȯ���Ѵ�.
//	** ����   :
//				POINT _s1     - ����1�� ������
//				POINT _e1     - ����1�� ����
//				POINT _s2     - ����2�� ������
//				POINT _e2     - ����2�� ����
//	** ��ȯ�� : �� ������ ���� ����
//	*/
//	bool IsIntersect(POINT _s1, POINT _e1, POINT _s2, POINT _e2);
//
//	/*
//	** ���   :
//	** ����   : ���������� ���� �������� �̵�
//				POINT _s1     - ����1�� ������
//				POINT _e1     - ����1�� ����
//				POINT _s2     - ����2�� ������
//				POINT _e2     - ����2�� ����
//	** ��ȯ�� :
//	*/
//	void CutLineToIntersect(POINT &_s1, POINT &_e1, POINT _s2, POINT _e2, CRect *viewPort);
//
//
//	/*
//	** ���   : 2���� ���Ϳ� x�� ������ ���� ���������� ������ ���Ѵ�.
//	** ����   :
//				POINT _p1 - ������ ������
//				POINT _p2 - ������ ����
//	** ��ȯ�� : �����(���� : radian, ���� : 0 ~ 2 * pi)
//	*/
//	double GetAngle(POINT _p1, POINT _p2);
//
//
//	/*
//	** ���   : ������ ���������� offset��ŭ(�Ÿ�) ������ ���� ���Ѵ�.
//	** ����   :
//				POINT _p1     - ������ ������
//				POINT _p2     - ������ ����
//				double offset - offset
//	** ��ȯ�� : ������ ���������� offset��ŭ ������ ��
//	*/
//	POINT GetOffsetPointOnLine(POINT _p1, POINT _p2, double offset);
//
//
//	/*
//	** ��� : CompositeLine�� Line���� �׸���.
//	** ���� :
//			  CDC*   pDC     - �׸��� ���� dc
//			  POINT _p1      - ������ ������
//			  POINT _p2      - ������ ����
//			  Symbol _symbol - CompositeLine
//	*/
//	void DrawCompositeLineOnLine(CDC *pDC, POINT _p1, POINT _p2, Symbol* _symbol);
//
//
//	/*
//	** ��� : CompositeLine�� Polyline���� �׸���.
//	** ���� :
//			  CDC*   pDC     - �׸��� ���� dc
//			  POINT  _p1     - ������ ������
//			  POINT  _p2     - ������ ����
//			  Symbol _symbol - CompositeLine
//	*/
//	void DrawCompositeLineOnPolyline(CDC *pDC, POINT *_p, int _count, Symbol* _symbol);
};