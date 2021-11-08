#include "StdAfx.h"
#include "Scaler.h"
#include "GeoCommonFuc.h"
#include "MBR.h"
#include "GeoPoint.h"
#include "ENCCommon.h"

#include "../LatLonUtility/LatLonUtility.h"

//#include "../extlibs/MTP/include/MTP_CHART_DLL.h"

//#include "..\\extlibs\\Clipper\\include\\clipper.hpp"

//#ifdef MTP
////#include "../extlibs/SMT/include/SMT_CHART_DLL.h"
//
//#ifdef _DEBUG
//#pragma comment(lib, "../extlibs/SMT/lib/Debug/SMT_CHART_LIB.lib")
//#pragma comment(lib, "../extlibs/SMT/lib/Debug/SMT_ROUTE_LIB.lib")
//#pragma comment(lib, "../extlibs/SMT/lib/Debug/SMT_RCDS_LIB.lib")
//#pragma comment(lib, "../extlibs/SMT/lib/Debug/SMT_CHART_DIC_LIB.lib")
//#pragma comment(lib, "../extlibs/SMT/lib/Debug/SMT_450_LIB.lib")
//#else
//#pragma comment(lib, "../extlibs/SMT/lib/Release/SMT_CHART_LIB.lib")
//#pragma comment(lib, "../extlibs/SMT/lib/Release/SMT_ROUTE_LIB.lib")
//#pragma comment(lib, "../extlibs/SMT/lib/Release/SMT_RCDS_LIB.lib")
//#pragma comment(lib, "../extlibs/SMT/lib/Release/SMT_CHART_DIC_LIB.lib")
//#pragma comment(lib, "../extlibs/SMT/lib/Release/SMT_450_LIB.lib")
//#endif
//
//#pragma comment(lib, "MTP_NEW_SEARCH_LIB.lib")
//#pragma comment(lib, "MTP_CHART_LIB.lib")
//#endif

#include <math.h>

Scaler::Scaler(void)
{
	projection(mxMinLimit, myMinLimit);
	projection(mxMaxLimit, myMaxLimit);
}

Scaler::~Scaler(void)
{
}

#pragma warning(disable:4244)
void Scaler::DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate)
{
	if (XYtoSP)
	{
		int _mx = 0;
		int _my = 0;
		XYtoSP(sx, sy, _my, _mx, false, false);
		*mx = _mx;
		*my = _my;

		return;
	}


	double currentRatio = 0;
	double currentXRatio = (mxMax - mxMin) / (sxMax - sxMin);
	double currentYRatio = (myMax - myMin) / (syMax - syMin);

	if (currentXRatio > currentYRatio) {
		currentRatio = currentXRatio;
	}
	else {
		currentRatio = currentYRatio;
	}

	mox = (mxMax + mxMin) / 2;
	moy = (myMax + myMin) / 2;
	sox = (sxMax + sxMin) / 2;
	soy = (syMax + syMin) / 2;

	//if (rotate == TRUE) {
	//	if (((int)rotateDegree % 360) != 0) {
	//		// 이동할 점을 원점으로 이동
	//		sx -= sox;
	//		sy -= soy;

	//		// 실세계의 축으로 변환
	//		sy *= (-1);

	//		// 움직일 점과 원점 사이의 거리
	//		double d = sqrt((double)(sx)* (sx)+(sy)* (sy));

	//		// 기존 직선과 x축 사이의 각도
	//		double alpha = acos(abs((double)(sx) / d));

	//		// 회전할 각도 
	//		double theta = (-1) * rotateDegree * EXC;

	//		// alpha의 값을 [0, 2 * PI]안에서 실제 값으로 바꿔준다
	//		// 1사분면
	//		if ((sx >= 0) && (sy >= 0)) {

	//		}
	//		// 2사분면
	//		else if ((sx < 0) && (sy >= 0)) {
	//			alpha = 3.14 - alpha;
	//		}
	//		// 3사분면	
	//		else if ((sx < 0) && (sy < 0)) {
	//			alpha = 3.14 + alpha;
	//		}
	//		// 4사분면
	//		else if ((sx >= 0) && (sy < 0)) {
	//			alpha = (3.14 * 2) - alpha;
	//		}
	//		// ???
	//		else {
	//			//AfxMessageBox(_T("???"));
	//			return;
	//		}

	//		// 새로운 직선과 x축 사이의 각도
	//		double beta = alpha - theta;

	//		// 실세계에서의 새로운 좌표 저장
	//		sx = (LONG)(d * cos(beta));
	//		sy = (LONG)(d * sin(beta));

	//		// 스크린 좌표 체계로 복구	
	//		sy *= (-1);

	//		// 이동할 점을 원점으로 옮겼던 일을 복구
	//		sx += sox;
	//		sy += soy;
	//	}
	//}

	*mx = mox + (sx - sox) * currentRatio;
	*my = moy - (sy - soy) * currentRatio;
}

