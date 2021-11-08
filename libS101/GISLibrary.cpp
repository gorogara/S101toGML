// GISLibrary.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "GISLibrary.h"
#include "S100ExchangeSetRender.h"
#include "DetectionFunction.h"
#include "S102Layer.h"
#include "S102Cell.h"
#include "S102.h"
#include "S111Layer.h"
#include "S111Cell.h"
#include "ClipperUtil.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/ENCGeometry.h"

#include "../S101Portrayal/PortrayalCatalogue.h"

#include "../S100Geometry/SGeometry.h"

#include "../PLLibrary/SymbolManager.h"

#include "../RoutePlanner/RoutePlanner.h"
#include "../RoutePlanner/Route.h"
#include "../RoutePlanner/BufLeg.h"
#include "../RoutePlanner/RTZ_Route.h"

#include "../DLL_InteroperabilityCatalog/S100_IC_InteroperabilityCatalogue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며 
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//

// CGISLibraryApp
// CGISLibraryApp 생성
CGISLibraryApp::CGISLibraryApp()
{
	m_pLayerManager->scaler = m_pScaler;

	// Load symbol
	ENCGeometry::pSymbolManager = new CSymbolManager();
	ENCGeometry::pSymbolManager->OpenColor(_T("..\\ProgramData\\dic\\day_blackback.dic"));
	ENCGeometry::pSymbolManager->OpenSymbol(_T("..\\ProgramData\\symbol\\symbol.txt"));
	ENCGeometry::pSymbolManager->OpenPattern(_T("..\\ProgramData\\symbol\\Symbol_Pattern.txt"));
	ENCGeometry::pSymbolManager->OpenCompositeSymbol(_T("..\\ProgramData\\symbol\\Composite_Line_Symbol.txt"));
	ENCGeometry::pSymbolManager->OpenComposite(_T("..\\ProgramData\\symbol\\Composite_Line.txt"));
}

CGISLibraryApp::~CGISLibraryApp()
{
	D2.DeleteDeviceDependentResources();
	D2.DeleteDeviceIndependentResources();

	delete m_pScaler;
	m_pScaler = nullptr;

	delete m_pLayerManager;
	m_pLayerManager = nullptr;

	delete s111Engine;
	s111Engine = nullptr;

	delete s413Engine;
	s413Engine = nullptr;

	delete[] SGeometry::viewPoints;
	SGeometry::viewPoints = nullptr;

	delete ENCGeometry::pSymbolManager;
	ENCGeometry::pSymbolManager = nullptr;
}

// 유일한 CGISLibraryApp 개체입니다.
//CGISLibraryApp theApp;
CGISLibraryApp* gisLib = new CGISLibraryApp();

Scaler* CGISLibraryApp::GetScaler()
{
	return m_pScaler;
}

void CGISLibraryApp::InitLibrary()
{
	s111Engine->OpenS100ColorProfile(L"..\\ProgramData\\S111_Portrayal\\ColorProfiles\\colorProfile.xml");
	s111Engine->OpenS100Symbol(L"..\\ProgramData\\S111_Portrayal\\Symbols\\*.svg");

	//s413Engine->OpenS100ColorProfile(L"..\\ProgramData\\S413_Portrayal\\ColorProfiles\\colorProfile.xml");
	//s413Engine->OpenS100Symbol(L"..\\ProgramData\\S413_Portrayal\\Symbols\\*.svg");

	D2.CreateDeviceIndependentResources();

	D2ChartResources.SetChartResources(&gisLib->D2);

	D2.CreateDeviceDependentResources();

	InitS100Engine();

	PC_FCPathSetting();
	BasicFileSetting();
}

bool CGISLibraryApp::AddLayer(CString _filepath)
{
	m_pLayerManager->AddLayer(_filepath);
	return true;
}

bool CGISLibraryApp::AddLayer(CString _filepath, Catalog* fc, PortrayalCatalogue* pc)
{
	m_pLayerManager->AddLayer(_filepath, fc, pc);
	return true;
}

bool CGISLibraryApp::AddLayerFromS100ExchangeSet(CString _filepath, Catalog* fc, PortrayalCatalogue* pc)
{
	return m_pLayerManager->AddLayerFromS100ExchangeSet(_filepath, fc, pc);
}

bool CGISLibraryApp::AddShipMonitoring(CString _filePath)
{
	return m_pLayerManager->AddShipMonitoringLayer(_filePath);
}
bool CGISLibraryApp::AddBackgroundLayer(CString _filepath)
{
	return m_pLayerManager->AddBackgroundLayer(_filepath);
}
bool CGISLibraryApp::AddUserDefinedLayer(CString _filepath)
{
	return m_pLayerManager->AddUserDefinedLayer(_filepath);
}

void CGISLibraryApp::Draw(HDC &hDC, int offset)
{
	m_pLayerManager->Draw(hDC, offset);
	//CDC *pdc = CDC::FromHandle(hDC);
	//DrawRoute(pdc);
	//pdc->DeleteDC();
}

void CGISLibraryApp::Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, double offsetX, double offsetY)
{
	m_pLayerManager->Draw(pRenderTarget, pDXFactory, offsetX, offsetY);
}

void CGISLibraryApp::DrawS100Catalog(HDC &hdc, int offset)
{
	S100ExchangeSetRender::Draw();
}

void CGISLibraryApp::BuildS101Portrayal(Layer* l)
{
	m_pLayerManager->BuildS101Portrayal(l);
}

void CGISLibraryApp::DeleteS101Portrayal(Layer* l)
{
	m_pLayerManager->DeleteS101Portrayal(l);
}
void CGISLibraryApp::S101RebuildPortrayal()
{
	m_pLayerManager->S101RebuildPortrayal();
}

void CGISLibraryApp::DrawValidationLayers(HDC &hDC, int offset)
{
	m_pLayerManager->DrawValidationLayers(hDC, offset);
}
void CGISLibraryApp::DrawOverlay(HDC &hDC, int type, int offset)
{
	m_pLayerManager->DrawOverlay(hDC, type, offset);
}
void CGISLibraryApp::DrawShipMonitoringLayer(HDC &hDC, int offset)
{
	m_pLayerManager->DrawShipMonitoringLayer(hDC, offset);
}
void CGISLibraryApp::GetLayer(int index, Layer *_layer)
{
	m_pLayerManager->GetLayer(index, _layer);
}
Layer* CGISLibraryApp::GetLayer(int index)
{
	return m_pLayerManager->GetLayer(index);
}

void CGISLibraryApp::DrawS100Symbol(int productNumber, std::wstring symbolName, int screenX, int screenY, int rotation, float scale)
{
	auto pc = LayerManager::GetPC(productNumber);
	if (pc)
	{
		auto pcManager = pc->GetS100PCManager();
		if (pcManager)
		{
			auto rt = gisLib->D2.RenderTarget();
			auto brush = gisLib->D2.SolidColorBrush();
			auto stroke = gisLib->D2.SolidStrokeStyle();

			D2D1::Matrix3x2F oldTransform;
			rt->GetTransform(&oldTransform);

			pcManager->Draw(
				symbolName,
				rt,
				brush,
				stroke,
				D2D1::Point2F((FLOAT)screenX, (FLOAT)screenY),
				(FLOAT)rotation,
				scale,
				pc->GetCurrentPaletteName());

			rt->SetTransform(oldTransform);
		}
	}
}

void CGISLibraryApp::DrawScaleBar()
{
	auto scaler = m_pLayerManager->GetScaler();
	auto scale = scaler->GetCurrentScale();
	auto screenHeight = scaler->GetScreenHeight();

	int sx1 = 10;
	int sy1 = screenHeight - 10;

	// 스케일바 시작점 (하단)
	double x1 = 0;
	double y1 = 0;

	scaler->DeviceToWorld(sx1, sy1, &x1, &y1);
	inverseProjection(x1, y1);

	// 스케일바 종료점 (상단)
	double x2 = 0;
	double y2 = 0;

	// scaledistance
	double scalebarDistance = 1852;

	if (scale >= 80000)
	{
		scalebarDistance *= 10;
	}

	LatLonUtility::CalcRhumbLinePosByGeographicLib(x1, y1, 0, scalebarDistance, &x2, &y2);

	projection(x2, y2);

	// 스케일바 종료점 (상단)
	long sx2 = 0;
	long sy2 = 0;

	scaler->WorldToDevice(x2, y2, &sx2, &sy2);

	double originalSymbolHeight = 20; // scalebar 심볼 높이
	double symbolScale = (sy1 - sy2) / originalSymbolHeight;

	if (scale < 80000)
	{
		DrawS100Symbol(101, L"SCALEB10", sx1, sy1, 0, (float)symbolScale);
	}
	else
	{
		DrawS100Symbol(101, L"SCALEB11", sx1, sy1, 0, (float)symbolScale);
	}
}

void CGISLibraryApp::LayerOn(int index)
{
	m_pLayerManager->LayerOn(index);
}
void CGISLibraryApp::LayerInfoOn(int index)
{
	m_pLayerManager->LayerInfoOn(index);
}

void CGISLibraryApp::LayerOff(int index)
{
	m_pLayerManager->LayerOff(index);
}
void CGISLibraryApp::LayerInfoOff(int index)
{
	m_pLayerManager->LayerInfoOff(index);
}

