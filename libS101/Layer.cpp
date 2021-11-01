#include "StdAfx.h"
#include "Layer.h"
#include "S100Utilities.h"
#include "S100CatalogFile.h"
#include "S104Cell.h"

#include "..\\LibMFCUtil\\LibMFCUtil.h"

Layer::Layer(void)
{
	On = true;
	Info = false;
}

Layer::~Layer(void)
{
	if (m_spatialObject)
	{
		delete m_spatialObject;
		m_spatialObject = nullptr;
	}
}

bool Layer::Open(CString _filepath)
{
	auto extension = LibMFCUtil::GetExtension(_filepath);

	if (extension.CompareNoCase(L"XML") == 0)
	{
		if (true == S100Utilities::IsS100Catalog(std::wstring(_filepath)))
		{
			m_spatialObject = new S100CatalogFile();
		}
	}
	else if (extension.CompareNoCase(L"h5") == 0)
	{
		auto name = LibMFCUtil::GetFileName(_filepath);
		
		if (name.GetLength() >= 3)
		{
			auto strProductNumber = name.Left(3);
			if (strProductNumber.CompareNoCase(L"104") == 0)
			{
				m_spatialObject = new S104Cell();
			}
		}
	}

	if (nullptr == m_spatialObject)
	{
		m_spatialObject = new SHPFile();
	}

	m_spatialObject->m_pLayer = this;

	CString strFolderPath;
	if (_filepath.Find(TEXT(":\\")) > 0)
	{
		strFolderPath = _filepath;
	}
	else
	{
		::GetModuleFileName(nullptr, strFolderPath.GetBuffer(MAX_PATH), MAX_PATH);
		strFolderPath.ReleaseBuffer();
		if (strFolderPath.Find('\\') != -1)
		{
			for (int i = strFolderPath.GetLength() - 1; i >= 0; i--)
			{
				TCHAR ch = strFolderPath[i];
				strFolderPath.Delete(i);
				if (ch == '\\') break;
			}
		}
		strFolderPath.Append(TEXT("\\") + _filepath);
	}
	return m_spatialObject->Open(strFolderPath);
}

void Layer::Save(CString _filepath, CString extend)
{
	m_spatialObject->Save(_filepath, extend);
}

void Layer::Draw(CDC *pDC, Scaler *scaler, double offset)
{
	// Layer는 ON 상태일 때만 그린다
	if (On == true)
	{
		m_spatialObject->Draw(pDC, scaler, offset);
	}
}

void Layer::DrawInfo(HDC &hDC, Scaler *scaler)
{
	//if (Info == true)
	//{
	//	// HDC -> CDC(GDI) -> Graphics (GDI+)
	//	Gdiplus::Graphics graphics(hDC);
	//	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	//	Color internalColor(50, 255, 0, 0);
	//	Color lineColor(255, 255, 0, 0);

	//	SolidBrush internalBrush(internalColor); //내부색상
	//	Pen linePen(lineColor); //선



	//	// MBR : Minimum bounding rectangle 
	//	//auto mbr = selectedLayer->GetMBR();
	//	auto mbr = GetMBR();
	//	long sxmin = 0;
	//	long symax = 0;

	//	// 내부 좌표계 -> 화면 좌표계
	//	scaler->WorldToDevice(mbr.xmin, mbr.ymin, &sxmin, &symax);

	//	long sxmax = 0;
	//	long symin = 0;
	//	scaler->WorldToDevice(mbr.xmax, mbr.ymax, &sxmax, &symin);

	//	RectF position = {
	//		(float)sxmin,
	//		(float)symin,
	//		(float)sxmax - sxmin,
	//		(float)symax - symin
	//	};

	//	Gdiplus::Font F(L"Arial", 20, FontStyleRegular, UnitPixel);
	//	//PointF P(10.0f, 10.0f);
	//	SolidBrush B(Color(0, 0, 255));
	//	StringFormat sf;

	//	CString chatName;
	//	chatName.Format(_T("Chart Name : %s \n"), GetChartName());
	//	CString EditionName;
	//	EditionName.Format(_T("Edition Number : %s \n"), GetEditionNumber());
	//	CString IssueDate;
	//	IssueDate.Format(_T("ISSUED DATE : %s \n"), GetIssueDate());

	//	chatName += EditionName;
	//	chatName += IssueDate;

	//	sf.SetAlignment(StringAlignmentNear);
	//	sf.SetLineAlignment(StringAlignmentNear);

	//	graphics.DrawRectangle(&linePen, position.X, position.Y, position.Width, position.Height);
	//	graphics.FillRectangle(&internalBrush, position.X, position.Y, position.Width, position.Height);
	//	graphics.DrawString(chatName, -1, &F, position, &sf, &B); //글자쓰기

	//	//for (auto i = m_SelectedLayer.begin(); i != m_SelectedLayer.end(); i++)
	//	//{

	//	//	// MBR : Minimum bounding rectangle 
	//	//	auto mbr = (*i)->GetMBR();
	//	//	//auto mbr = (*i)->GetMBR();
	//	//	long sxmin = 0;
	//	//	long symax = 0;

	//	//	// 내부 좌표계 -> 화면 좌표계
	//	//	scaler->WorldToDevice(mbr.xmin, mbr.ymin, &sxmin, &symax);

	//	//	long sxmax = 0;
	//	//	long symin = 0;
	//	//	scaler->WorldToDevice(mbr.xmax, mbr.ymax, &sxmax, &symin);

	//	//	/*RECT rect = {
	//	//		sxmin, symin, sxmax, symax
	//	//	};*/

	//	//	RectF position = {
	//	//		(float)sxmin,
	//	//		(float)symin,
	//	//		(float)sxmax - sxmin,
	//	//		(float)symax - symin
	//	//	};

	//	//	Font F(L"Arial", 20, FontStyleRegular, UnitPixel);
	//	//	//PointF P(10.0f, 10.0f);
	//	//	SolidBrush B(Color(0, 0, 255));
	//	//	StringFormat sf;

	//	//	CString layername;
	//	//	layername.Format(_T("LayerName : %s \n"), (*i)->GetLayerName());
	//	//	CString layer;
	//	//	layer.Format(_T("LayerType : %s \n"), (*i)->GetLayerType());
	//	//	layername += layer;

	//	//	sf.SetAlignment(StringAlignmentNear);
	//	//	sf.SetLineAlignment(StringAlignmentNear);

	//	//	graphics.DrawRectangle(&linePen, position.X, position.Y, position.Width, position.Height);
	//	//	graphics.FillRectangle(&internalBrush, position.X, position.Y, position.Width, position.Height);
	//	//	graphics.DrawString(layername, -1, &F, position, &sf, &B); //글자쓰기
	//	//}
	//}
}