#pragma warning(disable:4244)
void Scaler::WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate)
{
	if (SPtoXY)
	{
		int _sx = 0;
		int _sy = 0;
		SPtoXY(my, mx, _sx, _sy, false, false);
		*sx = _sx;
		*sy = _sy;

		return;
	}

	double currentXRatio = (mxMax - mxMin) / (sxMax - sxMin);
	double currentYRatio = (myMax - myMin) / (syMax - syMin);

	double currentRatio = 0;
	if (currentXRatio > currentYRatio)
	{
		currentRatio = currentXRatio;
	}
	else
	{
		currentRatio = currentYRatio;
	}

	mox = (mxMax + mxMin) / 2;
	moy = (myMax + myMin) / 2;
	sox = (sxMax + sxMin) / 2;
	soy = (syMax + syMin) / 2;

	*sx = (LONG)((mx - mox) / currentRatio + sox);
	*sy = (LONG)((moy - my) / currentRatio + soy);

	//if (rotate == TRUE) {
	//	if (((int)rotateDegree % 360) != 0) {
	//		// 이동할 점을 원점으로 이동
	//		*sx -= sox;
	//		*sy -= soy;

	//		// 실세계의 축으로 변환
	//		*sy *= (-1);

	//		// 움직일 점과 원점 사이의 거리
	//		double d = sqrt((double)(*sx) * (*sx) + (*sy) * (*sy));

	//		// 기존 직선과 x축 사이의 각도
	//		double alpha = acos(abs((double)(*sx) / d));

	//		// 회전할 각도 
	//		double theta = rotateDegree * EXC;

	//		// alpha의 값을 [0, 2 * PI]안에서 실제 값으로 바꿔준다
	//		// 1사분면
	//		if ((*sx >= 0) && (*sy >= 0)) {

	//		}
	//		// 2사분면
	//		else if ((*sx < 0) && (*sy >= 0)) {
	//			alpha = 3.14 - alpha;
	//		}
	//		// 3사분면	
	//		else if ((*sx < 0) && (*sy < 0)) {
	//			alpha = 3.14 + alpha;
	//		}
	//		// 4사분면
	//		else if ((*sx >= 0) && (*sy < 0)) {
	//			alpha = (3.14 * 2) - alpha;
	//		}
	//		// ???
	//		else {
	//			//AfxMessageBox(_T("???"));
	//			return;
	//		}

	//		// 새로운 직선과 x축 사이의 각도
	//		double beta = alpha - theta;

	//		// 실세계에서의 새로운 좌표 저장
	//		*sx = (LONG)(d * cos(beta));
	//		*sy = (LONG)(d * sin(beta));

	//		// 스크린 좌표 체계로 복구
	//		*sy *= (-1);

	//		// 이동할 점을 원점으로 옮겼던 일을 복구
	//		*sx += sox;
	//		*sy += soy;
	//	}
	//}
}

#pragma warning(disable:4244)
void Scaler::WorldToDevice_F(double mx, double my, float *sx, float *sy, bool rotate)
{
	if (SPtoXY)
	{
		int _sx = 0;
		int _sy = 0;
		SPtoXY(my, mx, _sx, _sy, false, false);
		*sx = _sx;
		*sy = _sy;

		return;
	}

	double currentRatio = 0;
	double currentXRatio = (mxMax - mxMin) / (sxMax - sxMin);
	double currentYRatio = (myMax - myMin) / (syMax - syMin);

	if (currentXRatio > currentYRatio)
	{
		currentRatio = currentXRatio;
	}
	else
	{
		currentRatio = currentYRatio;
	}

	mox = (mxMax + mxMin) / 2;
	moy = (myMax + myMin) / 2;
	sox = (sxMax + sxMin) / 2;
	soy = (syMax + syMin) / 2;

	*sx = (float)((mx - mox) / currentRatio + sox);
	*sy = (float)((moy - my) / currentRatio + soy);

	//if (rotate == TRUE) {
	//	if (((int)rotateDegree % 360) != 0) {
	//		// 이동할 점을 원점으로 이동
	//		*sx -= sox;
	//		*sy -= soy;

	//		// 실세계의 축으로 변환
	//		*sy *= (-1);

	//		// 움직일 점과 원점 사이의 거리
	//		double d = sqrt((double)(*sx) * (*sx) + (*sy) * (*sy));

	//		// 기존 직선과 x축 사이의 각도
	//		double alpha = acos(abs((double)(*sx) / d));

	//		// 회전할 각도 
	//		double theta = rotateDegree * EXC;

	//		// alpha의 값을 [0, 2 * PI]안에서 실제 값으로 바꿔준다
	//		// 1사분면
	//		if ((*sx >= 0) && (*sy >= 0)) {

	//		}
	//		// 2사분면
	//		else if ((*sx < 0) && (*sy >= 0)) {
	//			alpha = 3.14 - alpha;
	//		}
	//		// 3사분면	
	//		else if ((*sx < 0) && (*sy < 0)) {
	//			alpha = 3.14 + alpha;
	//		}
	//		// 4사분면
	//		else if ((*sx >= 0) && (*sy < 0)) {
	//			alpha = (3.14 * 2) - alpha;
	//		}
	//		// ???
	//		else {
	//			//AfxMessageBox(_T("???"));
	//			return;
	//		}

	//		// 새로운 직선과 x축 사이의 각도
	//		double beta = alpha - theta;

	//		// 실세계에서의 새로운 좌표 저장
	//		*sx = (float)(d * cos(beta));
	//		*sy = (float)(d * sin(beta));

	//		// 스크린 좌표 체계로 복구
	//		*sy *= (-1);

	//		// 이동할 점을 원점으로 옮겼던 일을 복구
	//		*sx += sox;
	//		*sy += soy;
	//	}
	//}
}

void Scaler::SetScreen(RECT *rect)
{
	if (rect == nullptr) return;

	SetScreen(*rect);

	//sxMin = rect->left;
	//syMin = rect->top;
	//sxMax = rect->right;
	//syMax = rect->bottom;

	//sox = (sxMin + sxMax) / 2;
	//soy = (syMin + syMax) / 2;

	//UpdateScale();
}

