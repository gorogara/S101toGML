#pragma once
#include "XML_Item.h"
#include "IntegerCode.h"
#include "DefinitionReference.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class ListedValue :
	public XML_Item
{
public:
	ListedValue();
	virtual ~ListedValue();

private:
	std::wstring label;
	std::wstring definition;
	IntegerCode code;
	std::wstring remarks; //�����ϴ�.
	std::list<std::wstring> alias; //�����ϴ�.
	DefinitionReference definitionReference; //�����ϴ�.

public:
	//void GetContents(MSXML2::IXMLDOMNodePtr pNode);
	void GetContents(pugi::xml_node& node);

	const std::wstring& GetLabel();
	void SetLabel(std::wstring& value);

	const std::wstring& GetDefinition();
	void SetDefinition(std::wstring& value);

	const std::wstring& GetRemarks(); //�����ϴ�.
	void SetRemarks(std::wstring& value);

	IntegerCode& GetCode();

	std::list<std::wstring>& GetAliasPointer(); //�����ϴ�.

	DefinitionReference& GetDefinitionReferencePointer(); //�����ϴ�.
};