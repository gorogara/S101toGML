// FeatureCatalog.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "pch.h"
#include "FeatureCatalogue.h"
#include "FeatureAssociation.h"
#include "Role.h"

//#include "..\\DLL_MessageProcess\\DLL_MessageProcess.h"

#include <atlstr.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

FeatureCatalogue::FeatureCatalogue()
{
	//producer = NULL;
}
FeatureCatalogue::~FeatureCatalogue()
{
	delete fieldOfApplication;
	fieldOfApplication = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 정보  검색 */

SimpleAttribute* FeatureCatalogue::GetSimpleAttribute(std::wstring code)
{
	auto itor = simpleAttributes.GetSimpleAttributePointer().find(code);

	if (itor == simpleAttributes.GetSimpleAttributePointer().end())
	{
		return nullptr; // 검색해서 못찾을때
	}
	else
	{
		return itor->second;
	}
}
SimpleAttribute* FeatureCatalogue::GetSimpleAttributeFromName(std::wstring name)
{
	for (auto i = simpleAttributes.GetSimpleAttributePointer().begin();
		i != simpleAttributes.GetSimpleAttributePointer().end();
		i++)
	{
		if (i->second->GetName().compare(name) == 0)
		{
			return i->second;
		}
	}

	return nullptr;

}

ComplexAttribute* FeatureCatalogue::GetComplexAttribute(std::wstring code)
{
	auto itor = complexAttributes.GetComplexAttributePointer().find(code);

	if (itor == complexAttributes.GetComplexAttributePointer().end())
		return NULL; // 검색해서 못찾을때
	else
		return &itor->second;
}

ComplexAttribute* FeatureCatalogue::GetComplexAttributeFromName(std::wstring name)
{
	for (auto i = complexAttributes.GetComplexAttributePointer().begin();
		i != complexAttributes.GetComplexAttributePointer().end();
		i++)
	{
		if (i->second.GetName().compare(name) == 0)
			return &i->second;
	}

	return nullptr;
}

FeatureType* FeatureCatalogue::GetFeatureType(std::wstring code)
{
	auto itor = featureTypes.GetFeatureTypePointer().find(code);

	if (itor == featureTypes.GetFeatureTypePointer().end())
		return NULL; // 검색해서 못찾을때
	else
		return &itor->second;
}

FeatureTypes& FeatureCatalogue::GetFeatureTypesPointer()
{
	return featureTypes;

}

FeatureType* FeatureCatalogue::GetFeatureTypeName(std::wstring name)
{
	for (auto i = featureTypes.GetFeatureTypePointer().begin();
		i != featureTypes.GetFeatureTypePointer().end();
		i++)
	{
		if (i->second.GetName().compare(name) == 0)
			return &i->second;
	}

	return nullptr;

	//auto itor = featureTypes.GetFeatureTypePointer().find(code);

	//if (itor == featureTypes.GetFeatureTypePointer().end())
	//	return nullptr; // 검색해서 못찾을때
	//else
	//	return &itor->second;
}


FeatureType* FeatureCatalogue::GetFeatureTypeFromIndex(int indexnum)
{
	//auto itor = featureTypes.GetFeatureTypePointer().find(code);
	int num = 0;
	for (auto i= featureTypes.GetFeatureTypePointer().begin(); i!=featureTypes.GetFeatureTypePointer().end(); i++)
	{
		if (num==indexnum)
		{
			auto dfsf = &i->second;
			OutputDebugString(L"");
			return &i->second;
		}
		
		num++;
	}


	return nullptr;

	//if (itor == featureTypes.GetFeatureTypePointer().end())
	//	return NULL; // 검색해서 못찾을때
	//else
	//	return &itor->second;
}

InformationType* FeatureCatalogue::GetInformationType(std::wstring code)
{
	auto itor = informationTypes.GetInformationTypePointer().find(code);

	if (itor == informationTypes.GetInformationTypePointer().end())
		return NULL; // 검색해서 못찾을때
	else
		return &itor->second;
}


InformationType* FeatureCatalogue::GetInformationTypeFromName(std::wstring name)
{
	for (auto itor = informationTypes.GetInformationTypePointer().begin();
		itor != informationTypes.GetInformationTypePointer().end();
		itor++)
	{
		if (itor->second.GetName().compare(name) == 0)
			return &itor->second;
	}

	return NULL;
}

FeatureAssociation* FeatureCatalogue::GetFeatureAssociation(std::wstring code)
{
	auto itor = featureAssociations.GetFeatureAssociationPointer().find(code);

	if (itor == featureAssociations.GetFeatureAssociationPointer().end())
		return NULL; // 검색해서 못찾을때
	else
		return &itor->second;
}

FeatureAssociation* FeatureCatalogue::GetFeatureAssociationFromName(std::wstring name)
{
	for (auto itor = featureAssociations.GetFeatureAssociationPointer().begin();
		itor != featureAssociations.GetFeatureAssociationPointer().end();
		itor++)
	{
		if (itor->second.GetName().compare(name) == 0)
			return &itor->second;
	}

	return NULL;
}

InformationAssociation* FeatureCatalogue::GetInformationAssociation(std::wstring code)
{
	auto itor = informationAssociations.GetInformationAssociationPointer().find(code);

	if (itor == informationAssociations.GetInformationAssociationPointer().end())
		return NULL; // 검색해서 못찾을때
	else
		return &itor->second;
}

InformationAssociation* FeatureCatalogue::GetInformationAssociationFromName(std::wstring name)
{
	for (auto itor = informationAssociations.GetInformationAssociationPointer().begin();
		itor != informationAssociations.GetInformationAssociationPointer().end();
		itor++)
	{
		if (itor->second.GetName().compare(name) == 0)
			return &itor->second;
	}

	return NULL;
}

Role* FeatureCatalogue::GetRole(std::wstring code)
{
	auto itor = roles.GetRolePointer().find(code);

	if (itor == roles.GetRolePointer().end())
		return NULL; // 검색해서 못찾을때
	else
		return itor->second;
}

Role* FeatureCatalogue::GetRoleFromName(std::wstring name)
{
	for (auto itor = roles.GetRolePointer().begin();
		itor != roles.GetRolePointer().end();
		itor++)
	{
		if (itor->second->GetName().compare(name) == 0)
			return itor->second;
	}

	return NULL;
}

//void FeatureCatalogue::GetContents(MSXML2::IXMLDOMNodePtr pNode)
//{
//	USES_CONVERSION;
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
//		std::string str = std::string(W2A(pChildNode->baseName));
//
//		if (str.compare("name") == 0)
//		{
//			name = pChildNode->Gettext();
//		}
//		//else if (str.compare("product") == 0)
//		//{
//		//	product = pChildNode->Gettext();
//		//}
//		else if (str.compare("scope") == 0)
//		{
//			scope = pChildNode->Gettext();
//		}
//		else if (str.compare("fieldOfApplication") == 0)
//		{
//			//fieldOfApplication = pChildNode->Gettext();
//			std::wstring value = pChildNode->Gettext();
//			SetFieldOfApplication(value);
//		}
//		else if (str.compare("versionNumber") == 0)
//		{
//			versionNumber = pChildNode->Gettext();
//		}
//		else if (str.compare("versionDate") == 0)
//		{
//			versionDate = pChildNode->Gettext();
//		}
//		else if (str.compare("producer") == 0)
//		{
//			producer.GetContents(pChildNode);
//		}
//		else if (str.compare("S100_FC_DefinitionSources") == 0)
//		{
//			definitionSources.GetContents(pChildNode);
//		}
//		else if (str.compare("S100_FC_SimpleAttributes") == 0)
//		{
//			simpleAttributes.GetContents(pChildNode);
//		}
//		else if (str.compare("S100_FC_ComplexAttributes") == 0)
//		{
//			complexAttributes.GetContents(pChildNode);
//		}
//		else if (str.compare("S100_FC_Roles") == 0)
//		{
//			roles.GetContents(pChildNode);
//		}
//		else if (str.compare("S100_FC_InformationAssociations") == 0)
//		{
//			informationAssociations.GetContents(pChildNode);
//		}
//		else if (str.compare("S100_FC_FeatureAssociations") == 0)
//		{
//			featureAssociations.GetContents(pChildNode);
//		}
//		else if (str.compare("S100_FC_InformationTypes") == 0)
//		{
//			informationTypes.GetContents(pChildNode);
//		}
//		else if (str.compare("S100_FC_FeatureTypes") == 0)
//		{
//			featureTypes.GetContents(pChildNode);
//		}
//	}
//	OutputDebugString(L"");
//}
void FeatureCatalogue::GetContents(pugi::xml_node& node)
{
	//수정예정 : 요소가 없는지 검사하고 내용추가
	//수정예정:맵에 값을 넣고 검색한다음에 있으면 불러오고 없으면 오류 메세지 출력으로 수정예정
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100FC:name"))
		{
			if (instruction.child_value() != nullptr) 
			{
				name = pugi::as_wide(instruction.child_value());
			}
			else
			{
				KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_WARNING, L"S100FC:name value is null",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
			}
		}
		else if (!strcmp(instructionName, "S100FC:scope"))
		{
			if (instruction.child_value() != nullptr)
			{
				scope = pugi::as_wide(instruction.child_value());
			}
			else
			{
				KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:scope value is null",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
			}
		}
		else if (!strcmp(instructionName, "S100FC:fieldOfApplication"))
		{
			if (instruction.child_value() != nullptr)
			{
				std::string value = instruction.child_value();
				SetFieldOfApplication(value);
			}
			else
			{
				KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_WARNING, L"S100FC:fieldOfApplication value is null",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
			}
		} 
		else if (!strcmp(instructionName, "S100FC:versionNumber"))
		{
			if (instruction.child_value() != nullptr)
			{
				versionNumber = pugi::as_wide(instruction.child_value());
			}
			else
			{
				KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_WARNING, L"S100FC:versionNumber value is null",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
			}
		}
		else if (!strcmp(instructionName, "S100FC:versionDate"))
		{
			if (instruction.child_value() != nullptr)
			{
				versionDate = pugi::as_wide(instruction.child_value());
			}
			else
			{
				KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_WARNING, L"S100FC:versionDate value is null",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
			}
		}
		else if (!strcmp(instructionName, "S100FC:producer"))
		{
			producer.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:producer setting Clear",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_definitionSources"))
		{
			definitionSources.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:S100_FC_definitionSources setting Clear",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_SimpleAttributes"))
		{
			simpleAttributes.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:S100_FC_SimpleAttributes setting Clear",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_ComplexAttributes"))
		{
			complexAttributes.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:S100_FC_ComplexAttributes setting Clear",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_Roles"))
		{
			roles.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:S100_FC_Roles setting Clear",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_InformationAssociations"))
		{
			informationAssociations.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:S100_FC_InformationAssociations setting Clear",KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::FC);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_FeatureAssociations"))
		{
			featureAssociations.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:S100_FC_FeatureAssociations setting Clear",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_InformationTypes"))
		{
			informationTypes.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:S100_FC_InformationTypes setting Clear",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_FeatureTypes"))
		{
			featureTypes.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"S100FC:S100_FC_FeatureTypes setting Clear",KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		}
		else  //지정되지않은 값이 있는경우 
		{
			std::wstring anotherName = pugi::as_wide(instructionName);
			anotherName.append(L"is another FCDateName");
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, anotherName.c_str(),KRS_MSG_PROCESS::Developer_Mode,KRS_MSG_PROCESS::FC);
		}
	}

	// 모든 정보를 가진 후 Association 의 상속관계를 재계산해줌
	SetFullAssociations();
}
void FeatureCatalogue::SetFullAssociations()
{
	// Relation이 있는 Feature 를 Super Type 으로 가지는 객체와 연관 관계를 추가로 작성해줌
	
	for (auto itor = featureTypes.GetFeatureTypePointer().begin(); itor != featureTypes.GetFeatureTypePointer().end(); itor++)
	{
		FeatureType *ft = &itor->second;
		SetSubAssociation(ft);
	}

	for (auto itor = informationTypes.GetInformationTypePointer().begin(); itor != informationTypes.GetInformationTypePointer().end(); itor++)
	{
		InformationType *it = &itor->second;
		SetSubAssociation(it);
	}

}