void Scaler::SetScreen(const RECT& rect)
{
	sxMin = rect.left;
	syMin = rect.top;
	sxMax = rect.right;
	syMax = rect.bottom;

	sox = (sxMin + sxMax) / 2;
	soy = (syMin + syMax) / 2;

	UpdateScale();
}

void Scaler::SetMap(double xmin, double ymin, double xmax, double ymax)
{
	mxMin = xmin;
	myMin = ymin;
	mxMax = xmax;
	myMax = ymax;

	mox = (mxMin + mxMax) / 2;
	moy = (myMin + myMax) / 2;
	
	if (rotateDegree)
	{
		Gdiplus::PointF l_u(mxMin - mox, myMax - moy);
		Gdiplus::PointF l_d(mxMin - mox, myMin - moy);
		Gdiplus::PointF r_u(mxMax - mox, myMax - moy);
		Gdiplus::PointF r_d(mxMax - mox, myMin - moy);

		double radian = (180 - rotateDegree) * DEG2RAD;
		double sinValue = sin(radian);
		double cosValue = cos(radian);

		FLOAT tempX = l_u.Y * sinValue + l_u.X * cosValue;
		FLOAT tempY = l_u.Y * cosValue - l_u.X * sinValue;
		
		l_u.X = tempX;
		l_u.Y = tempY;

		tempX = l_d.Y * sinValue + l_d.X * cosValue;
		tempY = l_d.Y * cosValue - l_d.X * sinValue;

		l_d.X = tempX;
		l_d.Y = tempY;

		tempX = r_u.Y * sinValue + r_u.X * cosValue;
		tempY = r_u.Y * cosValue - r_u.X * sinValue;

		r_u.X = tempX;
		r_u.Y = tempY;

		tempX = r_d.Y * sinValue + r_d.X * cosValue;
		tempY = r_d.Y * cosValue - r_d.X * sinValue;

		r_d.X = tempX;
		r_d.Y = tempY;

		mxMinCalcMBR = l_u.X;
		mxMaxCalcMBR = l_u.X;
		myMinCalcMBR = l_u.Y;
		myMaxCalcMBR = l_u.Y;

		mxMinCalcMBR = l_d.X < mxMinCalcMBR ? l_d.X : mxMinCalcMBR;
		mxMaxCalcMBR = l_d.X > mxMaxCalcMBR ? l_d.X : mxMaxCalcMBR;
		myMinCalcMBR = l_d.Y < myMinCalcMBR ? l_d.Y : myMinCalcMBR;
		myMaxCalcMBR = l_d.Y > myMaxCalcMBR ? l_d.Y : myMaxCalcMBR;
		
		mxMinCalcMBR = r_u.X < mxMinCalcMBR ? r_u.X : mxMinCalcMBR;
		mxMaxCalcMBR = r_u.X > mxMaxCalcMBR ? r_u.X : mxMaxCalcMBR;
		myMinCalcMBR = r_u.Y < myMinCalcMBR ? r_u.Y : myMinCalcMBR;
		myMaxCalcMBR = r_u.Y > myMaxCalcMBR ? r_u.Y : myMaxCalcMBR;

		mxMinCalcMBR = r_d.X < mxMinCalcMBR ? r_d.X : mxMinCalcMBR;
		mxMaxCalcMBR = r_d.X > mxMaxCalcMBR ? r_d.X : mxMaxCalcMBR;
		myMinCalcMBR = r_d.Y < myMinCalcMBR ? r_d.Y : myMinCalcMBR;
		myMaxCalcMBR = r_d.Y > myMaxCalcMBR ? r_d.Y : myMaxCalcMBR;
		
		mxMinCalcMBR += mox;
		mxMaxCalcMBR += mox;
		myMinCalcMBR += moy;
		myMaxCalcMBR += moy;
	}
	else
	{
		mxMinCalcMBR = mxMin;
		mxMaxCalcMBR = mxMax;
		myMinCalcMBR = myMin;
		myMaxCalcMBR = myMax;
	}

	//AdjustScreenMap();
	UpdateScale();
}

void Scaler::SetMap(MBR mbr)
{
	SetMap(mbr.xmin, mbr.ymin, mbr.xmax, mbr.ymax);
	//mxMin = mbr.xmin;
	//myMin = mbr.ymin;
	//mxMax = mbr.xmax;
	//myMax = mbr.ymax;

	//mox = (mxMin + mxMax) / 2;
	//moy = (myMin + myMax) / 2;
	//
	//UpdateScale();
	//AdjustScreenMap();
}

void Scaler::SetMap(RECT r)
{
	double xmin = 0;
	double ymin = 0;
	double xmax = 0;
	double ymax = 0;

	DeviceToWorld(r.left, r.bottom, &xmin, &ymin);
	DeviceToWorld(r.right, r.top, &xmax, &ymax);

	SetMap(xmin, ymin, xmax, ymax);
}

#pragma warning(disable:4244)
int Scaler::GetCurrentScale()
{
	return currentScale;
}

void Scaler::UpdateScale()
{
	// 지도의 X축 범위
	double _mxMin = mxMin;
	double _mxMax = mxMax;

	if (ENCCommon::USING_MTP)
	{
		_mxMin /= 10000000.0;
		_mxMax /= 10000000.0;
		_mxMin *= DEG2RAD;
		_mxMax *= DEG2RAD;
	}

	// 지도의 X축의 실제거리 (km)
	double distance = (_mxMax - _mxMin) * EARTH_RADIUS;

	// 화면 넓이
	int width = GetScreenWidth();

	// 픽셀당 km
	double KMPerPixel = distance / (double)width;

	// 축척 계산
	currentScale = KMPerPixel * 1000 * 100 * DOTPERCM;
}