void CGISLibraryApp::LayerUp(int index)
{
	m_pLayerManager->LayerUp(index);
}

void CGISLibraryApp::CheckDataCoverageComputing()
{
	m_pLayerManager->CheckDataCoverageComputing();
}

void CGISLibraryApp::ClearInformationLayer(int index)
{
	m_pLayerManager->ClearInformationLayer(index);
}
void CGISLibraryApp::ClearInformationLayer(CString filepath)
{
	m_pLayerManager->ClearInformationLayer(filepath);
}


void CGISLibraryApp::ClearAllInformationLayer()
{
	m_pLayerManager->ClearAllInformationLayer();
}

void CGISLibraryApp::LayerDown(int index)
{
	m_pLayerManager->LayerDown(index);
}
CString CGISLibraryApp::GetLayerName(int index)
{
	return m_pLayerManager->GetLayerName(index);
}
BOOL CGISLibraryApp::IsOn(int index)
{
	return m_pLayerManager->IsOn(index);
}
void CGISLibraryApp::DeleteLayer(int index)
{
	m_pLayerManager->DeleteLayer(index);

}
void CGISLibraryApp::DeleteLayer(CString filepath)
{
	m_pLayerManager->DeleteLayer(filepath);
}

void CGISLibraryApp::DeleteAllLayer()
{
	m_pLayerManager->DeleteAllLayer();
}

void CGISLibraryApp::ReMBR()
{
	m_pLayerManager->ReMBR();
}

void CGISLibraryApp::SaveLayer(CString filename, CString extend, int index)
{
	m_pLayerManager->SaveLayer(filename, extend, index);
}

void CGISLibraryApp::SaveLayerTextPlacement(CString filename, CString extend, int index)
{
	m_pLayerManager->SaveLayerTextPlacement(filename, extend, index);
}
void CGISLibraryApp::SaveLayer(CString filename, CString extend, Layer *pLayer)
{
	m_pLayerManager->SaveLayer(filename, extend, pLayer);
}

//void CGISLibraryApp::UpdateCS_SafetyDepth()
//{
//	m_pLayerManager->UpdateCS_SafetyDepth();
//}
//void CGISLibraryApp::UpdateCS_SafetyContour()
//{
//	m_pLayerManager->UpdateCS_SafetyContour();
//}
//void CGISLibraryApp::UpdateCS_ShadeType()
//{
//	m_pLayerManager->UpdateCS_ShadeType();
//}
//void CGISLibraryApp::UpdateCS_BoundarySymbol()
//{
//	m_pLayerManager->UpdateCS_BoundarySymbol();
//}
//void CGISLibraryApp::ChangeDisplayMode()
//{
//	m_pLayerManager->ChangeDisplayMode();
//}


void CGISLibraryApp::UpdateS102ColorMapType(int _type, std::vector<COLORREF> _colorMap)
{
	m_pLayerManager->UpdateS102ColorMapType(_type, _colorMap);
}
void CGISLibraryApp::GetS102ColorMapType(int& _type, std::vector<COLORREF>& _colorMap)
{
	m_pLayerManager->GetS102ColorMapType(_type, _colorMap);
}


void CGISLibraryApp::SetViewMBR(RECT r)
{
	m_pLayerManager->SetViewMBR(r);
}

int CGISLibraryApp::GetLayerCount()
{
	return m_pLayerManager->GetLayerCount();
}

std::vector<Layer*> CGISLibraryApp::GetAllLayer()
{
	return m_pLayerManager->m_listLayer;
}

MBR* CGISLibraryApp::GetMBR()
{
	return &m_pLayerManager->mbr;
}

void CGISLibraryApp::SetColorTable(std::wstring _path)
{
	m_pLayerManager->m_color.ChangeColorTable(_path.c_str());
}

/*
** 좌표 변환
*/
void CGISLibraryApp::DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate)
{
	m_pScaler->DeviceToWorld(sx, sy, mx, my, rotate);
}

void CGISLibraryApp::WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate)
{
	m_pScaler->WorldToDevice(mx, my, sx, sy, rotate);
}

void CGISLibraryApp::WorldToDevice(D2D1_POINT_2F& p)
{
	long sx = 0;
	long sy = 0;
	m_pScaler->WorldToDevice(p.x, p.y, &sx, &sy);

	p.x = sx;
	p.y = sy;
}

void CGISLibraryApp::DeviceToWorld(D2D1_POINT_2F& p)
{
	double mx = 0;
	double my = 0;
	m_pScaler->DeviceToWorld(p.x, p.y, &mx, &my);

	p.x = mx;
	p.y = my;
}

/*
** 거리 계산
*/
//double CGISLibraryApp::GetDistanceLatitudeLongitude(double longitude1, double latitude1, double longitude2, double latitude2)
//{
//	return m_pScaler->GetDistanceLatitudeLongitude(longitude1, latitude1, longitude2, latitude2);
//}
double CGISLibraryApp::GetDistanceScreen(int x1, int y1, int x2, int y2)
{
	return m_pScaler->GetDistanceScreen(x1, y1, x2, y2);
}

/*
** 화면 설정
*/
void CGISLibraryApp::SetScreen(RECT *rect)
{
	if (rect == nullptr) return;
	m_pScaler->SetScreen(rect);
}

void CGISLibraryApp::SetScreen(const RECT& rect)
{
	m_pScaler->SetScreen(rect);
}

int CGISLibraryApp::GetCenterXScreen()
{
	return (int)m_pScaler->sox;
}

int CGISLibraryApp::GetCenterYScreen()
{
	return (int)m_pScaler->soy;
}

/*
** 화면에 보여지는 Map의 정보
*/
void CGISLibraryApp::SetMap(double xmin, double ymin, double xmax, double ymax)
{
	m_pScaler->SetMap(xmin, ymin, xmax, ymax);
}

void CGISLibraryApp::SetMap(MBR mbr)
{
	m_pScaler->SetMap(mbr);
}

double CGISLibraryApp::GetMapWidth()
{
	return m_pScaler->GetMapWidth();
}
double CGISLibraryApp::GetMapHeight()
{
	return m_pScaler->GetMapHeight();
}

void CGISLibraryApp::GetMap(MBR *mbr)
{
	m_pScaler->GetMap(mbr);
}

/*
** Scale 정보
*/
int CGISLibraryApp::GetCurrentScale()
{
	return m_pScaler->GetCurrentScale();
}
void   CGISLibraryApp::SetScale(int scale)
{
	m_pScaler->SetScale(scale);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}
void   CGISLibraryApp::UpdateScale()
{
	m_pScaler->UpdateScale();

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}

/*
** 확대 & 축소
** 인  자 : %단위
** 사용예 : 20%확대하고 싶을땐 인자에 "20" 삽입
*/
void CGISLibraryApp::ZoomIn(double value)
{
	m_pScaler->ZoomIn(value);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}
void CGISLibraryApp::ZoomIn(double value, int x, int y)
{
	m_pScaler->ZoomIn(value, x, y);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}
void CGISLibraryApp::ZoomOut(double value)
{
	m_pScaler->ZoomOut(value);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();

#endif
}
void CGISLibraryApp::ZoomOut(double value, int x, int y)
{
	m_pScaler->ZoomOut(value, x, y);

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}
void CGISLibraryApp::AdjustScreenMap()
{
	m_pScaler->AdjustScreenMap();

#ifdef PORTRAYAL_RULE_TEST 
	ENCCommon::T_CURRENT_SCALE = (int)m_pScaler->GetCurrentScale();
	m_pLayerManager->S101RebuildPortrayal();
#endif
}

/*
** 화면 이동
*/
// 원하는 픽셀만큼 움직일때 사용(미완성)
void CGISLibraryApp::MoveMap(int x, int y)
{
	m_pScaler->MoveMap(x, y);
}

// 원하는 위치(sy, sy)에 원하는 점을(mx, my)를 위치 시킬때 사용
void CGISLibraryApp::MoveMap(int sx, int sy, double mx, double my)
{
	m_pScaler->MoveMap(sx, sy, mx, my);
}

//void CGISLibraryApp::Projection(double *x, double *y)
//{
//	m_pScaler->Projection(x, y);
//}
//void CGISLibraryApp::InverseProjection(double *x, double *y)
//{
//	m_pScaler->InverseProjection(x, y);
//}

/*
** 화면 회전
*/
void CGISLibraryApp::Rotate(LONG *sx, LONG *sy, double degree)
{
	m_pScaler->Rotate(sx, sy, degree);
}
void CGISLibraryApp::RotateMap(double degree)
{
	m_pScaler->RotateMap(degree);
}	// rotateDegree의 범위를 [0, 360) 으로 유지
void CGISLibraryApp::NorthUp()
{
	m_pScaler->NorthUp();
}

bool CGISLibraryApp::PtInMap(double _x, double _y)
{
	return m_pScaler->PtInMap(_x, _y);
}


//ClipperLib::PolyTree* CGISLibraryApp::ClipPolyline(GeoPoint* _p, int _cnt)
//{
//	return m_pScaler->ClipPolyline(_p, _cnt);
//}


int CGISLibraryApp::GetLayerIndex(CString pathName)
{
	return m_pLayerManager->GetLayerIndex(pathName);
}


void CGISLibraryApp::NextTimeIndex()
{
	m_pLayerManager->NextTimeIndex();
}