void FeatureCatalogue::SetSubAssociation(FeatureType* ft)
{
	for (auto fbi = ft->GetFeatureBindingPointer().begin();
		fbi != ft->GetFeatureBindingPointer().end();
		fbi++)
	{
		auto fb = &fbi->second;
		auto bindedFeatureName = fb->GetFeatureTypePointer().GetReference();
		// 모든 객체 중 해당 객체를 Super Type으로 가지는 객체를 찾아서 추가 해줌
		SetFeatureAssociationFromSuperType(ft, bindedFeatureName, fb->GetRolePointer().GetReference(), fb->GetAssociationPointer().GetReference());
	}

	for (auto ibi = ft->GetInformationBindingPointer().begin();
		ibi != ft->GetInformationBindingPointer().end();
		ibi++)
	{
		auto ib = &ibi->second;
		auto bindedinformationName = ib->GetInformationTypePointer().GetReference();
		// 모든 객체 중 해당 객체를 Super Type으로 가지는 객체를 찾아서 추가 해줌
		SetInformationAssociationFromSuperType(ft, bindedinformationName, ib->GetRolePointer().GetReference(), ib->GetAssociationPointer().GetReference());
	}

	return;
}

void FeatureCatalogue::SetSubAssociation(InformationType* it)
{
	for (auto ibi = it->GetInformationBindingPointer().begin();
		ibi != it->GetInformationBindingPointer().end();
		ibi++)
	{
		auto ib = &ibi->second;
		auto bindedinformationName = ib->GetInformationTypePointer().GetReference();
		// 모든 객체 중 해당 객체를 Super Type으로 가지는 객체를 찾아서 추가 해줌
		SetInformationAssociationFromSuperType(it, bindedinformationName, ib->GetRolePointer().GetReference(), ib->GetAssociationPointer().GetReference());
	}

	return;
}