#pragma warning(disable:4244)
void Scaler::SetScale(int scale)
{
	// 원래 지도 중심점
	double oldMox = mox;
	double oldMoy = moy;

	// 지도 중심점을 위경도 0, 0으로 변경
	MoveMap(sox, soy, 0, 0);

	// 화면 중심점의 실제 좌표를 구함(radian)
	inverseProjection(mox, moy);

	double finalWidthKM = GetScreenWidthKM() * (double)scale;
	double offsetXtheta = finalWidthKM / (2 * EARTH_RADIUS);

	if (ENCCommon::USING_MTP)
	{
		offsetXtheta *= RAD2DEG;
	}

	// X좌표는 투영좌표가 실제(radian) 좌표
	mxMin = mox - offsetXtheta;
	mxMax = mox + offsetXtheta;

	double finalHeightKM = (finalWidthKM * (double)GetScreenHeight()) / (double)GetScreenWidth();
	double offsetYtheta = finalHeightKM / (2 * EARTH_RADIUS);

	myMin = moy - offsetYtheta;
	myMax = moy + offsetYtheta;

	if (ENCCommon::USING_MTP)
	{
		offsetYtheta *= RAD2DEG;
	}

	//double DEG2RAD = acos(-1.0) / 180;
	double yLimit = 85;
	if (ENCCommon::USING_MTP == false)
	{
		yLimit *= DEG2RAD;
	}

	if (myMin < -yLimit) 
	{
		myMin = -yLimit;
	}
	
	if (myMax > yLimit)
	{
		myMax = yLimit;
	}

	if (ENCCommon::USING_MTP)
	{
		myMin *= DEG2RAD;
		myMax *= DEG2RAD;
		moy *= DEG2RAD;
	}

	if (ENCCommon::USING_MTP == false)
	{
		myMin = log(tan((myMin / 2.0) + M_PI_D4));
		myMax = log(tan((myMax / 2.0) + M_PI_D4));
		moy = log(tan((moy / 2.0) + M_PI_D4));
	}
	else
	{
		myMin *= 10000000;
		myMax *= 10000000;
		moy *= 10000000;
	}

	//if (ENCCommon::USING_MTP)
	//{
	//	myMin *= RAD2DEG;
	//	myMax *= RAD2DEG;
	//	moy *= RAD2DEG;
	//}

	//AdjustScreenMap();

	MoveMap(sox, soy, oldMox, oldMoy);

	UpdateScale();
}

double Scaler::GetRotationDegree()
{
	return rotateDegree;
}

void Scaler::SetRotationDegree(double _value)
{
	rotateDegree = _value;
}




//void Scaler::Projection(double *x, double *y)
//{
//	projection(*x, *y);
//}


//void Scaler::InverseProjection(double *x, double *y)
//{
//	inverseProjection(*x, *y);
//
//	//double pi = M_PI;
//	//double RADDEG = (180.0 / pi);
//
//	//*x *= RADDEG;
//	///*
//	//// X 좌표 값을 (-180, 180) 범위 안에 포함되게 수정 한다.
//	//while (*x < -180) {
//	//	*x += 360;
//	//}
//
//	//while (*x > 180) {
//	//	*x -= 360;
//	//}
//	//*/
//	//*y = 2 * (atan(exp(*y)) - (pi / 4.0));
//	//*y *= RADDEG;
//}


void Scaler::ZoomIn(double value)
{
	double old_mox = mox;
	double old_moy = moy;
	double old_mxMin = mxMin;
	double old_myMin = myMin;
	double old_mxMax = mxMax;
	double old_myMax = myMax;

	double mWidth = mxMax - mxMin;
	double mHeight = myMax - myMin;

	double zoomValue = (100.0 - value) / 100.0;

	mWidth  *= zoomValue;
	mHeight *= zoomValue;

	mxMin = mox - (mWidth / 2);
	mxMax = mox + (mWidth / 2);
	myMin = moy - (mHeight / 2);
	myMax = moy + (mHeight / 2);

	UpdateScale();

	if (GetCurrentScale() < 500) 
	{
		mox = old_mox;
		moy = old_moy;
		mxMin = old_mxMin;
		myMin = old_myMin;
		mxMax = old_mxMax;
		myMax = old_myMax;

		UpdateScale();
	}
}


void Scaler::ZoomIn(double value, int x, int y)
{
	double mx = 0;
	double my = 0;

	DeviceToWorld(x, y, &mx, &my);

	if (mx >= 0)
	{
		while (mx > (acos(-1.0)))
		{
			mx -= (2 * acos(-1.0));
		}
	}
	else
	{
		while (mx < (-acos(-1.0)))
		{
			mx += (2 * acos(-1.0));
		}
	}

	ZoomIn(value);

	MoveMap(x, y, mx, my);

	UpdateScale();
}


void Scaler::ZoomOut(double value)
{
	double mWidth = mxMax - mxMin;
	double mHeight = myMax - myMin;
	double zoomValue = (100.0 + value) / 100.0;

	mWidth  *= zoomValue;
	mHeight *= zoomValue;

	mxMin = mox - (mWidth / 2);
	mxMax = mox + (mWidth / 2);
	myMin = moy - (mHeight / 2);
	myMax = moy + (mHeight / 2);

	AdjustScreenMap();

	UpdateScale();
}

void Scaler::ZoomOut(double value, int x, int y)
{
	double oldMx, oldMy;

	DeviceToWorld(x, y, &oldMx, &oldMy);

	ZoomOut(value);

	MoveMap(x, y, oldMx, oldMy);

	UpdateScale();
}

