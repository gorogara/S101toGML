#include "pch.h"
#include "Layer.h"
#include "S100Utilities.h"
#include "S100CatalogFile.h"

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
	//auto extension = LibMFCUtil::GetExtension(_filepath);

	//if (extension.CompareNoCase(L"XML") == 0)
	//{
	//	if (true == S100Utilities::IsS100Catalog(std::wstring(_filepath)))
	//	{
	//		m_spatialObject = new S100CatalogFile();
	//	}
	//}
	//else if (extension.CompareNoCase(L"h5") == 0)
	//{
	//	auto name = LibMFCUtil::GetFileName(_filepath);
	//	
	//	if (name.GetLength() >= 3)
	//	{
	//		auto strProductNumber = name.Left(3);
	//		if (strProductNumber.CompareNoCase(L"104") == 0)
	//		{
	//			m_spatialObject = new S104Cell();
	//		}
	//	}
	//}

	//if (nullptr == m_spatialObject)
	//{
	//	m_spatialObject = new SHPFile();
	//}

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


//MBR* Layer::GetMBRPointer()
//{
//	return &m_mbr;
//}


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