#pragma once
#include "Namespace_GISLibrary.h"
#include "Enum_S100_FileType.h"

#undef _WINDOWS_
#include <afxext.h>
//#include "..\\GeoMetryLibrary\\MBR.h"

class SpatialObject;
class Scaler;

//레이어 들어갈정보를 저장할 Class
class Layer
{

public:
	Layer();
	virtual ~Layer();

public:
	bool	      On;
	bool		Info;
	//정보 레이어를 그릴지 안 그릴지 결정합니다.
	//bool		InfoOn;
	// 영역정보
	//MBR			  m_mbr;
	SpatialObject *m_spatialObject = nullptr;

	int scaleMinimum = 0;
	int scaleMaximum = 0;

public:
	virtual bool Open(CString _filepath);
	void Save(CString _filepath, CString extend);
	/*virtual void Draw(GISLibrary::D2D1Resources* D2, Scaler* scaler) {};
	void Draw(CDC *pDC, Scaler *scaler, double offset = 0);
	virtual void Draw(HDC &hDC, Scaler *scaler, double offset = 0);
	virtual void DrawInfo(HDC &hDC, Scaler *scaler);
	void Draw(HDC &hDC, Scaler *scaler, int priority, int geoType, double offset);
	void Draw(CDCRenderTarget* pRenderTarget, Scaler *scaler, double offsetX = 0, double offsetY = 0);
	void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, Scaler *scaler, double offsetX = 0, double offsetY = 0);
	bool IsOn();
	bool IsOn(int scale);
	bool InfoOn();
	void LayerOn();  
	void LayerOff();
	void LayerInfoOn();
	void LayerInfoOff();*/

	CString GetLayerName();
	CString GetLayerPath();
	CString GetLayerType();

	//void SetMBR(MBR& value);
	//MBR GetMBR();
	//MBR* GetMBRPointer();
	void SetSpatialObject(SpatialObject* value);
	SpatialObject* GetSpatialObject();
	bool IsS100Layer();

	S100_FileType GetFileType();

	int GetScaleMinimum();
	void SetScaleMiminum(int value);

	int GetScaleMaximum();
	void SetScaleMaximum(int value);
}; 