#pragma warning(disable:4244)
void Scaler::MoveMap(int x, int y)
{
	MoveMap(sox + x, soy + y, mox, moy); 
}

#pragma warning(disable:4244)
void Scaler::MoveMap(int sx, int sy, double mx, double my)
{
	PrivateMoveMap(sx, sy, mx, my);

	if (ENCCommon::USING_MTP)
	{
		return;
	}

	LONG sx1 = 0, sy1 = 0;     // 지도의 오른쪽
	LONG sx2 = 0, sy2 = 0;	   // 지도의 왼쪽

	double x = 180;
	double y = 0;

	projection(x, y);

	WorldToDevice(x, 0, &sx1, &sy1);
	WorldToDevice(-x, 0, &sx2, &sy2);

	double xlimit = acos(-1.0);
	if (ENCCommon::USING_MTP)
	{
		xlimit *= RAD2DEG;
	}

	/*
	** 축척이 3,500,000 보다 대축척이면 무한 화면회전 사용하지 않음
	*/
	if (currentScale < 3500000) {

		// 지도가 화면 왼쪽으로 나간 경우
		//if (mxMin > acos(-1.0)) {
		if (mxMin > 180) {
			//PrivateMoveMap(0, sy, -mx, my);
			//MoveMap(sxMax - sxMin, 0);
			MoveMap(sx1 - sx2, 0);
		}
		//else if (mxMax < -acos(-1.0)) {
		if (mxMax < -180) {
			//PrivateMoveMap(sxMax, sy, mx, my);
			//MoveMap(-(sxMax - sxMin), 0);
			MoveMap(sx2 - sx1, 0);
		}

		return;
	}


	if (sx1 <= 0) {
		PrivateMoveMap(0, sy, -xlimit, my);
	}
	// 지도가 화면 오른쪽으로 나간 경우
	else if (sx2 >= sxMax) {
		PrivateMoveMap(sxMax, sy, xlimit, my);
	}

	//if (rotateDegree == 0) {
	//	// 지도가 화면 왼쪽으로 나간 경우
	//	if (sx1 <= 0) {
	//		PrivateMoveMap(0, sy, -xlimit, my);
	//	}
	//	// 지도가 화면 오른쪽으로 나간 경우
	//	else if (sx2 >= sxMax) {
	//		PrivateMoveMap(sxMax, sy, xlimit, my);
	//	}
	//}
	//else if (rotateDegree == 90) {
	//	// 지도가 화면 위쪽으로 나간 경우
	//	if (sy1 <= 0) {
	//		PrivateMoveMap(sx, 0, -acos(-1.0), my);
	//	}
	//	// 지도가 화면 아래쪽으로 나간 경우
	//	else if (sy2 >= syMax) {
	//		PrivateMoveMap(sx, syMax, acos(-1.0), my);
	//	}
	//}
	//else if (rotateDegree == 180) {
	//	// 지도가 화면 왼쪽으로 나간 경우
	//	if (sx2 <= 0) {
	//		PrivateMoveMap(0, sy, acos(-1.0), my);
	//	}
	//	// 지도가 화면 오른쪽으로 나간 경우
	//	else if (sx1 >= sxMax) {
	//		PrivateMoveMap(sxMax, sy, -acos(-1.0), my);
	//	}
	//}
	//else if (rotateDegree == 270) {
	//	// 지도가 화면 위쪽으로 나간 경우
	//	if (sy2 <= 0) {
	//		PrivateMoveMap(sx, 0, acos(-1.0), my);
	//	}
	//	// 지도가 화면 아래쪽으로 나간 경우
	//	else if (sy1 >= syMax) {
	//		PrivateMoveMap(sx, syMax, -acos(-1.0), my);
	//	}
	//}
	//else if ((rotateDegree > 0) && (rotateDegree < 90)) {
	//	LONG x1, y1, x2, y2, x3, y3, x4, y4;

	//	WorldToDevice(-180, 85, &x1, &y1);
	//	WorldToDevice(-180, -85, &x2, &y2);
	//	WorldToDevice(180, 85, &x3, &y3);
	//	WorldToDevice(180, -85, &x4, &y4);
	//				
	//	// 지도가 화면의 좌상단 밖으로 나간 경우
	//	if (((x4 < sxMin) && (y4 < syMax)) && ((x3 < sxMax) && (y3 < syMin))) {
	//		LONG old_sx, old_sy;

	//		WorldToDevice(180, 0, &old_sx, &old_sy);
	//					
	//		PrivateMoveMap(old_sx, old_sy, -acos(-1.0), 0);
	//	}
	//				
	//	// 지도가 화면의 우하단 밖으로 나간 경우
	//	if (((x1 > sxMax) && (y1 > syMin)) && ((x2 > sxMin) && (y2 > syMax))) {
	//		LONG old_sx, old_sy;

	//		WorldToDevice(-180, 0, &old_sx, &old_sy);
	//					
	//		PrivateMoveMap(old_sx, old_sy, acos(-1.0), 0);
	//	}
	//}
	//else if ((rotateDegree > 90) && (rotateDegree < 180)) {
	//	LONG x1, y1, x2, y2, x3, y3, x4, y4;

	//	WorldToDevice(-180, 85, &x1, &y1);
	//	WorldToDevice(-180, -85, &x2, &y2);
	//	WorldToDevice(180, 85, &x3, &y3);
	//	WorldToDevice(180, -85, &x4, &y4);
	//				
	//	// 지도가 화면의 좌하단 밖으로 나간 경우
	//	if (((x1 < sxMax) && (y1 > syMax)) && ((x2 < sxMin) && (y2 > syMin))) {
	//		LONG old_sx, old_sy;

	//		WorldToDevice(-180, 0, &old_sx, &old_sy);
	//					
	//		PrivateMoveMap(old_sx, old_sy, acos(-1.0), 0);
	//	}
	//				
	//	// 지도가 화면의 우상단 밖으로 나간 경우
	//	if (((x3 > sxMax) && (y3 < syMax)) && ((x4 > sxMin) && (y4 < syMin))) {
	//		LONG old_sx, old_sy;

	//		WorldToDevice(180, 0, &old_sx, &old_sy);
	//					
	//		PrivateMoveMap(old_sx, old_sy,-acos(-1.0), 0);
	//	}
	//}
	//else if ((rotateDegree > 180) && (rotateDegree < 270)) {
	//	LONG x1, y1, x2, y2, x3, y3, x4, y4;

	//	WorldToDevice(-180, 85, &x1, &y1);
	//	WorldToDevice(-180, -85, &x2, &y2);
	//	WorldToDevice(180, 85, &x3, &y3);
	//	WorldToDevice(180, -85, &x4, &y4);
	//				
	//	// 지도가 화면의 좌상단 밖으로 나간 경우
	//	if (((x2 < sxMax) && (y2 < syMin)) && ((x1 < sxMin) && (y1 < syMax))) {
	//		LONG old_sx, old_sy;

	//		WorldToDevice(-180, 0, &old_sx, &old_sy);
	//					
	//		PrivateMoveMap(old_sx, old_sy, acos(-1.0), 0);
	//	}
	//				
	//	// 지도가 화면의 우하단 밖으로 나간 경우
	//	if (((x3 > sxMin) && (y3 > syMax)) && ((x4 > sxMax) && (y4 > syMin))) {
	//		LONG old_sx, old_sy;

	//		WorldToDevice(180, 0, &old_sx, &old_sy);
	//					
	//		PrivateMoveMap(old_sx, old_sy, -acos(-1.0), 0);
	//	}
	//}
	//else if ((rotateDegree > 270) && (rotateDegree < 360)) {
	//	LONG x1, y1, x2, y2, x3, y3, x4, y4;

	//	WorldToDevice(-180, 85, &x1, &y1);
	//	WorldToDevice(-180, -85, &x2, &y2);
	//	WorldToDevice(180, 85, &x3, &y3);
	//	WorldToDevice(180, -85, &x4, &y4);
	//				
	//	// 지도가 화면의 좌하단 밖으로 나간 경우
	//	if (((x4 < sxMax) && (y4 > syMax)) && ((x3 < sxMin) && (y3 > syMin))) {
	//		LONG old_sx, old_sy;

	//		WorldToDevice(180, 0, &old_sx, &old_sy);
	//					
	//		PrivateMoveMap(old_sx, old_sy, -acos(-1.0), 0);
	//	}
	//				
	//	// 지도가 화면의 우상단 밖으로 나간 경우
	//	if (((x1 > sxMin) && (y1 < syMin)) && ((x2 > sxMax) && (y2 < syMax))) {
	//		LONG old_sx, old_sy;

	//		WorldToDevice(-180, 0, &old_sx, &old_sy);
	//					
	//		PrivateMoveMap(old_sx, old_sy, acos(-1.0), 0);
	//	}
	//}
}