void FeatureCatalogue::SetFeatureAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName)
{
	for (auto fti = featureTypes.GetFeatureTypePointer().begin(); fti != featureTypes.GetFeatureTypePointer().end(); fti++)
	{
		// 모든 객체 중 해당 객체를 Super Type으로 가지는 객체를 찾아서 추가 해줌
		auto currentFeautreType = &fti->second;
		//auto currentFeautreCode = currentFeautreType->code.GetValueString();
		auto currentFeautreCode = currentFeautreType->GetCodeAsWString();

		if (superTypeName.compare(currentFeautreCode) == 0)
			continue;

		auto duplicationCheckIter = ft->GetFeatureBindingPointer().find(currentFeautreCode);
		if (duplicationCheckIter != ft->GetFeatureBindingPointer().end())
			continue;;

		if (superTypeName.compare(currentFeautreType->GetSuperType()) == 0)
		{
			//AddFeatureBinding(ft, currentFeautreType->code.GetValueString(), roleName, associationName);
			AddFeatureBinding(ft, currentFeautreType->GetCodeAsWString(), roleName, associationName);

			if (currentFeautreType->GetSuperType().size() > 0)
			{
				SetFeatureAssociationFromSuperType(ft, currentFeautreType->GetSuperType(), roleName, associationName);
			}
		}
	}
	return;
}

