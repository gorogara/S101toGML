#pragma once

#include <vector>

#include "MBR.h"

class GeoPoint;

class Scaler
{
public:
	Scaler();
	virtual ~Scaler();

public:
	static const long long COMF = 10000000000;

	// ���� �߽�
	double mox = 0;
	double moy = 0;

	// ���� ����
	double mxMin = 0;
	double myMin = 0;
	double mxMax = 0;
	double myMax = 0;

	// ȸ���� ��� MBR�� ����ϱ� ���� ����
	double mxMinCalcMBR = 0;
	double myMinCalcMBR = 0;
	double mxMaxCalcMBR = 0;
	double myMaxCalcMBR = 0;

	// ȭ�� �߽�
	double sox = 0;
	double soy = 0;

	// ȭ�� ����
	double sxMin = 0;
	double syMin = 0;
	double sxMax = 0;
	double syMax = 0;


	double currentScale = 0;

	double mxMinLimit = -179.9;
	double myMinLimit = -84.9;
	double mxMaxLimit = 179.9;
	double myMaxLimit = 84.9;

private:
	double rotateDegree = 0;

public:
	/*
	** ��ǥ ��ȯ
	*/
	void DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate = TRUE);
	void WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate = TRUE);
	void WorldToDevice_F(double mx, double my, float *sx, float *sy, bool rotate = TRUE);

	/*
	******* �Ÿ� ��� *******
	*/
	/*
	** ��� : �� ǥ�� ���� �� �� ������ �Ÿ� ���
	** ���� :
			  double longitude1 - ù ��° ���� �浵(���� : ����)
			  double latitude1  - ù ��° ���� ����(���� : ����)
			  double longitude2 - �� ��° ���� �浵(���� : ����)
			  double latitude2  - �� ��° ���� ����(���� : ����)
	** ��� : �� �� ������ �Ÿ�(���� : km)
	*/
	//double GetDistanceLatitudeLongitude(double longitude1, double latitude1, double longitude2, double latitude2); // ��� ���� (LatLonUtility���)

	//double GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian); // ��� ���� (LatLonUtility���)

	/*
	** ��� : 2���� ��� ���� �� �� ������ �Ÿ� ���
	** ���� :
			  int x1 - ù ��° ���� x��ǥ(���� : ����)
			  int y1 - ù ��° ���� y��ǥ(���� : ����)
			  int x2 - �� ��° ���� x��ǥ(���� : ����)
			  int y2 - �� ��° ���� y��ǥ(���� : ����)
	** ��� : �� �� ������ �Ÿ�(���� : ����)
	*/
	double GetDistanceScreen(int x1, int y1, int x2, int y2);

	/*
	** ȭ�� ����
	*/
	// View�� ũ�Ⱑ ����� �� �� ȣ���ؾ���(OnSize)
	void SetScreen(RECT *rect);
	void SetScreen(const RECT& rect);
	// ȭ�� ���̸� �ȼ� ������ ���� ����(Max - Min + 1, +1�� �ϴ� ������ ȭ����ǥ�� zero-base �̱� ����)
	int  GetScreenWidth();
	int  GetScreenHeight();
	CRect GetScreenRect();
	D2D1_RECT_F GetD2Rect();

	/*
	** ȭ�鿡 �������� Map�� ����
	*/
	// ��ü ������ MBR�� ����� �� �� ȣ���ؾ���(���� ���� ����)
	void SetMap(double xmin, double ymin, double xmax, double ymax);
	void SetMap(MBR mbr);
	void SetMap(RECT r);

	double GetMapWidth();
	double GetMapHeight();

	void GetMap(MBR *mbr);
	MBR GetMap();

	MBR GetMapCalcMBR();

	/*
	** Scale ����
	*/
	int GetCurrentScale();
	void SetScale(int scale);
	void UpdateScale();
	
	/*
	** Rotation ����
	*/
	double GetRotationDegree();
	void SetRotationDegree(double _value);

	/*
	** Ȯ�� & ���
	** ��  �� : %����
				Ȯ��/��� ���� ��ǥ(ȭ�� ��ǥ)
	** ��뿹 : 20%Ȯ���ϰ� ������ ���ڿ� "20" ����
	*/
	void ZoomIn(double value);
	void ZoomIn(double value, int x, int y);
	void ZoomOut(double value);
	void ZoomOut(double value, int x, int y);

	/*
	** ȭ�� �̵�
	*/
	// ���ϴ� �ȼ���ŭ �����϶� ���(�̿ϼ�)
	void MoveMap(int x, int y);

	// ���ϴ� ��ġ(sy, sy)�� ���ϴ� ����(mx, my)�� ��ġ ��ų�� ���
	void MoveMap(int sx, int sy, double mx, double my);

	//void Projection(double *x, double *y);
	//void InverseProjection(double *x, double *y);

	/*
	** ȭ�� ȸ��
	*/
	void Rotate(LONG *sx, LONG *sy, double degree);
	void RotateMap(double degree);	// rotateDegree�� ������ [0, 360) ���� ����
	void NorthUp();

	// ��� ȭ���� ����
	void AdjustScreenMap();

	void AdjustScreenMap_Internal();

	// ������� ���� ũ��(���� : km)
	double GetScreenWidthKM();

	bool PtInMap(double _x, double _y);
	//ClipperLib::PolyTree* ClipPolyline(GeoPoint* _p, int _cnt);

	void PrivateMoveMap(int sx, int sy, double mx, double my);

	D2D1::Matrix3x2F GetMatrix();
	D2D1::Matrix3x2F GetInverseMatrix();

	CRect WGS84MBRtoCRect(MBR& mbr);

	double GetMapXMin();
	double GetMapYMin();
	double GetMapXMax();
	double GetMapYMax();

	long long GetCOMF();

	void(*XYtoSP) (int nX, int nY, int &nLat, int &nLon, bool bPluse, bool bDirect) = nullptr;
	void(*SPtoXY) (int nX, int nY, int &nLat, int &nLon, bool bPluse, bool bDirect) = nullptr;
};