void Scaler::PrivateMoveMap(int sx, int sy, double mx, double my)
{
	double mx1 = 0;
	double my1 = 0;

	// 이동하려는 지점의 지도 좌표
	DeviceToWorld(sx, sy, &mx1, &my1);

	// 새로운 지도 중심점
	mox += (mx - mx1);
	moy += (my - my1);

	double mWidth = mxMax - mxMin;
	double mHeight = myMax - myMin;

	// 새로운 지도 범위
	mxMin = mox - (mWidth / 2);
	mxMax = mox + (mWidth / 2);
	myMin = moy - (mHeight / 2);
	myMax = moy + (mHeight / 2);

	// y 축이 범위 벗어나면 조정
	if (myMin < myMinLimit)		
	{
		myMin = myMinLimit;
		myMax = myMin + mHeight;
		moy = (myMin + myMax) / 2;
	}

	if (myMax > myMaxLimit) 
	{
		myMax = myMaxLimit; 
		myMin = myMax - mHeight;
		moy = (myMin + myMax) / 2;
	}
}


#pragma warning(disable:4244)
void Scaler::Rotate(LONG *sx, LONG *sy, double degree) {
	if (((int)degree % 360) != 0) {
		// 이동할 점을 원점으로 이동
		*sx -= sox;
		*sy -= soy;

		// 실세계의 축으로 변환
		*sy *= (-1);

		// 움직일 점과 원점 사이의 거리
		double d = sqrt((double)(*sx) * (*sx) + (*sy) * (*sy));

		// 기존 직선과 x축 사이의 각도
		double alpha = acos(abs((double)(*sx) / d));

		// 회전할 각도 
		double theta = rotateDegree * EXC;

		// alpha의 값을 [0, 2 * PI]안에서 실제 값으로 바꿔준다
		// 1사분면
		if ((*sx >= 0) && (*sy >= 0)) {

		}
		// 2사분면
		else if ((*sx < 0) && (*sy >= 0)) {
			alpha = 3.14 - alpha;
		}
		// 3사분면	
		else if ((*sx < 0) && (*sy < 0)) {
			alpha = 3.14 + alpha;
		}
		// 4사분면
		else if ((*sx >= 0) && (*sy < 0)) {
			alpha = (3.14 * 2) - alpha;
		} 
		// ???
		else {
			//AfxMessageBox(_T("???"));
			return;
		}

		// 새로운 직선과 x축 사이의 각도
		double beta = alpha - theta;

		// 실세계에서의 새로운 좌표 저장
		*sx = (LONG)(d * cos(beta));
		*sy = (LONG)(d * sin(beta));

		// 스크린 좌표 체계로 복구
		*sy *= (-1);

		// 이동할 점을 원점으로 옮겼던 일을 복구
		*sx += sox;
		*sy += soy;
	}
}