void Layer::Draw(HDC &hDC, Scaler *scaler, double offset)
{
	// Layer는 ON 상태일 때만 그린다
	if (On)
	{
		m_spatialObject->Draw(hDC, scaler, offset);
	}
}

void Layer::Draw(HDC &hDC, Scaler *scaler, int priority, int geoType, double offset)
{
	// Layer는 ON 상태일 때만 그린다
	if (On)
	{
		m_spatialObject->Draw(hDC, scaler, priority, geoType, offset);
	}
}

void Layer::Draw(CDCRenderTarget* pRenderTarget, Scaler *scaler, double offsetX, double offsetY)
{
	// Layer는 ON 상태일 때만 그린다
	if (On == TRUE)
	{
		m_spatialObject->Draw(pRenderTarget, scaler, offsetX, offsetY);
	}
}

void Layer::Draw(ID2D1HwndRenderTarget* pRenderTarget, ID2D1Factory *pDXFactory, Scaler *scaler, double offsetX, double offsetY)
{
	// Layer는 ON 상태일 때만 그린다
	if (On == TRUE)
	{
		m_spatialObject->Draw(pRenderTarget, pDXFactory, scaler, offsetX, offsetY);
	}
}

bool Layer::IsOn()
{
	return On;
}

bool Layer::IsOn(int scale)
{
	auto min = GetScaleMinimum();
	auto max = GetScaleMaximum();
	if (0 == max ||
		0 == min ||
		min < max)
	{
		return true;
	}
	else if (min >= scale &&
		max <= scale)
	{
		return true;
	}

	return false;
}

bool Layer::InfoOn()
{
	return Info;
}

void Layer::LayerOn()
{
	On = true;
}


void Layer::LayerOff()
{
	On = false;
}

void Layer::LayerInfoOn()
{
	Info = true;
}

void Layer::LayerInfoOff()
{
	Info = false;
}

CString Layer::GetLayerName()
{
	return m_spatialObject->GetFileName();
}

CString Layer::GetLayerPath()
{
	return m_spatialObject->GetFilePath();
}

CString Layer::GetLayerType()
{
	if (m_spatialObject->m_FileType == S100_FileType::FILE_Shape)
	{
		return L"FILE_Shape";
	}
	else if (m_spatialObject->m_FileType == S100_FileType::FILE_S_57)
	{
		return L"FILE_S_57";
	}
	else if (m_spatialObject->m_FileType == S100_FileType::FILE_S_100_VECTOR)
	{
		return L"FILE_S_100_VECTOR";
	}
	else if (m_spatialObject->m_FileType == S100_FileType::FILE_S_100_GRID_H5)
	{
		return L"FILE_S_100_GRID_H5";
	}
	else if (m_spatialObject->m_FileType == S100_FileType::FILE_S_100_GRID_BAG)
	{
		return L"FILE_S_100_GRID_BAG";
	}

	return L"";
}


void Layer::SetMBR(MBR& value)
{
	m_mbr.SetMBR(value);
}


MBR Layer::GetMBR()
{
	return m_mbr;
}


MBR* Layer::GetMBRPointer()
{
	return &m_mbr;
}


void Layer::SetSpatialObject(SpatialObject* value)
{
	m_spatialObject = value;
}


SpatialObject* Layer::GetSpatialObject()
{
	return m_spatialObject;
}


bool Layer::IsS100Layer()
{
	if (nullptr != GetSpatialObject())
	{
		auto type = GetSpatialObject()->GetFileType();

		if (type == S100_FileType::FILE_S_100_VECTOR ||
			type == S100_FileType::FILE_S_100_GRID_BAG ||
			type == S100_FileType::FILE_S_100_GRID_H5
			)
		{
			return true;
		}
	}

	return false;
}

S100_FileType Layer::GetFileType()
{
	if (nullptr != m_spatialObject)
	{
		return m_spatialObject->m_FileType;
	}

	return S100_FileType::FILE_NONE;
}

int Layer::GetScaleMinimum()
{
	return scaleMinimum;
}

void Layer::SetScaleMiminum(int value)
{
	if (value >= 0)
	{
		scaleMinimum = value;
	}
	else
	{
		scaleMinimum = 0;
	}
}

int Layer::GetScaleMaximum()
{
	return scaleMaximum;
}

void Layer::SetScaleMaximum(int value)
{
	if (value >= 0)
	{
		scaleMaximum = value;
	}
	else
	{
		scaleMaximum = 0;
	}
}