#include "stdafx.h"
#include "SpatialObject.h"

SpatialObject::SpatialObject(void)
{
}

SpatialObject::~SpatialObject(void)
{
}

bool SpatialObject::Open(std::wstring _filepath)
{
	return true;
}

void SpatialObject::Save(std::wstring filename, std::wstring extend)
{
	
}

void SpatialObject::SetLayer(Layer* value)
{
	m_pLayer = value;
}

Layer* SpatialObject::GetLayer()
{
	return m_pLayer;
}

void SpatialObject::SetFilePath(std::wstring& value)
{
	m_FilePath = value;
	int index= value.ReverseFind('\\');
	auto name= value.Mid(index + 1);

	//파일 이름, 확장자 포함
	SetFileName(name);
}

std::wstring SpatialObject::GetFilePath()
{
	return m_FilePath;
	
}

void SpatialObject::SetFileName(std::wstring& value)
{
	m_FileName = value;
}


std::wstring SpatialObject::GetFileName()
{
	return m_FileName;
}

void SpatialObject::SetFileType(S100_FileType value)
{
	m_FileType = value;
}

S100_FileType SpatialObject::GetFileType()
{
	return m_FileType;
}