#pragma once

#include <string>
#include <unordered_map>

class FeatureCatalogue;

class Catalog
{
public:
	Catalog();
	Catalog(std::wstring path);
	virtual ~Catalog();

private:
	std::wstring m_filepath;
	std::unordered_map<std::wstring, FeatureCatalogue*> m_pCatalogueList;

public:
	bool ReadXMLFileByPugiXML(std::wstring path);

	void AddFeatureCatalogue(std::wstring name, FeatureCatalogue* fc);

	// return name of first fc
	std::wstring GetFeatureCatalogueName();

	void SetFilePath(std::wstring& value);
	std::wstring GetFilePath();

	// return first fc
	FeatureCatalogue* GetFC();
};