void FeatureCatalogue::SetInformationAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName)
{
	for (auto iti = informationTypes.GetInformationTypePointer().begin(); iti != informationTypes.GetInformationTypePointer().end(); iti++)
	{
		// 모든 객체 중 해당 객체를 Super Type으로 가지는 객체를 찾아서 추가 해줌
		auto currentInformationType = &iti->second;
		//auto currentInformationCode = currentInformationType->code.GetValueString();
		auto currentInformationCode = currentInformationType->GetCodeAsWString();
		
		if (superTypeName.compare(currentInformationCode) == 0)
			continue;

		auto duplicationCheckIter = ft->GetInformationBindingPointer().find(currentInformationCode);
		if (duplicationCheckIter != ft->GetInformationBindingPointer().end())
			continue;;

		if (superTypeName.compare(currentInformationType->GetSuperType()) == 0)
		{
			//AddInformationBinding(ft, currentInformationType->code.GetValueString(), roleName, associationName);
			AddInformationBinding(ft, currentInformationType->GetCodeAsWString(), roleName, associationName);

			if (currentInformationType->GetSuperType().size() > 0)
			{
				SetInformationAssociationFromSuperType(ft, currentInformationType->GetSuperType(), roleName, associationName);
			}
		}
	}

	return;
}
void FeatureCatalogue::SetInformationAssociationFromSuperType(InformationType* it, std::wstring superTypeName, std::wstring roleName, std::wstring associationName)
{
	for (auto iti = informationTypes.GetInformationTypePointer().begin(); iti != informationTypes.GetInformationTypePointer().end(); iti++)
	{
		// 모든 객체 중 해당 객체를 Super Type으로 가지는 객체를 찾아서 추가 해줌
		auto currentInformationType = &iti->second;
		//auto currentInformationCode = currentInformationType->code.GetValueString();
		auto currentInformationCode = currentInformationType->GetCodeAsWString();

		if (superTypeName.compare(currentInformationCode) == 0)
			continue;

		auto duplicationCheckIter = it->GetInformationBindingPointer().find(currentInformationCode);
		if (duplicationCheckIter != it->GetInformationBindingPointer().end())
			continue;;

		if (superTypeName.compare(currentInformationType->GetSuperType()) == 0)
		{
			//AddInformationBinding(it, currentInformationType->code.GetValueString(), roleName, associationName);
			AddInformationBinding(it, currentInformationType->GetCodeAsWString(), roleName, associationName);

			if (currentInformationType->GetSuperType().size() > 0)
			{
				SetInformationAssociationFromSuperType(it, currentInformationType->GetSuperType(), roleName, associationName);
			}
		}
	}

	return;
}

