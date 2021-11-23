#include "pch.h"
#include "GeoPointZ.h"

#include <sstream>

GeoPointZ::GeoPointZ(void)
{
}

GeoPointZ::GeoPointZ(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
}

GeoPointZ::~GeoPointZ(void)
{

}

void GeoPointZ::SetPoint(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;

}
double GeoPointZ::GetZ()
{
	return z;
}

//void GeoPointZ::DrawGeometry(HDC &hDC, Scaler *scaler, double offset)
//{
//	//CPoint point;
//	POINT point;
//	scaler->WorldToDevice(x + offset, y, &(point.x), &(point.y));
//	// 폴리곤 색을 바꾸는 부분
////	CBrush newBrush = CBrush(RGB(165, 191, 221));
////	CBrush *oldBrush = pDC->SelectObject(&newBrush);
//	//pDC->Ellipse(point.x - 2, point.y - 2, point.x + 2, point.y + 2);
////	CString str;
////	str.Format(_T("%0.1lf"),z);
//
////	pDC->TextOutW(point.x - 15, point.y - 30, str, str.GetLength());
//	std::wstringstream streamVal;
//	streamVal << z;
//	std::wstring strVal = streamVal.str();
//	::TextOut(hDC, point.x - 15, point.y - 30, strVal.c_str(), (int)strVal.size());
//}