void CGISLibraryApp::PrevTimeIndex()
{
	m_pLayerManager->PrevTimeIndex();
}


void CGISLibraryApp::InitS100Engine()
{
	// View의 Direct2D 자원을 SVGManager로 넘기는 부분
	auto svgManager = S100_SVG_D2D1_DLL::GetApp()->GetSVGManager();
	if (svgManager)
	{
		svgManager->SetD2Resource(gisLib->D2.pD2Factory, gisLib->D2.pRT, gisLib->D2.pBrush, gisLib->D2.D2D1StrokeStyleGroup[0]);
	}

	if (s111Engine)
	{
		s111Engine->CreateSVGGeometry(gisLib->D2.Factory());
	}
}

LayerManager* CGISLibraryApp::GetLayerManager()
{
	return m_pLayerManager;
}


void CGISLibraryApp::ChangeDisplayFont()
{
	SafeRelease(&D2.pDWriteTextFormat);
	SafeRelease(&D2.pDWriteTextFormatArea);

	// Create a DirectWrite text format object.
	HRESULT hr = D2.pDWriteFactory->CreateTextFormat(
		ENCCommon::DISPLAY_FONT_NAME.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)ENCCommon::DISPLAY_FONT_SIZE,
		L"", //locale
		&D2.pDWriteTextFormat
	);

	D2.pDWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	if (!D2.pDWriteTextFormat)
	{
		CString errMsg;
		errMsg.Format(_T("Failed to create IDWriteTextFormat! (%02X)"), hr);
		OutputDebugString(errMsg);
	}

	auto fsfsf = ENCCommon::DISPLAY_FONT_NAME.c_str();

	// Create a DirectWrite text format object.
	hr = D2.pDWriteFactory->CreateTextFormat(
		ENCCommon::DISPLAY_FONT_NAME.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)ENCCommon::DISPLAY_FONT_SIZE,
		L"", //locale
		&D2.pDWriteTextFormatArea
	);

	D2.pDWriteTextFormatArea->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	D2.pDWriteTextFormatArea->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	D2.pDWriteTextFormatArea->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	if (!D2.pDWriteTextFormatArea)
	{
		CString errMsg;
		errMsg.Format(_T("Failed to create IDWriteTextFormat (for Area)! (%02X)"), hr);
		OutputDebugString(errMsg);
	}
}

void CGISLibraryApp::SetDrawBackground(bool on)
{
	m_pLayerManager->m_baseMapOn = on;
}
bool CGISLibraryApp::GetDrawBackground()
{
	return m_pLayerManager->m_baseMapOn;
}

void CGISLibraryApp::SetExPCPath(CString value)
{
	m_pLayerManager->SetExPCPath(value);
}

CString CGISLibraryApp::GetExPCPath()
{
	return m_pLayerManager->GetExPCPath();
}

void CGISLibraryApp::SetExFCPath(CString value)
{
	m_pLayerManager->SetExFCPath(value);
}

CString CGISLibraryApp::GetExFCPath()
{
	return m_pLayerManager->GetExFCPath();
}

void CGISLibraryApp::PC_FCPathSetting()
{
	CStdioFile file;
	if (file.Open(_T("..\\ProgramData\\data\\BasicFilePath.txt"), CFile::modeRead))
	{
		CString FCstart;
		file.ReadString(FCstart);
		std::vector<CString> content;
		try
		{
			if (FCstart.Compare(L"PC/FC") == 0)
			{
				CString value;
				while (file.ReadString(value))
				{
					content.push_back(value);
				}

				for (unsigned i = 0; i < content.size(); i++)
				{
					if (i % 2 == 0)
					{
						BasicFile basicfilepath;
						basicfilepath.Set_BasicFile(content[i], content[i + 1]);
						BasicFilePath.push_back(basicfilepath);
					}
				}
			}
			file.Close();
		}
		catch (std::exception e)
		{
			AfxMessageBox(L"오류발생");
		}
	}
	else //파일오픈에 실패할경우 발생합니다.
	{
		BasicFile S_101;
		S_101.Set_BasicFile(L"..\\ProgramData\\xml\\S-101_FC.xml"
			, L"..\\ProgramData\\S101_Portrayal\\");
		BasicFilePath.push_back(S_101);

		//==================================================================================

		BasicFile S_102;
		S_102.Set_BasicFile(L"..\\ProgramData\\xml\\S-102 FC.xml"
			, L"..\\ProgramData\\S102_Portrayal\\");
		BasicFilePath.push_back(S_102);

		//==================================================================================

		BasicFile S_111;
		S_111.Set_BasicFile(L"..\\ProgramData\\xml\\S-111 FC.xml"
			, L"..\\ProgramData\\S111_Portrayal\\");
		BasicFilePath.push_back(S_111);

		//==================================================================================

		BasicFile S_122;
		S_122.Set_BasicFile(L"..\\ProgramData\\xml\\S-122_FC.xml"
			, L"..\\ProgramData\\S122_Portrayal\\");
		BasicFilePath.push_back(S_122);

		//==================================================================================

		BasicFile S_123;
		S_123.Set_BasicFile(L"..\\ProgramData\\xml\\S-123_FC.xml"
			, L"..\\ProgramData\\S123_Portrayal\\");
		BasicFilePath.push_back(S_123);

		//==================================================================================

		BasicFile S_124;
		S_124.Set_BasicFile(L"..\\ProgramData\\xml\\S-124_FC.xml"
			, L"..\\ProgramData\\S124_Portrayal\\");
		BasicFilePath.push_back(S_124);

		//==================================================================================

		BasicFile S_126;
		S_126.Set_BasicFile(L"..\\ProgramData\\xml\\S-126 Physical Environment_0.0.1.xml"
			, L"..\\ProgramData\\S126_Portrayal\\");
		BasicFilePath.push_back(S_126);

		//==================================================================================

		BasicFile S_127;
		S_127.Set_BasicFile(L"..\\ProgramData\\xml\\S-127_FC.xml"
			, L"..\\ProgramData\\S127_Portrayal\\");
		BasicFilePath.push_back(S_127);

		//==================================================================================

		BasicFile S_128;
		S_128.Set_BasicFile(L"..\\ProgramData\\xml\\Feature Catalogue for S-128 ENCs_0.1.4.xml"
			, L"..\\ProgramData\\S128_Portrayal\\");
		BasicFilePath.push_back(S_128);

		//==================================================================================

		BasicFile S_129;
		S_129.Set_BasicFile(L"..\\ProgramData\\xml\\S-129_FC.xml"
			, L"..\\ProgramData\\S129_Portrayal\\");
		BasicFilePath.push_back(S_129);

		//==================================================================================

		BasicFile S_412;
		S_412.Set_BasicFile(L"..\\ProgramData\\xml\\S-412_FC.xml"
			, L"..\\ProgramData\\S412_Portrayal\\");
		BasicFilePath.push_back(S_412);

		//==================================================================================

		BasicFile S_413;
		S_413.Set_BasicFile(L"..\\ProgramData\\xml\\S-413_Draft.xml"
			, L"..\\ProgramData\\S413_Portrayal\\");
		BasicFilePath.push_back(S_413);

		//==================================================================================

		BasicFile S_421;
		S_421.Set_BasicFile(L"..\\ProgramData\\xml\\S-421 Route information FC(0.1).xml"
			, L"..\\ProgramData\\S421_Portrayal\\");
		BasicFilePath.push_back(S_421);
	}
}


void CGISLibraryApp::BasicFileSetting()
{
	// FC/PC 로딩
	if (!LayerManager::pPortrayalCatalogues) //여기가 S101 같은걸 추가하는 부분
	{
		LayerManager::pS100Catalogs = new std::vector<Catalog*>();
		LayerManager::pPortrayalCatalogues = new std::vector<PortrayalCatalogue*>();

		LayerManager::hash_FC = new std::unordered_map<std::wstring, Catalog*>();
		LayerManager::hash_PC = new std::unordered_map<std::wstring, PortrayalCatalogue*>();

		for (unsigned i = 0; i < BasicFilePath.size(); i++)
		{
			//피쳐 카탈로그 읽기 시작 
			//색상 변경 추가
			auto fc = LayerManager::AddS100FC(BasicFilePath[i].Get_FcPath());
			if (GetColorTable() != L"")
			{
				auto pc = LayerManager::AddS100PC(fc, BasicFilePath[i].Get_PortrayalPath(), GetColorTable());
				pc->CreatePatternImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
				pc->CreateLineImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
			}
			else
			{
				auto pc = LayerManager::AddS100PC(fc, BasicFilePath[i].Get_PortrayalPath());
				pc->CreatePatternImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
				pc->CreateLineImages(gisLib->D2.pD2Factory, gisLib->D2.pImagingFactory, gisLib->D2.D2D1StrokeStyleGroup.at(0));
			}

		}
	}

	LayerManager::LoadUserDefined_FC_PC();
}

std::wstring CGISLibraryApp::GetColorTable()
{
	if (ENCCommon::m_eColorTable == GeoMetryLibrary ::ColorTable::Day)
	{
		return L"Day";
	}
	else if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Dusk)
	{
		return L"Dusk";
	}
	else if (ENCCommon::m_eColorTable == GeoMetryLibrary::ColorTable::Night)
	{
		return L"Night";
	}
	else
	{
		return L"";
	}
}

