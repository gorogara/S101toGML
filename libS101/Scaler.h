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

	// 지도 중심
	double mox = 0;
	double moy = 0;

	// 지도 범위
	double mxMin = 0;
	double myMin = 0;
	double mxMax = 0;
	double myMax = 0;

	// 회전한 경우 MBR을 계산하기 위한 정보
	double mxMinCalcMBR = 0;
	double myMinCalcMBR = 0;
	double mxMaxCalcMBR = 0;
	double myMaxCalcMBR = 0;

	// 화면 중심
	double sox = 0;
	double soy = 0;

	// 화면 범위
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
	** 좌표 변환
	*/
	void DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate = TRUE);
	void WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate = TRUE);
	void WorldToDevice_F(double mx, double my, float *sx, float *sy, bool rotate = TRUE);

	/*
	******* 거리 계산 *******
	*/
	/*
	** 기능 : 구 표면 위의 두 점 사이의 거리 계산
	** 인자 :
			  double longitude1 - 첫 번째 점의 경도(단위 : 라디안)
			  double latitude1  - 첫 번째 점의 위도(단위 : 라디안)
			  double longitude2 - 두 번째 점의 경도(단위 : 라디안)
			  double latitude2  - 두 번째 점의 위도(단위 : 라디안)
	** 결과 : 두 점 사이의 거리(단위 : km)
	*/
	//double GetDistanceLatitudeLongitude(double longitude1, double latitude1, double longitude2, double latitude2); // 사용 금지 (LatLonUtility사용)

	//double GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian); // 사용 금지 (LatLonUtility사용)

	/*
	** 기능 : 2차원 평면 위의 두 점 사이의 거리 계산
	** 인자 :
			  int x1 - 첫 번째 점의 x좌표(단위 : 정수)
			  int y1 - 첫 번째 점의 y좌표(단위 : 정수)
			  int x2 - 두 번째 점의 x좌표(단위 : 정수)
			  int y2 - 두 번째 점의 y좌표(단위 : 정수)
	** 결과 : 두 점 사이의 거리(단위 : 정수)
	*/
	double GetDistanceScreen(int x1, int y1, int x2, int y2);

	/*
	** 화면 설정
	*/
	// View의 크기가 변경될 때 꼭 호출해야함(OnSize)
	void SetScreen(RECT *rect);
	void SetScreen(const RECT& rect);
	// 화면 넓이를 픽셀 단위로 전달 받음(Max - Min + 1, +1을 하는 이유는 화면좌표는 zero-base 이기 때문)
	int  GetScreenWidth();
	int  GetScreenHeight();
	CRect GetScreenRect();
	D2D1_RECT_F GetD2Rect();

	/*
	** 화면에 보여지는 Map의 정보
	*/
	// 전체 지도의 MBR이 변경될 때 꼭 호출해야함(지도 파일 오픈)
	void SetMap(double xmin, double ymin, double xmax, double ymax);
	void SetMap(MBR mbr);
	void SetMap(RECT r);

	double GetMapWidth();
	double GetMapHeight();

	void GetMap(MBR *mbr);
	MBR GetMap();

	MBR GetMapCalcMBR();

	/*
	** Scale 정보
	*/
	int GetCurrentScale();
	void SetScale(int scale);
	void UpdateScale();
	
	/*
	** Rotation 정보
	*/
	double GetRotationDegree();
	void SetRotationDegree(double _value);

	/*
	** 확대 & 축소
	** 인  자 : %단위
				확대/축소 기준 좌표(화면 좌표)
	** 사용예 : 20%확대하고 싶을땐 인자에 "20" 삽입
	*/
	void ZoomIn(double value);
	void ZoomIn(double value, int x, int y);
	void ZoomOut(double value);
	void ZoomOut(double value, int x, int y);

	/*
	** 화면 이동
	*/
	// 원하는 픽셀만큼 움직일때 사용(미완성)
	void MoveMap(int x, int y);

	// 원하는 위치(sy, sy)에 원하는 점을(mx, my)를 위치 시킬때 사용
	void MoveMap(int sx, int sy, double mx, double my);

	//void Projection(double *x, double *y);
	//void InverseProjection(double *x, double *y);

	/*
	** 화면 회전
	*/
	void Rotate(LONG *sx, LONG *sy, double degree);
	void RotateMap(double degree);	// rotateDegree의 범위를 [0, 360) 으로 유지
	void NorthUp();

	// 벗어난 화면을 조정
	void AdjustScreenMap();

	void AdjustScreenMap_Internal();

	// 모니터의 가로 크기(단위 : km)
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