void Scaler::RotateMap(double degree)
{
	rotateDegree += degree;

	if (rotateDegree >= 360.0) {
		rotateDegree -= 360.0;
	}

	if (rotateDegree < 0) {
		rotateDegree += 360.0;
	}
}


void Scaler::NorthUp()
{
	rotateDegree = 0;
}


//double Scaler::GetDistanceRhumbLineMercator(double lat1, double lon1, double lat2, double lon2, bool radian)
//{
//	//double DEG2RAD = acos(-1.0) / 180;
//	double PI = acos(-1.0);
//
//	// 입력값의 단위가 degree인 경우
//	if (radian == false)
//	{
//		lat1 *= DEG2RAD;
//		lon1 *= DEG2RAD;
//		lat2 *= DEG2RAD;
//		lon2 *= DEG2RAD;
//	}
//
//	double dLat = lat2 - lat1;
//	double dLon = abs(lon2 - lon1);
//
//	// if dLon over PI take shorter rhumb line across the anti-meridian
//	if (abs(dLon) > PI) 
//	{
//		dLon = (dLon > 0)? -(2 * PI - dLon) : (2 * PI + dLon);
//	}
//
//	// on Mercator projection, lonitude gets increasing stretched by latitude; q is the 'stretch factor'
//	double d = log(tan((lat2 / 2) + (PI / 4)) / tan((lat1 / 2) + (PI / 4)));
//
//	// the stretch factor becomes ill-conditioned along E-W line (0/0); use empirical toerance to avoid it
//	double q = (abs(d) > DBL_MIN)? (dLat / d) : cos(lat1);
//
//	// distance is pythagoras on 'stretched' Mercator projection
//	double theta = sqrt((dLat * dLat) + (q * q * dLon * dLon)); // angular distance in radians
//	return theta * EARTH_RADIUS;
//}


void Scaler::AdjustScreenMap()
{
	AdjustScreenMap_Internal();
	AdjustScreenMap_Internal();
	AdjustScreenMap_Internal();
	AdjustScreenMap_Internal();
	AdjustScreenMap_Internal();
}

#pragma warning(disable:4244)
void Scaler::AdjustScreenMap_Internal()
{
	DeviceToWorld(sxMin, syMax, &mxMin, &myMin);
	DeviceToWorld(sxMax, syMin, &mxMax, &myMax);
	mox = (mxMax + mxMin) / 2;
	moy = (myMax + myMin) / 2;

	// View나 Map의 크기가 지정되지 않은경우 
	if (((myMaxLimit - myMinLimit) == 0) || ((syMax - syMin) == 0))
	{
		return;
	}

	double mRatio = (mxMaxLimit - mxMinLimit) / (myMaxLimit - myMinLimit); // 지도 가로 세로 비율
	double sRatio = (sxMax - sxMin) / (syMax - syMin); // 화면 가로 세로 비율

	// 화면이 더 납작
	// 지도의 가로가 가득 차게
	if (mRatio < sRatio)
	{
 		if ((mxMin > mxMinLimit) || (mxMax < mxMaxLimit))
		{
			if (((mxMax - mxMin) < (mxMaxLimit - mxMinLimit)))
			{
				return;
			}
		}

		myMax = (mxMaxLimit * (syMax - syMin)) / (sxMax - sxMin);
		myMin = (mxMinLimit * (syMax - syMin)) / (sxMax - sxMin);

		mxMin = mxMinLimit;
		mxMax = mxMaxLimit;

		mox = (mxMin + mxMax) / 2;
		moy = (myMin + myMax) / 2;
	}
	else if (mRatio == sRatio)
	{
		mxMin = mxMinLimit;
		myMin = myMinLimit;
		mxMax = mxMaxLimit;
		myMax = myMaxLimit;
	}
	// 지도가 더 납작
	// 지도와 세로가 가득 차게
	else
	{
		if ((myMin > myMinLimit) || (myMax < myMaxLimit))
		{
			if ((myMax - myMin) < (myMaxLimit - myMinLimit))
			{
				return;
			}
		}

		mxMin = (myMinLimit * (sxMax - sxMin)) / (syMax - syMin);
		mxMax = (myMaxLimit * (sxMax - sxMin)) / (syMax - syMin);

		myMin = myMinLimit;
		myMax = myMaxLimit;

		mox = (mxMin + mxMax) / 2;
		moy = (myMin + myMin) / 2;
	}
}

double Scaler::GetMapWidth()
{
	return mxMax - mxMin;
}

double Scaler::GetMapHeight()
{
	return myMax - myMin;
}

void Scaler::GetMap(MBR *mbr)
{
	MBR _mbr(mxMin, myMin, mxMax, myMax);

	if (mbr == nullptr)
	{
		mbr = new MBR();
	}

	mbr->SetMBR(_mbr);
}

MBR Scaler::GetMap()
{
	MBR mbr(mxMin, myMin, mxMax, myMax);

	return mbr;
}