bool CGISLibraryApp::PickReport_S102(double inMx, double inMy, double& outMx, double& outMy, float& depth)
{
	auto lm = GetLayerManager();
	auto s102Layer = (S102Layer*)lm->GetS100Layer(102, inMx, inMy);

	if (nullptr != s102Layer)
	{
		auto s102Cell = s102Layer->GetS102Cell();
		if (nullptr != s102Cell)
		{
			s102Cell->ReOpen();

			auto s102 = s102Cell->GetS102();
			if (s102)
			{
				inverseProjection(inMx, inMy);
				auto result = s102->PickReport(inMx, inMy, 0, "BathymetryCoverage");

				outMx = result.GetX();
				outMy = result.GetY();
				inverseProjection(outMx, outMy);

				depth = result.GetValue(0);
				if (depth > 1000000 - 0.1)
				{
					return false;
				}

				//s102Cell->Close();

				return true;
			}
		}
	}

	return false;
}

bool CGISLibraryApp::PickReport_S111(double inMx, double inMy, double& outMx, double& outMy, float& direction, float& speed)
{
	auto lm = GetLayerManager();
	auto layer = (S111Layer*)lm->GetS100Layer(111, inMx, inMy);

	if (nullptr != layer)
	{
		auto cell = (S111Cell*)layer->GetS100SpatialObject();
		if (nullptr != cell)
		{
			double dir = 0;
			double spd = 0;
			auto result = cell->PickReport(inMx, inMy, outMx, outMy, dir, spd);
			direction = dir;
			speed = spd;

			return result;
		}
	}

	return false;
}

bool CGISLibraryApp::PickReport_S111(double inMx, double inMy, std::vector<double>& direction, std::vector<double>& speed)
{
	auto lm = GetLayerManager();
	auto layer = (S111Layer*)lm->GetS100Layer(111, inMx, inMy);

	if (nullptr != layer)
	{
		auto cell = (S111Cell*)layer->GetS100SpatialObject();
		if (nullptr != cell)
		{
			cell->PickReport(inMx, inMy, direction, speed);

			return true;
		}
	}

	return false;
}

void CGISLibraryApp::SetOwnShipPosition(double lon, double lat)
{
	m_pLayerManager->SetOwnShipPosition(lon, lat);
}

void CGISLibraryApp::SetOwnShipSize(double length, double width)
{
	m_pLayerManager->SetOwnShipSize(length, width);
}

void CGISLibraryApp::MakeOwnShipPosition()
{
#ifdef ROUTE_CHECK
	// 시간 단위로 끊어서 새로운 항로 tempRoute 생성
	int currentRouteIndex = 0;

	RT::Route* currentRoute = pRoutePlanner->GetRoute(currentRouteIndex);
	if (currentRoute)
	{
		int wpCnt = currentRoute->GetWaypointCount();
		if (wpCnt < 2)
		{
			return;
		}
	}
#endif
}

void CGISLibraryApp::DrawRoute(CDC* pDC)
{
#ifdef ROUTE_CHECK
	MakeOwnShipPosition();
#endif
	// 그려야할 Route의 인덱스
	int routeIndex = pRoutePlanner->GetSelectedRouteIndex();

	if (nullptr == pRoutePlanner->GetRoute(routeIndex))
	{
		return;
	}

	// 그려야할 Route의 Waypoint 개수
	int waypointCount = pRoutePlanner->GetWaypointCount(routeIndex);

	if (waypointCount < 1) return;

	GeoPoint* ptWp = new GeoPoint[waypointCount];

	for (int i = 0; i < waypointCount; i++) {
		ptWp[i].x = pRoutePlanner->GetX(routeIndex, i);
		ptWp[i].y = pRoutePlanner->GetY(routeIndex, i);
	}

	ClipperLib::PolyTree* clippedPLs = ClipperUtil::ClipPolyline(ptWp, waypointCount, gisLib->GetScaler());
	ClipperLib::Paths paths;
	ClipperLib::PolyTreeToPaths(*clippedPLs, paths);

	Symbol* symbol = ENCGeometry::pSymbolManager->GetComposite(_T("PLNRTE04"));

	auto scaler = gisLib->GetScaler();

	// Draw Line
	for (unsigned i = 0; i < paths.size(); i++) {
		ClipperLib::Path path = paths.at(i);

		POINT* tmpPtWp = new POINT[path.size()];

		for (unsigned j = 0; j < path.size(); j++) {
			gisLib->WorldToDevice(
				(double)path.at(j).X / scaler->GetCOMF(),
				(double)path.at(j).Y / scaler->GetCOMF(),
				&tmpPtWp[j].x,
				&tmpPtWp[j].y);
		}

		GeoPolyline line;
		line.DrawCompositeLineOnPolyline(pDC, tmpPtWp, (int)path.size(), symbol);

		delete[] tmpPtWp;
	}

	if (waypointCount >= 1) {
		delete[] ptWp;
	}

	delete clippedPLs;

	// Draw Waypoint
	POINT tmpP;
	for (int i = 0; i < waypointCount; i++)
	{
		double x = pRoutePlanner->GetX(routeIndex, i);
		double y = pRoutePlanner->GetY(routeIndex, i);

		if (gisLib->PtInMap(x, y))
		{
			gisLib->WorldToDevice(x, y, &(tmpP.x), &(tmpP.y));

			ENCGeometry::pSymbolManager->DrawSymbol(_T("WAYPNT02"), pDC, CPoint(tmpP.x, tmpP.y));
		}
	}

	// Draw Route Buffer
	DrawRouteBuffer_D2D1(pDC, routeIndex);
	//DrawRouteBuffer(pDC, routeIndex, isMoveDraw);
}

