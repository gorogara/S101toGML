#include "pch.h"
#include "S100Layer.h"
#include "GISLibrary.h"
#include "Catalog.h"
#include "PortrayalCatalogue.h"
#include "LibMFCUtil.h"
#include "S100SpatialObject.h"

//#include "..\\LibMFCUtil\\LibMFCUtil.h"
//#include "..\\FeatureCatalog\\Catalog.h"
//#include "..\\S101Portrayal\\PortrayalCatalogue.h"


S100Layer::S100Layer() : Layer()
{
	hasIndividualFC = false;
	hasIndividualPC = false;
}


S100Layer::S100Layer(Catalog *fc, PortrayalCatalogue *pc) : Layer()
{
	SetFC(fc);
	SetPC(pc);
}


S100Layer::~S100Layer()
{
	DeleteCatalog();
}


void S100Layer::SetFC(Catalog *value)
{
	featureCatalogue = value;
}


Catalog* S100Layer::GetFC()
{
	return featureCatalogue;
}

FeatureCatalogue* S100Layer::GetFeatureCatalog()
{
	if (nullptr == GetFC())
	{
		return nullptr;
	}

	return GetFC()->GetFC();
}

void S100Layer::SetPC(PortrayalCatalogue *value)
{
	portrayalCatalogue = value;
}


PortrayalCatalogue* S100Layer::GetPC()
{
	return portrayalCatalogue;
}

bool S100Layer::OpenFC(CString path)
{
	try
	{
		Catalog* catalog = new Catalog(std::wstring(path));
		SetIndividualFC(true);
		SetFC(catalog);
		return true;
	}
	catch (int /*e*/)
	{
		SetIndividualFC(false);
		return false;
	}

	return false;
}

bool S100Layer::OpenPC(CString path)
{
	try
	{
		auto filePath = LibMFCUtil::GetFolderPathFromFilePath(path);
		PortrayalCatalogue* pc = new PortrayalCatalogue(std::wstring(filePath));

		pc->CreatePatternImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
		pc->CreateLineImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));

		SetIndividualPC(true);
		SetPC(pc);
		return true;
	}
	catch (int /*e*/)
	{
		SetIndividualPC(false);
		return false;
	}
	return false;
}

void S100Layer::SetIndividualFC(bool value) 
{
	hasIndividualFC = value;
}

bool S100Layer::GetIndividualFC() 
{
	return hasIndividualFC;
}

void S100Layer::SetIndividualPC(bool value) 
{
	hasIndividualPC = value;
}
bool S100Layer::GetIndividualPC() 
{
	return hasIndividualPC;
}

void S100Layer::DeleteCatalog()
{
	if (GetIndividualFC())
	{
		delete	featureCatalogue;
	}


	if (GetIndividualPC())
	{
		delete portrayalCatalogue;
	}
}
//
//void S100Layer::DrawInfo(HDC &hDC, Scaler *scaler)
//{
//	if (Info == true)
//	{
//		// HDC -> CDC(GDI) -> Graphics (GDI+)
//		Gdiplus::Graphics graphics(hDC);
//		graphics.SetSmoothingMode(SmoothingModeHighQuality);
//
//		Color internalColor(50, 255, 0, 0);
//		Color lineColor(255, 255, 0, 0);
//
//		SolidBrush internalBrush(internalColor); //내부색상
//		Pen linePen(lineColor); //선
//
//
//
//		// MBR : Minimum bounding rectangle 
//		//auto mbr = selectedLayer->GetMBR();
//		auto mbr = GetMBR();
//		long sxmin = 0;
//		long symax = 0;
//
//		// 내부 좌표계 -> 화면 좌표계
//		scaler->WorldToDevice(mbr.xmin, mbr.ymin, &sxmin, &symax);
//
//		long sxmax = 0;
//		long symin = 0;
//		scaler->WorldToDevice(mbr.xmax, mbr.ymax, &sxmax, &symin);
//
//		RectF position = {
//			(float)sxmin,
//			(float)symin,
//			(float)sxmax - sxmin,
//			(float)symax - symin
//		};
//
//		Gdiplus::Font F(L"Arial", 20, FontStyleRegular, UnitPixel);
//		//PointF P(10.0f, 10.0f);
//		SolidBrush B(Color(0, 0, 255));
//		StringFormat sf;
//
//		CString chatName;
//		chatName.Format(_T("Chart Name : %s \n"), GetChartName().c_str());
//		CString EditionName;
//		EditionName.Format(_T("Edition Number : %s \n"), GetEditionNumberAsWstring().c_str());
//		CString IssueDate;
//		IssueDate.Format(_T("ISSUED DATE : %s \n"), GetIssueDateAsWstring().c_str());
//
//		chatName += EditionName;
//		chatName += IssueDate;
//
//		sf.SetAlignment(StringAlignmentNear);
//		sf.SetLineAlignment(StringAlignmentNear);
//
//		graphics.DrawRectangle(&linePen, position.X, position.Y, position.Width, position.Height);
//		graphics.FillRectangle(&internalBrush, position.X, position.Y, position.Width, position.Height);
//		graphics.DrawString(chatName, -1, &F, position, &sf, &B); //글자쓰기
//
//	}
//}
//


S100SpatialObject* S100Layer::GetS100SpatialObject()
{
	return (S100SpatialObject*)m_spatialObject;
}

int S100Layer::GetProductNumber() {
	if (GetS100SpatialObject() == nullptr)
	{
		return 0;
	}

	return 	GetS100SpatialObject()->GetS100Product();
};