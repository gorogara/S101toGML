#include "pch.h"
#include "SimpleAttributes.h"


SimpleAttributes::SimpleAttributes(void)
{
	//simpleAttribute = NULL;

}

SimpleAttributes::~SimpleAttributes(void)
{
	for (auto i = simpleAttribute.begin(); i != simpleAttribute.end(); i++)
	{
		delete i->second;
	}
}


//void SimpleAttributes::GetContents(MSXML2::IXMLDOMNodePtr pNode)
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
//		bstr_t bstr = pChildNode->baseName;
//		if (!!bstr)
//		{
//			std::string baseName = std::string(bstr);
//
//			if (baseName.compare("S100_FC_SimpleAttribute") == 0)
//			{
//				auto sa = new SimpleAttribute();
//				sa->GetContents(pChildNode);
//				simpleAttribute.insert({ sa->GetCodeAsWString(), sa });
//			}
//		}
//	}
//}
void SimpleAttributes::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_SimpleAttribute"))
		{
			auto sa = new SimpleAttribute();
			sa->GetContents(instruction);
			simpleAttribute.insert({ sa->GetCodeAsWString(), sa });
		}
	}

	//auto findNode = node.child("S100FC:S100_FC_SimpleAttribute");
	//while (findNode)
	//{
	//	SimpleAttribute sa;
	//	sa.GetContents(findNode);
	//	simpleAttribute[sa.code.valueString] = sa;
	//	findNode = findNode.next_sibling();
	//}
}


std::unordered_map<std::wstring, SimpleAttribute*>& SimpleAttributes::GetSimpleAttributePointer() {
	return simpleAttribute;
}