void FeatureCatalogue::AddFeatureBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName)
{
	FeatureBinding fb;

	fb.GetFeatureTypePointer().SetReference(bindedTypeName);
	fb.GetRolePointer().SetReference(roleName);
	fb.GetAssociationPointer().SetReference(associationName);
	
	ft->GetFeatureBindingPointer().insert(
		std::unordered_map<std::wstring, FeatureBinding>::value_type(
			bindedTypeName,
			fb));
}

void FeatureCatalogue::AddInformationBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName)
{
	InformationBinding ib;
	ib.GetInformationTypePointer().SetReference(bindedTypeName);
	ib.GetRolePointer().SetReference(roleName);
	ib.GetAssociationPointer().SetReference(associationName);
	
	ft->GetInformationBindingPointer().insert(
		std::unordered_map<std::wstring, InformationBinding>::value_type(
			bindedTypeName,
			ib));
}

void FeatureCatalogue::AddInformationBinding(InformationType* it, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName)
{
	InformationBinding ib;
	ib.GetInformationTypePointer().SetReference(bindedTypeName);
	ib.GetRolePointer().SetReference(roleName);
	ib.GetAssociationPointer().SetReference(associationName);

	it->GetInformationBindingPointer().insert(
		std::unordered_map<std::wstring, InformationBinding>::value_type(
			bindedTypeName,
			ib));
}

void FeatureCatalogue::NullCheckFieldOfApplication()
{
	if (fieldOfApplication == nullptr)
	{
		fieldOfApplication = new std::wstring();
	}
}


//std::wstring FeatureCatalogue::GetFieldOfApplicationAsWString()
//{
//	if (nullptr != fieldOfApplication)
//	{
//		return *fieldOfApplication;
//	}
//
//	return L"";
//}

//std::string FeatureCatalogue::GetFieldOfApplication()
//{
//	if (nullptr != fieldOfApplication)
//	{
//		return pugi::as_utf8(*fieldOfApplication);
//	}
//
//	return "";
//}

InformationTypes& FeatureCatalogue::GetInformationTypesPointer()
{
	return informationTypes;
}

Roles& FeatureCatalogue::GetRolesPointer()
{
	return roles;
}


SimpleAttributes& FeatureCatalogue::GetSimpleAttributesPointer()
{
	return simpleAttributes;
}

ComplexAttributes& FeatureCatalogue::GetComplexAttributesPointer()
{
	return complexAttributes;
}

InformationAssociations& FeatureCatalogue::GetInformationAssociationsPointer()
{
	return informationAssociations;
}

FeatureAssociations& FeatureCatalogue::GetFeatureAssociationsPointer()
{
	return featureAssociations;
}


const std::wstring& FeatureCatalogue::GetName()
{
	return name;
}

void FeatureCatalogue::SetName(std::string value)
{
	name = pugi::as_wide(value);
}

void FeatureCatalogue::SetName(std::wstring value)
{
	name = value;
}

const std::wstring& FeatureCatalogue::GetScope()
{
	return scope;
}

void FeatureCatalogue::SetScope(std::string value)
{
	scope = pugi::as_wide(value);
}

void FeatureCatalogue::SetScope(std::wstring value)
{
	scope = value;
}

const std::wstring& FeatureCatalogue::GetFieldOfApplication()
{
	return *fieldOfApplication;
}

void FeatureCatalogue::SetFieldOfApplication(std::string& value)
{
	NullCheckFieldOfApplication();

	*fieldOfApplication = pugi::as_wide(value);
}

void FeatureCatalogue::SetFieldOfApplication(std::wstring& value)
{
	NullCheckFieldOfApplication();

	*fieldOfApplication = value;
}

const std::wstring& FeatureCatalogue::GetVersionNumber()
{
	return versionNumber;
}

void FeatureCatalogue::SetVersionNumber(std::string value)
{
	versionNumber = pugi::as_wide(value);
}

void FeatureCatalogue::SetVersionNumber(std::wstring value)
{
	versionNumber = value;
}

const std::wstring& FeatureCatalogue::GetVersionDate()
{
	return versionDate;
}

void FeatureCatalogue::SetVersionDate(std::string value)
{
	versionDate = pugi::as_wide(value);
}

void FeatureCatalogue::SetVersionDate(std::wstring value)
{
	versionDate = value;
}