MBR Scaler::GetMapCalcMBR()
{
	//MBR(double _xmin, double _ymin, double _xmax, double _ymax);
	MBR mbr(mxMinCalcMBR, myMinCalcMBR, mxMaxCalcMBR, myMaxCalcMBR);

	return mbr;
}
//double Scaler::GetDistanceLatitudeLongitude(double longitude1, double latitude1, double longitude2, double latitude2)
//{
//	// 두 점의 위경도 값을 x, y, z값으로 변환
//	double x1, y1, z1; 
//	double x2, y2, z2;
//	double radiusOfEarth = 6378.1;
//
//	x1 = cos(latitude1) * cos(longitude1);
//	y1 = cos(latitude1) * sin(longitude1);
//	z1 = sin(latitude1);
//
//	x2 = cos(latitude2) * cos(longitude2);
//	y2 = cos(latitude2) * sin(longitude2);
//	z2 = sin(latitude2);
//
//	// 두 벡터 사이의 거리를 구한다.
//	double theta = acos((x1 * x2) + (y1 * y2) + (z1 * z2));
//
//	// 호의 길이 구하는 공식으로 거리를 구한다.
//	double distance = theta * radiusOfEarth;
//
//	return distance;
//}

double Scaler::GetDistanceScreen(int x1, int y1, int x2, int y2)
{
	return sqrt((double)((x2 - x1) * (x2 - x1)  + (y2 - y1) * (y2 - y1)));
}

double Scaler::GetScreenWidthKM()
{
	int width = GetScreenWidth();
	double widthCM = (double)width / (double)DOTPERCM;

	return (double)widthCM * pow(10.0, -5);
}

#pragma warning(disable:4244)
int Scaler::GetScreenWidth()
{
	return sxMax - sxMin + 1;
}

int Scaler::GetScreenHeight()
{
	return syMax - syMin + 1;
}


CRect Scaler::GetScreenRect()
{
	return CRect(0, 0, GetScreenWidth(), GetScreenHeight());
}


#pragma warning(disable:4244)
D2D1_RECT_F Scaler::GetD2Rect()
{
	return D2D1::RectF(0, 0, GetScreenWidth(), GetScreenHeight());
}


bool Scaler::PtInMap(double _x, double _y)
{
	if ((_x >= mxMin) && (_x <= mxMax) && (_y >= myMin) && (_y <= myMax)) {
		return true;
	}

	return false;
}


//ClipperLib::PolyTree* Scaler::ClipPolyline(GeoPoint* _p, int _cnt)
//{
//	ClipperLib::Path  polyline, view;
//	ClipperLib::PolyTree* result = new ClipperLib::PolyTree();
//	ClipperLib::Clipper clipper;
//
//	ClipperLib::IntPoint tmp;
//
//	tmp.X = (ClipperLib::cInt)(mxMin * COMF);
//	tmp.Y = (ClipperLib::cInt)(myMax * COMF);
//	view.push_back(tmp);
//	tmp.X = (ClipperLib::cInt)(mxMax * COMF);
//	tmp.Y = (ClipperLib::cInt)(myMax * COMF);
//	view.push_back(tmp);
//	tmp.X = (ClipperLib::cInt)(mxMax * COMF);
//	tmp.Y = (ClipperLib::cInt)(myMin * COMF);
//	view.push_back(tmp);
//	tmp.X = (ClipperLib::cInt)(mxMin * COMF);
//	tmp.Y = (ClipperLib::cInt)(myMin * COMF);
//	view.push_back(tmp);
//	view.push_back(*view.begin());
//
//	for (int i = 0; i < _cnt; i++) {
//		tmp.X = (ClipperLib::cInt)(_p[i].x * COMF);
//		tmp.Y = (ClipperLib::cInt)(_p[i].y * COMF);
//		polyline.push_back(tmp);
//	}
//
//	clipper.AddPath(polyline, ClipperLib::ptSubject, false);
//	clipper.AddPath(view, ClipperLib::ptClip, true);
//	clipper.Execute(ClipperLib::ctIntersection, *result);
//
//	return result;
//}


#pragma warning(disable:4244)
D2D1::Matrix3x2F Scaler::GetMatrix()
{
	D2D1::Matrix3x2F result = D2D1::Matrix3x2F::Identity();

	auto translation1 = D2D1::Matrix3x2F::Translation(-mox, moy);

	double ratio = (sxMax - sxMin) / (mxMax - mxMin);

	auto scale = D2D1::Matrix3x2F::Scale(ratio, ratio);

	auto translation2 = D2D1::Matrix3x2F::Translation(sox, soy);

	result = translation1 * scale * translation2;

	return result;
}


D2D1::Matrix3x2F Scaler::GetInverseMatrix()
{
	auto matrix = GetMatrix();
	matrix.Invert();
	return matrix;
}


CRect Scaler::WGS84MBRtoCRect(MBR& mbr)
{
	CRect rect;
	WorldToDevice(mbr.GetXMin(), mbr.GetYMin(), &rect.left, &rect.bottom);
	WorldToDevice(mbr.GetXMax(), mbr.GetYMax(), &rect.right, &rect.top);
	return rect;
}

double Scaler::GetMapXMin()
{
	return mxMin;
}

double Scaler::GetMapYMin()
{
	return myMin;
}

double Scaler::GetMapXMax()
{
	return mxMax;
}

double Scaler::GetMapYMax()
{
	return myMax;
}

long long Scaler::GetCOMF()
{
	if (ENCCommon::USING_MTP)
	{
		return 1;
	}

	return COMF;
}