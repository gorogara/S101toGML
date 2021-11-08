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
	std::wstring remarks; //없습니다.
	std::list<std::wstring> alias; //없습니다.
	DefinitionReference definitionReference; //없습니다.

public:
	//void GetContents(MSXML2::IXMLDOMNodePtr pNode);
	void GetContents(pugi::xml_node& node);

	const std::wstring& GetLabel();
	void SetLabel(std::wstring& value);

	const std::wstring& GetDefinition();
	void SetDefinition(std::wstring& value);

	const std::wstring& GetRemarks(); //없습니다.
	void SetRemarks(std::wstring& value);

	IntegerCode& GetCode();

	std::list<std::wstring>& GetAliasPointer(); //없습니다.

	DefinitionReference& GetDefinitionReferencePointer(); //없습니다.
};