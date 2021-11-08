#pragma once
#include "LayerManager.h"

#include "D2D1Resources.h"
#include "D2D1ChartResources.h"
#include "BasicFile.h"

#include "../libS100HDF5/Enum_S413FeatureTypes.h"

#include "../S100Engine/S100Engine.h"

#include "../GeoMetryLibrary/Scaler.h"
#include "../GeoMetryLibrary/ENCGeometry.h"

#include "../RoutePlanner/RoutePlanner.h"

#include "..\\extlibs\\Clipper\\include\\clipper.hpp"

// For S-100 ECDIS(NAVIK) co-work DLL
#include "../EXTERNAL_CLASS/UKC_DATA.h"
#include "../EXTERNAL_CLASS/S100_PICK_REPORT_DATA.h"
#include "../EXTERNAL_CLASS/S100_TEST_DATA_LIST.h"
#include "../EXTERNAL_CLASS/PCB_MESSAGE.h"
#include "../EXTERNAL_CLASS/FIXED_STATION.h"

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

class S100Engine;
struct ID2D1Factory1;
struct ID2D1StrokeStyle1;

class CGISLibraryApp
{
public:
	CGISLibraryApp();
	virtual ~CGISLibraryApp();

public:
	Scaler* m_pScaler = new Scaler();
	LayerManager* m_pLayerManager = new LayerManager(m_pScaler);

	S100Engine *s111Engine = new S100Engine();
	S100Engine *s413Engine = new S100Engine();

	S413FeatureType s413Type = WaveWind;

	GISLibrary::D2D1Resources D2;
	GISLibrary::D2D1ChartResources D2ChartResources;

	std::vector<BasicFile> BasicFilePath;

	RT::RoutePlanner* pRoutePlanner = new RT::RoutePlanner();

	bool checkRouteDetection = false;

public:
	Scaler* GetScaler();
	LayerManager* GetLayerManager();

	int GetLayerIndex(CString pathName);

	void InitS100Engine();

	void InitLibrary();

	bool AddBackgroundLayer(CString _filepath);
	bool AddUserDefinedLayer(CString _filepath);
	bool AddLayer(CString _filepath);
	bool AddLayer(CString _filepath, Catalog* fc, PortrayalCatalogue* pc);
	bool AddLayerFromS100ExchangeSet(CString _filepath, Catalog* fc, PortrayalCatalogue* pc);
	bool AddShipMonitoring(CString _filePath);

