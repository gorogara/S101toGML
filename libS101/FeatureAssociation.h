#pragma once

#include "NamedType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class FeatureAssociation : public NamedType
{
public:
	FeatureAssociation();
	virtual ~FeatureAssociation();

private:
	std::wstring superType;
	std::list<std::wstring> subType;
	Reference role[2];

public:
	//void GetContents(MSXML2::IXMLDOMNodePtr pNode);
	void GetContents(pugi::xml_node& node);

	std::list<std::wstring> &GetSubTypePointer();
	std::wstring& GetSuperType();

	Reference(&GetRolePointer())[2];
};