void CGISLibraryApp::DrawRouteBuffer_D2D1(CDC* pDC, int _routeIndex)
{
	auto scaler = gisLib->GetScaler();

	gisLib->D2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	if (_routeIndex < 0) {
		return;
	}

	// 그려야할 Route의 Waypoint 개수
	int waypointCount = pRoutePlanner->GetWaypointCount(_routeIndex);

	if (waypointCount < 2) {
		return;
	}

	// Waypoint를 가져옴, WGS84(Degree단위)로 변환
	GeoPoint* ptWp = new GeoPoint[waypointCount];

	for (int i = 0; i < waypointCount; i++) {
		ptWp[i].x = pRoutePlanner->GetX(_routeIndex, i);
		ptWp[i].y = pRoutePlanner->GetY(_routeIndex, i);
		inverseProjection(ptWp[i].x, ptWp[i].y);
	}

	auto centerBufLeg = new RT::BufLeg[waypointCount - 1];
	auto rightBufLeg = new RT::BufLeg[waypointCount - 1];
	auto leftBufLeg = new RT::BufLeg[waypointCount - 1];

	bool* isRight = nullptr;
	if (waypointCount >= 3) {
		isRight = new bool[waypointCount - 2];
	}

	double* startOffset = new double[waypointCount - 1];
	double* endOffset = new double[waypointCount - 1];

	for (int i = 0; i < (waypointCount - 1); i++) {
		startOffset[i] = 0;
		endOffset[i] = 0;
	}
	
	
	//double buffDistance = 0.4;
	double buffDistance = pRoutePlanner->GetBufferDistance(_routeIndex) * 2700;

	HRESULT hr = S_OK;
	HDC hdc = pDC->GetSafeHdc();
	gisLib->D2.Begin(hdc);
	ID2D1PathGeometry* tempGeometry = nullptr;
	ID2D1PathGeometry* pathRouteBufferArcPart = nullptr;
	ID2D1PathGeometry* pathRouteBuffer = nullptr;
	ID2D1PathGeometry* pathRouteBufferPart = nullptr;


	ID2D1PathGeometry* pathRouteBufferCenter = nullptr;
	ID2D1PathGeometry* pathRouteBufferLeft = nullptr;
	ID2D1PathGeometry* pathRouteBufferRight = nullptr;

	ID2D1GeometrySink* pGeometrySinkRouteBufferCenter = nullptr;
	ID2D1GeometrySink* pGeometrySinkRouteBufferLeft = nullptr;
	ID2D1GeometrySink* pGeometrySinkRouteBufferRight = nullptr;

	hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pathRouteBufferCenter);
	if (SUCCEEDED(hr))
	{
		hr = pathRouteBufferCenter->Open(&pGeometrySinkRouteBufferCenter);
		pGeometrySinkRouteBufferCenter->SetFillMode(D2D1_FILL_MODE_WINDING);
	}
	hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pathRouteBufferLeft);
	if (SUCCEEDED(hr))
	{
		hr = pathRouteBufferLeft->Open(&pGeometrySinkRouteBufferLeft);
		pGeometrySinkRouteBufferLeft->SetFillMode(D2D1_FILL_MODE_WINDING);
	}
	hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pathRouteBufferRight);
	if (SUCCEEDED(hr))
	{
		hr = pathRouteBufferRight->Open(&pGeometrySinkRouteBufferRight);
		pGeometrySinkRouteBufferRight->SetFillMode(D2D1_FILL_MODE_WINDING);
	}

	gisLib->D2.pBrush->SetColor(D2D1::ColorF((FLOAT)(255 / 255.0), (FLOAT)(100 / 255.0), (FLOAT)(100 / 255.0)));
	gisLib->D2.pBrush->SetOpacity(float(0.7));
	gisLib->D2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pathRouteBuffer);

	D2D1_POINT_2F ptCircle;
	D2D1_POINT_2F ptCircleStart;
	D2D1_POINT_2F ptCircleEnd;
	D2D1_POINT_2F ptInnerCircleStart;
	D2D1_POINT_2F ptInnerCircleEnd;
	D2D1_POINT_2F ptAngularPoint;

	for (int i = 0; i < waypointCount - 1; i++)
	{
		if ((i == 0) && (i == waypointCount - 2)) {
			// Simple(start)
			centerBufLeg[i].ptStart = SimpleLeftBufPoint(ptWp[i].x, ptWp[i].y, pRoutePlanner->GetHeading(_routeIndex, i), 0, false);
			projection(centerBufLeg[i].ptStart.x, centerBufLeg[i].ptStart.y);
			leftBufLeg[i].ptStart = SimpleLeftBufPoint(ptWp[i].x, ptWp[i].y, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(leftBufLeg[i].ptStart.x, leftBufLeg[i].ptStart.y);
			rightBufLeg[i].ptStart = SimpleRightBufPoint(ptWp[i].x, ptWp[i].y, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(rightBufLeg[i].ptStart.x, rightBufLeg[i].ptStart.y);

			// Simple(end)
			centerBufLeg[i].ptEnd = SimpleLeftBufPoint(ptWp[i + 1].x, ptWp[i + 1].y, pRoutePlanner->GetHeading(_routeIndex, i), 0, false);
			projection(centerBufLeg[i].ptEnd.x, centerBufLeg[i].ptEnd.y);
			leftBufLeg[i].ptEnd = SimpleLeftBufPoint(ptWp[i + 1].x, ptWp[i + 1].y, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(leftBufLeg[i].ptEnd.x, leftBufLeg[i].ptEnd.y);
			rightBufLeg[i].ptEnd = SimpleRightBufPoint(ptWp[i + 1].x, ptWp[i + 1].y, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(rightBufLeg[i].ptEnd.x, rightBufLeg[i].ptEnd.y);
		}
		else if (i == 0) {
			// Simple(start)
			centerBufLeg[i].ptStart = SimpleLeftBufPoint(ptWp[i].x, ptWp[i].y, pRoutePlanner->GetHeading(_routeIndex, i), 0, false);
			projection(centerBufLeg[i].ptStart.x, centerBufLeg[i].ptStart.y);
			leftBufLeg[i].ptStart = SimpleLeftBufPoint(ptWp[i].x, ptWp[i].y, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(leftBufLeg[i].ptStart.x, leftBufLeg[i].ptStart.y);
			rightBufLeg[i].ptStart = SimpleRightBufPoint(ptWp[i].x, ptWp[i].y, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(rightBufLeg[i].ptStart.x, rightBufLeg[i].ptStart.y);

			// Complex(end)
			double currentHeading = pRoutePlanner->GetHeading(_routeIndex, i);
			double nextHeading = pRoutePlanner->GetHeading(_routeIndex, i + 1);

			double theta = GetAngleFromLegs(currentHeading, nextHeading, false) * DEG2RAD;

			double offset = 0;

			if (theta != M_PI) {
				offset = buffDistance / tan(theta / 2.0);
			}

			LatLonUtility::Pos pos = LatLonUtility::GetDestinationPointRhumbLineMercator(ptWp[i + 1].y, ptWp[i + 1].x, pRoutePlanner->GetHeading(_routeIndex, i) + 180, offset, false);
			centerBufLeg[i].ptEnd = SimpleLeftBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), 0, false);
			projection(centerBufLeg[i].ptEnd.x, centerBufLeg[i].ptEnd.y);
			leftBufLeg[i].ptEnd = SimpleLeftBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(leftBufLeg[i].ptEnd.x, leftBufLeg[i].ptEnd.y);
			rightBufLeg[i].ptEnd = SimpleRightBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(rightBufLeg[i].ptEnd.x, rightBufLeg[i].ptEnd.y);

			endOffset[i] = LatLonUtility::GetDistanceRhumbLineMercator(pos.lat, pos.lon, ptWp[i + 1].y, ptWp[i + 1].x, false);
		}
		else if (i == waypointCount - 2) {
			// Complex(start)
			double prevHeading = pRoutePlanner->GetHeading(_routeIndex, i - 1);
			double currentHeading = pRoutePlanner->GetHeading(_routeIndex, i);

			double angle = GetAngleFromLegs(prevHeading, currentHeading, false);
			double theta = angle * DEG2RAD;

			double offset = 0;

			if (theta != M_PI) {
				offset = buffDistance / tan(theta / 2.0);
			}

			LatLonUtility::Pos pos = LatLonUtility::GetDestinationPointRhumbLineMercator(ptWp[i].y, ptWp[i].x, pRoutePlanner->GetHeading(_routeIndex, i), offset, false);
			centerBufLeg[i].ptStart = SimpleLeftBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), 0, false);
			projection(centerBufLeg[i].ptStart.x, centerBufLeg[i].ptStart.y);
			leftBufLeg[i].ptStart = SimpleLeftBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(leftBufLeg[i].ptStart.x, leftBufLeg[i].ptStart.y);
			rightBufLeg[i].ptStart = SimpleRightBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(rightBufLeg[i].ptStart.x, rightBufLeg[i].ptStart.y);

			startOffset[i] = LatLonUtility::GetDistanceRhumbLineMercator(pos.lat, pos.lon, ptWp[i].y, ptWp[i].x, false);

			D2D1_POINT_2F ptOffset;
			projection(pos.lon, pos.lat);

			ptOffset.x = pos.lon;
			ptOffset.y = pos.lat;
			WorldToDevice(ptOffset);

			if (waypointCount >= 3) {
				isRight[i - 1] = IsRight(ptWp[i - 1].x, ptWp[i - 1].y, ptWp[i].x, ptWp[i].y,
					ptWp[i].x, ptWp[i].y, ptWp[i + 1].x, ptWp[i + 1].y);


				if (isRight[i - 1]) { // 오른쪽 
					ptCircle.x = rightBufLeg[i].ptStart.x;
					ptCircle.y = rightBufLeg[i].ptStart.y;
					WorldToDevice(ptCircle);

					double bufDistancePixel = sqrt((ptCircle.x - ptOffset.x) * (ptCircle.x - ptOffset.x) + (ptCircle.y - ptOffset.y) * (ptCircle.y - ptOffset.y));

					ptInnerCircleStart.x = centerBufLeg[i - 1].ptEnd.x;
					ptInnerCircleStart.y = centerBufLeg[i - 1].ptEnd.y;

					ptInnerCircleEnd.x = centerBufLeg[i].ptStart.x;
					ptInnerCircleEnd.y = centerBufLeg[i].ptStart.y;

					ptCircleStart.x = leftBufLeg[i - 1].ptEnd.x;
					ptCircleStart.y = leftBufLeg[i - 1].ptEnd.y;

					ptCircleEnd.x = leftBufLeg[i].ptStart.x;
					ptCircleEnd.y = leftBufLeg[i].ptStart.y;

					ptAngularPoint.x = rightBufLeg[i].ptStart.x;
					ptAngularPoint.y = rightBufLeg[i].ptStart.y;

					WorldToDevice(ptInnerCircleEnd);

					pGeometrySinkRouteBufferCenter->AddArc(
						D2D1::ArcSegment(
							ptInnerCircleEnd,//end point
							D2D1::SizeF(bufDistancePixel, bufDistancePixel),
							angle,//rotation angle
							D2D1_SWEEP_DIRECTION_CLOCKWISE,
							D2D1_ARC_SIZE_SMALL
						));

					WorldToDevice(ptCircleEnd);

					pGeometrySinkRouteBufferRight->AddArc(
						D2D1::ArcSegment(
							ptCircleEnd,//end point
							D2D1::SizeF(bufDistancePixel * 2, bufDistancePixel * 2),
							angle,//rotation angle
							D2D1_SWEEP_DIRECTION_CLOCKWISE,
							D2D1_ARC_SIZE_SMALL
						));

					// arc area part
					{
						HRESULT hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pathRouteBufferArcPart);

						if (SUCCEEDED(hr))
						{
							ID2D1GeometrySink* pGeometrySink = NULL;
							hr = pathRouteBufferArcPart->Open(&pGeometrySink);

							pGeometrySink->SetFillMode(D2D1_FILL_MODE_WINDING);

							WorldToDevice(ptCircleStart);

							pGeometrySink->BeginFigure(
								ptCircleStart,
								D2D1_FIGURE_BEGIN_FILLED
							);

							WorldToDevice(ptAngularPoint);
							//WorldToDevice(ptCircleEnd);

							pGeometrySink->AddLine(ptAngularPoint);
							pGeometrySink->AddLine(ptCircleEnd);
							pGeometrySink->AddArc(
								D2D1::ArcSegment(
									ptCircleStart,//end point
									D2D1::SizeF(bufDistancePixel * 2, bufDistancePixel * 2),
									angle,//rotation angle
									D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
									D2D1_ARC_SIZE_SMALL
								));

							pGeometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);

							hr = pGeometrySink->Close();
							SafeRelease(&pGeometrySink);
						}
					}
				}
				else { // 왼쪽
					ptCircle.x = leftBufLeg[i].ptStart.x;
					ptCircle.y = leftBufLeg[i].ptStart.y;
					WorldToDevice(ptCircle);

					double bufDistancePixel = sqrt((ptCircle.x - ptOffset.x) * (ptCircle.x - ptOffset.x) + (ptCircle.y - ptOffset.y) * (ptCircle.y - ptOffset.y));

					ptInnerCircleStart.x = centerBufLeg[i - 1].ptEnd.x;
					ptInnerCircleStart.y = centerBufLeg[i - 1].ptEnd.y;

					ptInnerCircleEnd.x = centerBufLeg[i].ptStart.x;
					ptInnerCircleEnd.y = centerBufLeg[i].ptStart.y;

					ptCircleStart.x = rightBufLeg[i - 1].ptEnd.x;
					ptCircleStart.y = rightBufLeg[i - 1].ptEnd.y;

					ptCircleEnd.x = rightBufLeg[i].ptStart.x;
					ptCircleEnd.y = rightBufLeg[i].ptStart.y;

					ptAngularPoint.x = leftBufLeg[i].ptStart.x;
					ptAngularPoint.y = leftBufLeg[i].ptStart.y;

					WorldToDevice(ptInnerCircleEnd);

					pGeometrySinkRouteBufferCenter->AddArc(
						D2D1::ArcSegment(
							ptInnerCircleEnd,//end point
							D2D1::SizeF(bufDistancePixel, bufDistancePixel),
							angle,//rotation angle
							D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
							D2D1_ARC_SIZE_SMALL
						));

					WorldToDevice(ptCircleEnd);

					pGeometrySinkRouteBufferLeft->AddArc(
						D2D1::ArcSegment(
							ptCircleEnd,//end point
							D2D1::SizeF(bufDistancePixel * 2, bufDistancePixel * 2),
							angle,//rotation angle
							D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
							D2D1_ARC_SIZE_SMALL
						));

					// arc area part
					{
						HRESULT hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pathRouteBufferArcPart);

						if (SUCCEEDED(hr))
						{
							ID2D1GeometrySink* pGeometrySink = NULL;
							hr = pathRouteBufferArcPart->Open(&pGeometrySink);

							pGeometrySink->SetFillMode(D2D1_FILL_MODE_WINDING);

							WorldToDevice(ptCircleStart);

							pGeometrySink->BeginFigure(
								ptCircleStart,
								D2D1_FIGURE_BEGIN_FILLED
							);

							WorldToDevice(ptAngularPoint);
							//WorldToDevice(ptCircleEnd);

							pGeometrySink->AddLine(ptAngularPoint);
							pGeometrySink->AddLine(ptCircleEnd);
							pGeometrySink->AddArc(
								D2D1::ArcSegment(
									ptCircleStart,//end point
									D2D1::SizeF(bufDistancePixel * 2, bufDistancePixel * 2),
									angle,//rotation angle
									D2D1_SWEEP_DIRECTION_CLOCKWISE,
									D2D1_ARC_SIZE_SMALL
								));

							pGeometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);

							hr = pGeometrySink->Close();
							SafeRelease(&pGeometrySink);
						}
					}
				}
			}

			// Simple(end)
			centerBufLeg[i].ptEnd = SimpleLeftBufPoint(ptWp[i + 1].x, ptWp[i + 1].y, pRoutePlanner->GetHeading(_routeIndex, i), 0, false);
			projection(centerBufLeg[i].ptEnd.x, centerBufLeg[i].ptEnd.y);
			leftBufLeg[i].ptEnd = SimpleLeftBufPoint(ptWp[i + 1].x, ptWp[i + 1].y, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(leftBufLeg[i].ptEnd.x, leftBufLeg[i].ptEnd.y);
			rightBufLeg[i].ptEnd = SimpleRightBufPoint(ptWp[i + 1].x, ptWp[i + 1].y, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(rightBufLeg[i].ptEnd.x, rightBufLeg[i].ptEnd.y);
		}
		else {
			// Complex(start)
			double prevHeading = pRoutePlanner->GetHeading(_routeIndex, i - 1);
			double currentHeading = pRoutePlanner->GetHeading(_routeIndex, i);

			double angle = GetAngleFromLegs(prevHeading, currentHeading, false);
			double theta = angle * DEG2RAD;

			double offset = 0;

			if (theta != M_PI) {
				offset = buffDistance / tan(theta / 2.0);
			}

			LatLonUtility::Pos pos = LatLonUtility::GetDestinationPointRhumbLineMercator(ptWp[i].y, ptWp[i].x, pRoutePlanner->GetHeading(_routeIndex, i), offset, false);
			centerBufLeg[i].ptStart = SimpleLeftBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), 0, false);
			projection(centerBufLeg[i].ptStart.x, centerBufLeg[i].ptStart.y);
			leftBufLeg[i].ptStart = SimpleLeftBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(leftBufLeg[i].ptStart.x, leftBufLeg[i].ptStart.y);
			rightBufLeg[i].ptStart = SimpleRightBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(rightBufLeg[i].ptStart.x, rightBufLeg[i].ptStart.y);

			startOffset[i] = LatLonUtility::GetDistanceRhumbLineMercator(pos.lat, pos.lon, ptWp[i].y, ptWp[i].x, false);

			D2D1_POINT_2F ptOffset;
			projection(pos.lon, pos.lat);

			ptOffset.x = pos.lon;
			ptOffset.y = pos.lat;
			WorldToDevice(ptOffset);

			isRight[i - 1] = IsRight(ptWp[i - 1].x, ptWp[i - 1].y, ptWp[i].x, ptWp[i].y,
				ptWp[i].x, ptWp[i].y, ptWp[i + 1].x, ptWp[i + 1].y);

			D2D1_POINT_2F ptCircle;
			D2D1_POINT_2F ptCircleStart;
			D2D1_POINT_2F ptCircleEnd;
			D2D1_POINT_2F ptInnerCircleStart;
			D2D1_POINT_2F ptInnerCircleEnd;

			if (isRight[i - 1]) { // 오른쪽 
				ptCircle.x = rightBufLeg[i].ptStart.x;
				ptCircle.y = rightBufLeg[i].ptStart.y;
				WorldToDevice(ptCircle);
				double bufDistancePixel = sqrt((ptCircle.x - ptOffset.x) * (ptCircle.x - ptOffset.x) + (ptCircle.y - ptOffset.y) * (ptCircle.y - ptOffset.y));

				ptAngularPoint.x = rightBufLeg[i].ptEnd.x;
				ptAngularPoint.y = rightBufLeg[i].ptEnd.y;

				ptInnerCircleEnd.x = centerBufLeg[i].ptStart.x;
				ptInnerCircleEnd.y = centerBufLeg[i].ptStart.y;

				ptCircleStart.x = leftBufLeg[i - 1].ptEnd.x;
				ptCircleStart.y = leftBufLeg[i - 1].ptEnd.y;

				ptCircleEnd.x = leftBufLeg[i].ptStart.x;
				ptCircleEnd.y = leftBufLeg[i].ptStart.y;

				ptAngularPoint.x = rightBufLeg[i].ptStart.x;
				ptAngularPoint.y = rightBufLeg[i].ptStart.y;

				WorldToDevice(ptInnerCircleEnd);

				pGeometrySinkRouteBufferCenter->AddArc(
					D2D1::ArcSegment(
						ptInnerCircleEnd,//end point
						D2D1::SizeF(bufDistancePixel, bufDistancePixel),
						angle,//rotation angle
						D2D1_SWEEP_DIRECTION_CLOCKWISE,
						D2D1_ARC_SIZE_SMALL
					));

				WorldToDevice(ptCircleEnd);

				pGeometrySinkRouteBufferRight->AddArc(
					D2D1::ArcSegment(
						ptCircleEnd,//end point
						D2D1::SizeF(bufDistancePixel * 2, bufDistancePixel * 2),
						angle,//rotation angle
						D2D1_SWEEP_DIRECTION_CLOCKWISE,
						D2D1_ARC_SIZE_SMALL
					));
				// arc area part
				{
					HRESULT hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pathRouteBufferArcPart);

					if (SUCCEEDED(hr))
					{
						ID2D1GeometrySink* pGeometrySink = NULL;
						hr = pathRouteBufferArcPart->Open(&pGeometrySink);

						pGeometrySink->SetFillMode(D2D1_FILL_MODE_WINDING);

						WorldToDevice(ptCircleStart);

						pGeometrySink->BeginFigure(
							ptCircleStart,
							D2D1_FIGURE_BEGIN_FILLED
						);

						WorldToDevice(ptAngularPoint);
						//WorldToDevice(ptCircleEnd);

						pGeometrySink->AddLine(ptAngularPoint);
						pGeometrySink->AddLine(ptCircleEnd);
						pGeometrySink->AddArc(
							D2D1::ArcSegment(
								D2D1::Point2F(ptCircleStart.x, ptCircleStart.y),//end point
								D2D1::SizeF(bufDistancePixel * 2, bufDistancePixel * 2),
								angle,//rotation angle
								D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
								D2D1_ARC_SIZE_SMALL
							));

						pGeometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);

						hr = pGeometrySink->Close();
						SafeRelease(&pGeometrySink);
					}
				}

			}
			else { // 왼쪽
				ptCircle.x = leftBufLeg[i].ptStart.x;
				ptCircle.y = leftBufLeg[i].ptStart.y;
				WorldToDevice(ptCircle);
				double bufDistancePixel = sqrt((ptCircle.x - ptOffset.x) * (ptCircle.x - ptOffset.x) + (ptCircle.y - ptOffset.y) * (ptCircle.y - ptOffset.y));

				ptInnerCircleStart.x = centerBufLeg[i - 1].ptEnd.x;
				ptInnerCircleStart.y = centerBufLeg[i - 1].ptEnd.y;

				ptInnerCircleEnd.x = centerBufLeg[i].ptStart.x;
				ptInnerCircleEnd.y = centerBufLeg[i].ptStart.y;

				ptCircleStart.x = rightBufLeg[i - 1].ptEnd.x;
				ptCircleStart.y = rightBufLeg[i - 1].ptEnd.y;

				ptCircleEnd.x = rightBufLeg[i].ptStart.x;
				ptCircleEnd.y = rightBufLeg[i].ptStart.y;

				ptAngularPoint.x = leftBufLeg[i].ptStart.x;
				ptAngularPoint.y = leftBufLeg[i].ptStart.y;

				WorldToDevice(ptInnerCircleEnd);
				WorldToDevice(ptCircleEnd);

				pGeometrySinkRouteBufferCenter->AddArc(
					D2D1::ArcSegment(
						ptInnerCircleEnd,//end point
						D2D1::SizeF(bufDistancePixel, bufDistancePixel),
						angle,//rotation angle
						D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
						D2D1_ARC_SIZE_SMALL
					));

				pGeometrySinkRouteBufferLeft->AddArc(
					D2D1::ArcSegment(
						ptCircleEnd,//end point
						D2D1::SizeF(bufDistancePixel * 2, bufDistancePixel * 2),
						angle,//rotation angle
						D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
						D2D1_ARC_SIZE_SMALL
					));

				// arc area part
				{
					HRESULT hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pathRouteBufferArcPart);

					if (SUCCEEDED(hr))
					{
						ID2D1GeometrySink* pGeometrySink = NULL;
						hr = pathRouteBufferArcPart->Open(&pGeometrySink);

						pGeometrySink->SetFillMode(D2D1_FILL_MODE_WINDING);

						WorldToDevice(ptCircleStart);
						WorldToDevice(ptAngularPoint);
						//WorldToDevice(ptCircleEnd);

						pGeometrySink->BeginFigure(
							ptCircleStart,
							D2D1_FIGURE_BEGIN_FILLED
						);

						pGeometrySink->AddLine(ptAngularPoint);
						pGeometrySink->AddLine(ptCircleEnd);
						pGeometrySink->AddArc(
							D2D1::ArcSegment(
								ptCircleStart,//end point
								D2D1::SizeF(bufDistancePixel * 2, bufDistancePixel * 2),
								angle,//rotation angle
								D2D1_SWEEP_DIRECTION_CLOCKWISE,
								D2D1_ARC_SIZE_SMALL
							));

						pGeometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);

						hr = pGeometrySink->Close();
						SafeRelease(&pGeometrySink);
					}
				}
			}

			// Complex(end)
			currentHeading = pRoutePlanner->GetHeading(_routeIndex, i);
			double nextHeading = pRoutePlanner->GetHeading(_routeIndex, i + 1);

			theta = GetAngleFromLegs(currentHeading, nextHeading, false) * DEG2RAD;

			offset = 0;

			if (theta != M_PI) {
				offset = buffDistance / tan(theta / 2.0);
			}

			pos = LatLonUtility::GetDestinationPointRhumbLineMercator(ptWp[i + 1].y, ptWp[i + 1].x, pRoutePlanner->GetHeading(_routeIndex, i) + 180, offset, false);
			centerBufLeg[i].ptEnd = SimpleLeftBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), 0, false);
			projection(centerBufLeg[i].ptEnd.x, centerBufLeg[i].ptEnd.y);
			leftBufLeg[i].ptEnd = SimpleLeftBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(leftBufLeg[i].ptEnd.x, leftBufLeg[i].ptEnd.y);
			rightBufLeg[i].ptEnd = SimpleRightBufPoint(pos.lon, pos.lat, pRoutePlanner->GetHeading(_routeIndex, i), buffDistance, false);
			projection(rightBufLeg[i].ptEnd.x, rightBufLeg[i].ptEnd.y);

			endOffset[i] = LatLonUtility::GetDistanceRhumbLineMercator(pos.lat, pos.lon, ptWp[i + 1].y, ptWp[i + 1].x, false);

			projection(pos.lon, pos.lat);

			ptOffset.x = pos.lon;
			ptOffset.y = -pos.lat;
		}

		D2D1_POINT_2F points[4];
		D2D1_POINT_2F pointsCenter[2];
		D2D1_POINT_2F pointsLeft[2];
		D2D1_POINT_2F pointsRight[2];
		// draw right
		D2D1_POINT_2F ptStart;
		D2D1_POINT_2F ptEnd;

		ptStart.x = centerBufLeg[i].ptStart.x;
		ptStart.y = centerBufLeg[i].ptStart.y;

		ptEnd.x = centerBufLeg[i].ptEnd.x;
		ptEnd.y = centerBufLeg[i].ptEnd.y;

		pointsCenter[0].x = ptStart.x;
		pointsCenter[0].y = ptStart.y;

		pointsCenter[1].x = ptEnd.x;
		pointsCenter[1].y = ptEnd.y;

		WorldToDevice(pointsCenter[0]);
		WorldToDevice(pointsCenter[1]);

		ptStart.x = rightBufLeg[i].ptStart.x;
		ptStart.y = rightBufLeg[i].ptStart.y;

		ptEnd.x = rightBufLeg[i].ptEnd.x;
		ptEnd.y = rightBufLeg[i].ptEnd.y;

		points[0].x = ptStart.x;
		points[0].y = ptStart.y;

		points[1].x = ptEnd.x;
		points[1].y = ptEnd.y;

		pointsLeft[0].x = ptStart.x;
		pointsLeft[0].y = ptStart.y;

		pointsLeft[1].x = ptEnd.x;
		pointsLeft[1].y = ptEnd.y;

		WorldToDevice(pointsLeft[0]);
		WorldToDevice(pointsLeft[1]);

		ptStart.x = leftBufLeg[i].ptStart.x;
		ptStart.y = leftBufLeg[i].ptStart.y;
		ptEnd.x = leftBufLeg[i].ptEnd.x;
		ptEnd.y = leftBufLeg[i].ptEnd.y;

		points[2].x = ptEnd.x;
		points[2].y = ptEnd.y;

		points[3].x = ptStart.x;
		points[3].y = ptStart.y;

		pointsRight[0].x = ptStart.x;
		pointsRight[0].y = ptStart.y;

		pointsRight[1].x = ptEnd.x;
		pointsRight[1].y = ptEnd.y;

		WorldToDevice(pointsRight[0]);
		WorldToDevice(pointsRight[1]);

		HRESULT hr = gisLib->D2.pD2Factory->CreatePathGeometry(&pathRouteBufferPart);

		if (SUCCEEDED(hr))
		{
			ID2D1GeometrySink* pGeometrySink = NULL;
			hr = pathRouteBufferPart->Open(&pGeometrySink);

			pGeometrySink->SetFillMode(D2D1_FILL_MODE_WINDING);

			WorldToDevice(points[0]);
			WorldToDevice(points[1]);
			WorldToDevice(points[2]);
			WorldToDevice(points[3]);

			pGeometrySink->BeginFigure(
				points[3],
				D2D1_FIGURE_BEGIN_FILLED
			);

			pGeometrySink->AddLines(points, 4);
			pGeometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);

			hr = pGeometrySink->Close();
			SafeRelease(&pGeometrySink);
		}

		if (i == 0)
		{
			pathRouteBuffer = pathRouteBufferPart;
		}
		else
		{
			if (pathRouteBufferArcPart)
			{
				tempGeometry = combine_twice_path_geometries(gisLib->D2.pD2Factory, pathRouteBuffer, pathRouteBufferArcPart);
				SafeRelease(&pathRouteBuffer);
				SafeRelease(&pathRouteBufferArcPart);
				pathRouteBuffer = tempGeometry;
			}
			tempGeometry = combine_twice_path_geometries(gisLib->D2.pD2Factory, pathRouteBuffer, pathRouteBufferPart);
			SafeRelease(&pathRouteBuffer);
			SafeRelease(&pathRouteBufferPart);
			pathRouteBuffer = tempGeometry;
		}

		//// Center
		if (i == 0)
		{
			pGeometrySinkRouteBufferCenter->BeginFigure(pointsCenter[0], D2D1_FIGURE_BEGIN_FILLED);
		}

		pGeometrySinkRouteBufferCenter->AddLine(pointsCenter[1]);

		//// Left
		if (i == 0)
		{
			pGeometrySinkRouteBufferLeft->BeginFigure(pointsLeft[0], D2D1_FIGURE_BEGIN_FILLED);
		}

		pGeometrySinkRouteBufferLeft->AddLine(pointsLeft[1]);

		//// Right
		if (i == 0)
		{
			pGeometrySinkRouteBufferRight->BeginFigure(pointsRight[0], D2D1_FIGURE_BEGIN_FILLED);
		}

		pGeometrySinkRouteBufferRight->AddLine(pointsRight[1]);
	}

	ID2D1SolidColorBrush* pRedBrush = nullptr;

	gisLib->D2.pRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Yellow),
		&pRedBrush
	);

	gisLib->D2ChartResources.SetRouteBufferZoneGeometry(pathRouteBuffer);

	D2D1_RECT_F boundsBuffer;
	//pathRouteBuffer->GetBounds(D2D1::Matrix3x2F::Identity(), &boundsBuffer);
	//CString strBounds;
	//strBounds.Format(L"%f %f %f %f\n", boundsBuffer.left, boundsBuffer.top, boundsBuffer.right, boundsBuffer.bottom);
	//OutputDebugString(strBounds);

	pGeometrySinkRouteBufferCenter->EndFigure(D2D1_FIGURE_END_OPEN);
	hr = pGeometrySinkRouteBufferCenter->Close();
	SafeRelease(&pGeometrySinkRouteBufferCenter);

	pGeometrySinkRouteBufferLeft->EndFigure(D2D1_FIGURE_END_OPEN);
	hr = pGeometrySinkRouteBufferLeft->Close();
	SafeRelease(&pGeometrySinkRouteBufferLeft);

	pGeometrySinkRouteBufferRight->EndFigure(D2D1_FIGURE_END_OPEN);
	hr = pGeometrySinkRouteBufferRight->Close();
	SafeRelease(&pGeometrySinkRouteBufferRight);

	//gisLib->D2.pRT->DrawGeometry(pathRouteBufferCenter, gisLib->D2.pBrush, 10, gisLib->D2.D2D1StrokeStyleGroup[1]);
	gisLib->D2.pRT->DrawGeometry(pathRouteBufferLeft, gisLib->D2.pBrush, 3, gisLib->D2.D2D1StrokeStyleGroup[1]);
	gisLib->D2.pRT->DrawGeometry(pathRouteBufferRight, gisLib->D2.pBrush, 3, gisLib->D2.D2D1StrokeStyleGroup[1]);

	SafeRelease(&pRedBrush);
	SafeRelease(&pathRouteBufferCenter);
	SafeRelease(&pathRouteBufferLeft);
	SafeRelease(&pathRouteBufferRight);

	gisLib->D2.End();

	//DrawOwnShip(pDC, isMoveDraw, ptWp, rightBufLeg, leftBufLeg, isRight, startOffset, endOffset, buffDistance, waypointCount);

	delete[] rightBufLeg;
	delete[] leftBufLeg;
	delete[] ptWp;
	delete[] isRight;
	delete[] startOffset;
	delete[] endOffset;
}

