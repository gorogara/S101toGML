#include "pch.h"
#include "ListedValue.h"


ListedValue::ListedValue()
{
}


ListedValue::~ListedValue()
{
}

//void ListedValue::GetContents(MSXML2::IXMLDOMNodePtr pNode)
//{
//	USES_CONVERSION;
//
//	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();
//
//	int cnt = pNodeList->Getlength();
//	for (int i = 0; i < cnt; i++) {
//		MSXML2::IXMLDOMNodePtr pChildNode = pNodeList->Getitem(i);
//		if (pChildNode == NULL)
//			continue;
//
//		if (!pChildNode->baseName)
//			continue;
//
//		std::string baseName = std::string(W2A(pChildNode->baseName));
//
//		if (baseName.compare("label") == 0)
//		{
//			label = pChildNode->Gettext();
//		}
//		else if (baseName.compare("definition") == 0)
//		{
//			definition = pChildNode->Gettext();
//		}
//		else if (baseName.compare("code") == 0)
//		{
//			code.GetContents(pChildNode);
//		}
//		else if (baseName.compare("remarks") == 0)
//		{
//			remarks = pChildNode->Gettext();
//		}
//		else if (baseName.compare("alias") == 0)
//		{
//			alias.push_back(std::wstring(pChildNode->Gettext()));
//		}
//		else if (baseName.compare("definitionReference") == 0)
//		{
//			definitionReference.GetContents(pChildNode);
//		}
//	}
//}
void ListedValue::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:label"))
		{
			label = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:definition"))
		{
			definition = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:code"))
		{
			code.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:remarks"))
		{
			remarks = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:alias"))
		{
			alias.push_back(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "S100FC:definitionReference"))
		{
			definitionReference.GetContents(instruction);
		}
	}
}

const std::wstring& ListedValue::GetLabel()
{
	return label;
}

void ListedValue::SetLabel(std::wstring& value)
{
	label = value;
}

const std::wstring& ListedValue::GetDefinition()
{
	return definition;
}

void ListedValue::SetDefinition(std::wstring& value)
{
	definition = value;
}

IntegerCode& ListedValue::GetCode()
{
	return code;
}

const std::wstring& ListedValue::GetRemarks()
{
	return remarks;
}

void ListedValue::SetRemarks(std::wstring& value)
{
	remarks = value;
}

std::list<std::wstring>& ListedValue::GetAliasPointer()
{
	return alias;
}

DefinitionReference& ListedValue::GetDefinitionReferencePointer()
{
	return definitionReference;
}