	void Draw(HDC &hDC, int offset = 0);
	void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, double offsetX = 0, double offsetY = 0);
	void DrawS100Catalog(HDC &hdc, int offset = 0);

	void BuildS101Portrayal(Layer* l/*, PORTRAYAL_BUILD_TYPE type = PORTRAYAL_BUILD_TYPE::ALL*/);
	void DeleteS101Portrayal(Layer* l/*, PORTRAYAL_BUILD_TYPE type = PORTRAYAL_BUILD_TYPE::ALL*/);
	void S101RebuildPortrayal(/*PORTRAYAL_BUILD_TYPE type = PORTRAYAL_BUILD_TYPE::ALL*/);
	void DrawValidationLayers(HDC &hDC, int offset = 0);
	void DrawOverlay(HDC &hDC, int type, int offset = 0);
	void DrawShipMonitoringLayer(HDC &hDC, int offset = 0);
	void GetLayer(int index, Layer *_layer);
	Layer* GetLayer(int index);

	void DrawS100Symbol(int productNumber, std::wstring symbolName, int screenX, int screenY, int rotation, float scale = 5);
	void DrawScaleBar();

	void LayerOn(int index);
	void LayerInfoOn(int index);

	void LayerOff(int index);
	void LayerInfoOff(int index);

	void LayerUp(int index);
	void LayerDown(int index);
	CString GetLayerName(int index);
	BOOL IsOn(int index);
	void DeleteLayer(int index);
	void DeleteLayer(CString filepath);
	void DeleteAllLayer();
	void ReMBR();

	void ClearInformationLayer(int index);
	void ClearInformationLayer(CString filepath);
	void ClearAllInformationLayer();
	void CheckDataCoverageComputing();

	void SaveLayerTextPlacement(CString filename, CString extend, int index);
	void SaveLayer(CString filename, CString extend, int index);
	void SaveLayer(CString filename, CString extend, Layer* pLayer);

	// 0 : const
	// 1 : dynamic
	void UpdateS102ColorMapType(int _type, std::vector<COLORREF> _colorMap);
	void GetS102ColorMapType(int& _type, std::vector<COLORREF>& _colorMap);

	void SetViewMBR(RECT r);

	/*
	** 저장된 레이어의 개수를 불러 온다.
	*/
	int GetLayerCount();
	std::vector<Layer*> GetAllLayer();

	MBR* GetMBR();
	void SetColorTable(std::wstring _path);
						
	/////////////////////////////////////////////////////////////////////////////
	//  Scalar Method
	// 좌표 변환
	void DeviceToWorld(long sx, long sy, double *mx, double *my, bool rotate = TRUE);
	void WorldToDevice(double mx, double my, long *sx, long *sy, bool rotate = TRUE);
	void WorldToDevice(D2D1_POINT_2F& p);
	void DeviceToWorld(D2D1_POINT_2F& p);

	// 거리 계산
	//double GetDistanceLatitudeLongitude(double longitude1, double latitude1, double longitude2, double latitude2);
	double GetDistanceScreen(int x1, int y1, int x2, int y2);

	// 화면 설정
	void SetScreen(RECT *rect);
	void SetScreen(const RECT& rect);
	int GetCenterXScreen();
	int GetCenterYScreen();

	// 화면에 보여지는 Map의 정보 
	void SetMap(double xmin, double ymin, double xmax, double ymax);
	void SetMap(MBR mbr);

	double GetMapWidth();
	double GetMapHeight();

	void GetMap(MBR *mbr);

	// Scale 정보
	int GetCurrentScale();
	void   SetScale(int scale);
	void   UpdateScale();

	/*
	** 확대 & 축소
	** 인  자 : %단위
	** 사용예 : 20%확대하고 싶을땐 인자에 "20" 삽입
	*/
	void ZoomIn(double value);
	void ZoomIn(double value, int x, int y);
	void ZoomOut(double value);
	void ZoomOut(double value, int x, int y);
	void AdjustScreenMap();

	// 화면 이동
	// 원하는 픽셀만큼 움직일때 사용(미완성)
	void MoveMap(int x, int y);

	// 원하는 위치(sy, sy)에 원하는 점을(mx, my)를 위치 시킬때 사용
	void MoveMap(int sx, int sy, double mx, double my);

	// 화면 회전
	void Rotate(LONG *sx, LONG *sy, double degree);
	void RotateMap(double degree);	// rotateDegree의 범위를 [0, 360) 으로 유지
	void NorthUp();

	void NextTimeIndex();
	void PrevTimeIndex();

	bool PtInMap(double _x, double _y);

	void ChangeDisplayFont();

	void SetDrawBackground(bool on);
	bool GetDrawBackground();

	void SetExPCPath(CString value);
	CString GetExPCPath();

	void SetExFCPath(CString value);
	CString GetExFCPath();

	// 초기 로딩할 FC, PC 지정
	void PC_FCPathSetting();

	// FC, PC 로딩
	void BasicFileSetting();

	std::wstring GetColorTable();


	////////////////////////////////////////////// 
	// For S-100 ECDIS(NAVIK) co-work DLL
	// 추가 구현해야 하는 함수
	// inMx, inMy, outMx, outMy - 프로젝션된 좌표
	bool PickReport_S102(double inMx, double inMy, double& outMx, double& outMy, float& depth);

	bool PickReport_S111(double inMx, double inMy, double& outMx, double& outMy, float& direction, float& speed);

	bool PickReport_S111(double inMx, double inMy, std::vector<double>& direction, std::vector<double>& speed);

	// information related Own Ship
	void SetOwnShipPosition(double lon, double lat);
	void SetOwnShipSize(double length, double width);

	// Route
	void MakeOwnShipPosition();
	void DrawRoute(CDC* pDC);
	void DrawRouteBuffer_D2D1(CDC* pDC, int _routeIndex);
	RT::PointD SimpleRightBufPoint(double _x, double _y, double _heading, double _distance, bool _radian = true);
	RT::PointD SimpleLeftBufPoint(double _x, double _y, double _heading, double _distance, bool _radian = true);
	ID2D1PathGeometry* combine_twice_path_geometries(ID2D1Factory1*& srcfactory, ID2D1PathGeometry* geo1, ID2D1PathGeometry* geo2);
	double GetAngleFromLegs(double _heading1, double _heading2, bool _radian);
	bool IsRight(double _x1Leg1, double _y1Leg1, double _x2Leg1, double _y2Leg1,
		double _x1Leg2, double _y1Leg2, double _x2Leg2, double _y2Leg2);

	RT::RoutePlanner* GetRoutePlanner();
	void DrawOwnShipGuardZone(HDC hdc);

	bool OpenRouteFromRTZ(std::wstring path);
};

extern CGISLibraryApp* gisLib;