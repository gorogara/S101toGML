#include "pch.h"
#include "Catalog.h"
#include "FeatureCatalogue.h"

//#include "..\\DLL_MessageProcess\\DLL_MessageProcess.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

Catalog::Catalog()
{
}

Catalog::Catalog(std::wstring path)
{
	ReadXMLFileByPugiXML(path);
}

Catalog::~Catalog()
{
	for (auto itor = m_pCatalogueList.begin(); itor != m_pCatalogueList.end(); itor++)
	{
		delete itor->second;
	}
}

bool Catalog::ReadXMLFileByPugiXML(std::wstring path)
{
	// 피처 카탈로그 (파일명) 로딩 시작
	SetFilePath(path);

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(m_filepath.c_str()); ///파일을 읽습니다.
	pugi::xml_node displayList = doc.child("S100FC:S100_FC_FeatureCatalogue");

 	if (displayList!= nullptr) 
	{
		FeatureCatalogue *fc = new FeatureCatalogue();
		fc->GetContents(displayList);
		AddFeatureCatalogue(fc->GetName(), fc);
		// 피처 카탈로그 (파일명) 로딩 완료
		//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:S100_FC_FeatureCatalogue loading",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		return true;
	}
	else
	{ 
		//KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_ERROR, L"S100FC:S100_FC_FeatureCatalogue is null",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		return false;
	}
}

void Catalog::AddFeatureCatalogue(std::wstring name, FeatureCatalogue* fc)
{
	if (name.empty() == true || fc == nullptr)
	{
		return;
	}

	m_pCatalogueList.insert({ name, fc });
}

std::wstring Catalog::GetFeatureCatalogueName()
{
	if (m_pCatalogueList.size() > 0)
	{
		return m_pCatalogueList.begin()->first;
	}

	return L"";
}

void Catalog::SetFilePath(std::wstring& value)
{
	m_filepath = value;
}

std::wstring Catalog::GetFilePath()
{
	return m_filepath;
}

FeatureCatalogue* Catalog::GetFC()
{
	if (m_pCatalogueList.size() > 0)
	{
		return m_pCatalogueList.begin()->second;
	}

	return nullptr;
}