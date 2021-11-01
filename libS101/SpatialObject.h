#pragma once

#include "Enum_S100_FileType.h"
#include "Namespace_GISLibrary.h"

#include <string>

class Layer;
class Scaler;

class SpatialObject
{
public:
	SpatialObject();
	virtual ~SpatialObject();

public:
	Layer* m_pLayer = nullptr;
	S100_FileType m_FileType; // 파일 타입 : SHP, S-57, S-101, GML 지원

private:
	std::wstring m_FilePath; // 파일 전체 경로 (이름 포함)
	std::wstring m_FileName; // 파일 이름 (확장자 포함)

	//CString m_FilePath; // 파일 전체 경로 (이름 포함)
	//CString m_FileName; // 파일 이름 (확장자 포함)

public:
	virtual bool Open(std::wstring _filepath);
	/*virtual void Draw(CDC *pDC, Scaler *scaler, double offset = 0) {};
	virtual void Draw(HDC &hDC, Scaler *scaler, double offset = 0) {};
	virtual void Draw(HDC &hDC, Scaler *scaler, int priority, int geoType, double offset = 0) {};
	virtual void Draw(GISLibrary::D2D1Resources* D2, Scaler* scaler) {};
	virtual void Draw(CDCRenderTarget* pRenderTarget, Scaler *scaler, double offsetX = 0, double offsetY = 0) {};
	virtual void Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, Scaler *scaler, double offsetX = 0, double offsetY = 0) {};*/
	virtual void Save(std::wstring filename, std::wstring extend);

	void SetLayer(Layer* value);
	Layer* GetLayer();

	void SetFilePath(std::wstring& value);
	std::wstring GetFilePath();

	void SetFileName(std::wstring& value);
	std::wstring GetFileName();

	void SetFileType(S100_FileType value);
	S100_FileType GetFileType();

};