RT::PointD CGISLibraryApp::SimpleRightBufPoint(double _x, double _y, double _heading, double _distance, bool _radian)
{
	if (_radian) {
		_heading += (M_PI / 2);
	}
	else {
		_heading += 90;
	}

	LatLonUtility::Pos pos = LatLonUtility::GetDestinationPointRhumbLineMercator(_y, _x, _heading, _distance, _radian);
	RT::PointD result;
	result.x = pos.lon;
	result.y = pos.lat;
	return result;
}


RT::PointD CGISLibraryApp::SimpleLeftBufPoint(double _x, double _y, double _heading, double _distance, bool _radian)
{
	if (_radian) {
		_heading -= (M_PI / 2);
	}
	else {
		_heading -= 90;
	}

	LatLonUtility::Pos pos = LatLonUtility::GetDestinationPointRhumbLineMercator(_y, _x, _heading, _distance, _radian);
	RT::PointD result;
	result.x = pos.lon;
	result.y = pos.lat;
	return result;
}

ID2D1PathGeometry* CGISLibraryApp::combine_twice_path_geometries(ID2D1Factory1*& srcfactory, ID2D1PathGeometry* geo1, ID2D1PathGeometry* geo2)
{
	ID2D1PathGeometry* path_geo_1 = NULL;
	srcfactory->CreatePathGeometry(&path_geo_1);
	ID2D1GeometrySink* cmpl_s1 = NULL;
	path_geo_1->Open(&cmpl_s1);
	geo1->CombineWithGeometry(geo2, D2D1_COMBINE_MODE_UNION, NULL, cmpl_s1);
	cmpl_s1->Close();
	cmpl_s1->Release();

	return path_geo_1;
}

double CGISLibraryApp::GetAngleFromLegs(double _heading1, double _heading2, bool _radian)
{
	double PI = acos(-1.0);

	if (!_radian) {
		PI *= RAD2DEG;
	}

	if (_heading1 > PI) {
		_heading1 -= PI;
	}

	if (_heading2 > PI) {
		_heading2 -= PI;
	}

	double result = PI - _heading1 + _heading2;

	//if (result > PI) {
	//	result -= PI;
	//}

	if (result > PI) {
		result = (2 * PI) - result;
	}

	return abs(result);
}

bool CGISLibraryApp::IsRight(double _x1Leg1, double _y1Leg1, double _x2Leg1, double _y2Leg1,
	double _x1Leg2, double _y1Leg2, double _x2Leg2, double _y2Leg2)
{
	double x1 = _x2Leg1 - _x1Leg1;
	double y1 = _y2Leg1 - _y1Leg1;
	double x2 = _x2Leg2 - _x1Leg2;
	double y2 = _y2Leg2 - _y1Leg2;

	double det = (x1 * y2) - (y1 * x2);

	if (det < 0) {
		return true;
	}

	return false;
}

RT::RoutePlanner* CGISLibraryApp::GetRoutePlanner()
{
	return pRoutePlanner;
}

void CGISLibraryApp::DrawOwnShipGuardZone(HDC hdc)
{
	if (false == ENCCommon::DETECTION_MODE)
	{
		return;
	}

	//ID2D1PathGeometry* OwnShipGuardZoneGeometry = gisLib->D2ChartResources.GetGuardZoneGeometry();
	auto OwnShipGuardZoneGeometry = gisLib->D2ChartResources.MakeGuardZoneGeometry(gisLib->D2ChartResources.GetGuardZone());

	//auto matrix = gisLib->GetScaler()->GetMatrix();

	gisLib->D2.Begin(hdc);
	gisLib->D2.pBrush->SetColor(D2D1::ColorF(1, 1, 1));
	gisLib->D2.pBrush->SetOpacity(1.0);
	//gisLib->D2.pRT->SetTransform(matrix);
	gisLib->D2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());
	gisLib->D2.pRT->DrawGeometry(OwnShipGuardZoneGeometry, gisLib->D2.pBrush, 2, gisLib->D2.D2D1StrokeStyleGroup[2]);
	gisLib->D2.End();
}

bool CGISLibraryApp::OpenRouteFromRTZ(std::wstring path)
{
	checkRouteDetection = true;

	RTZ::Route route;
	if (route.Open(path))
	{
		auto RouteName = route.getRouteInfo()->getRouteName();

		auto pRoutePlanner = gisLib->pRoutePlanner;

		//라우트 등록
		pRoutePlanner->AddRoute(pugi::as_wide(RouteName));

		auto RouteIdx = pRoutePlanner->GetRouteCount() - 1;

		if (route.getWaypoints()->getDefaultWaypoint() != nullptr)
		{
			//Radius 등록
			std::shared_ptr<double> radius = route.getWaypoints()->getDefaultWaypoint()->getRadius();
			if (radius != nullptr)
				pRoutePlanner->SetBufferDistance(RouteIdx, *radius);

			//Waypoint 등록
			std::vector<std::shared_ptr<RTZ::Waypoint>> waypoints = route.getWaypoints()->getWaypoints();
			for (int i = 0; i < waypoints.size(); i++)
			{
				RTZ::GM_Point point = waypoints[i]->getPosition();
				double Max = 10;
				double Min = 10;

				if (waypoints[i]->getLeg() != nullptr)
				{
					if (waypoints[i]->getLeg()->getSpeedMax() != nullptr && waypoints[i]->getLeg()->getSpeedMin() != nullptr)
					{
						Max = *waypoints[i]->getLeg()->getSpeedMax();
						Min = *waypoints[i]->getLeg()->getSpeedMin();
					}
				}
				pRoutePlanner->AddWaypoint(RouteIdx, point.getLon(), point.getLat(), Min, Max);
			}
		}
		else
		{
			std::vector<std::shared_ptr<RTZ::Waypoint>> waypoints = route.getWaypoints()->getWaypoints();
			//Radius 등록
			if (waypoints.empty() == FALSE)
				waypoints[0]->getRadius() == nullptr ? NULL : pRoutePlanner->SetBufferDistance(RouteIdx, *waypoints[0]->getRadius());

			//Waypoint 등록
			for (int i = 0; i < waypoints.size(); i++)
			{
				RTZ::GM_Point point = waypoints[i]->getPosition();
				double Max = 10;
				double Min = 10;

				if (waypoints[i]->getLeg() != nullptr)
				{
					if (waypoints[i]->getLeg()->getSpeedMax() != nullptr && waypoints[i]->getLeg()->getSpeedMin() != nullptr)
					{
						Max = *waypoints[i]->getLeg()->getSpeedMax();
						Min = *waypoints[i]->getLeg()->getSpeedMin();
					}
				}
				pRoutePlanner->AddWaypoint(RouteIdx, point.getLon(), point.getLat(), Min, Max);
			}
		}
	}

	return true;
}