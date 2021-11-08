#pragma once

//#ifndef __AFXWIN_H__
//	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
//#endif

#include "ResponsibleParty.h"
#include "DefinitionSources.h"
#include "SimpleAttributes.h"
#include "ComplexAttributes.h"
#include "Roles.h"
#include "InformationAssociations.h"
//#include "FeatureAssociations.h"
#include "InformationTypes.h"
#include "FeatureTypes.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class SimpleAttribute;
class ComplexAttribute;
class FeatureType;
class InformationType;

// S100_FC_FeatureCatalogue
class FeatureCatalogue
{
public:
	FeatureCatalogue();
	virtual ~FeatureCatalogue();

public:
	// extra tag

private:
	// std::wstring product = L""; //모든데이터에 공통으로 있는값이 아닙니다.(회의필요)(있을수도있고 없을수도있습니다)

private:
	std::wstring name = L"";
	std::wstring scope = L"";

private:
	std::wstring *fieldOfApplication = nullptr;  // 올수도 있고 안올수 있음

private:
	std::wstring versionNumber = L"";
	std::wstring versionDate = L""; //DATE 시간 관리 타입은 나중에 수정
	
private:
	// CI_Responsibility (보류)
	ResponsibleParty producer; 

	// FC_DefinitionSource (보류)
	DefinitionSources definitionSources; // 없습니다.

	// MD_ClassificationCode (보류)
	
private:
	SimpleAttributes simpleAttributes;
	ComplexAttributes complexAttributes;
	Roles roles;
	InformationAssociations informationAssociations;
	FeatureAssociations featureAssociations;
	InformationTypes informationTypes;
	FeatureTypes featureTypes; 

public:
	/* 정보 검색 */
	//void GetContents(MSXML2::IXMLDOMNodePtr pNode);
	void GetContents(pugi::xml_node& node);

	const std::wstring& GetName();
	void SetName(std::string value);
	void SetName(std::wstring value);
	
	const std::wstring& GetScope();
	void SetScope(std::string value);
	void SetScope(std::wstring value);

	const std::wstring& GetFieldOfApplication();
	void SetFieldOfApplication(std::string& value);
	void SetFieldOfApplication(std::wstring& value);

	const std::wstring& GetVersionNumber();
	void SetVersionNumber(std::string value);
	void SetVersionNumber(std::wstring value);

	const std::wstring& GetVersionDate();
	void SetVersionDate(std::string value);
	void SetVersionDate(std::wstring value);

	//ResponsibleParty producer;
	//DefinitionSources definitionSources; // 없습니다.

	SimpleAttribute* GetSimpleAttribute(std::wstring Code);
	SimpleAttribute* GetSimpleAttributeFromName(std::wstring name);
	SimpleAttributes& GetSimpleAttributesPointer();
	
	ComplexAttribute* GetComplexAttribute(std::wstring Code);
	ComplexAttribute* GetComplexAttributeFromName(std::wstring name);
	ComplexAttributes& GetComplexAttributesPointer();

	Role* GetRole(std::wstring Code);
	Role* GetRoleFromName(std::wstring name);
	Roles& GetRolesPointer();

	InformationAssociation* GetInformationAssociation(std::wstring Code);
	InformationAssociation* GetInformationAssociationFromName(std::wstring name);
	InformationAssociations& GetInformationAssociationsPointer();

	FeatureAssociation* GetFeatureAssociation(std::wstring Code);
	FeatureAssociation* GetFeatureAssociationFromName(std::wstring name);
	FeatureAssociations& GetFeatureAssociationsPointer();

	InformationType* GetInformationType(std::wstring Code);
	InformationType* GetInformationTypeFromName(std::wstring name);
	InformationTypes& GetInformationTypesPointer();

	FeatureType* GetFeatureType(std::wstring Code);
	FeatureType* GetFeatureTypeName(std::wstring Name);
	FeatureType* GetFeatureTypeFromIndex(int indexnum);
	FeatureTypes& GetFeatureTypesPointer();

	void SetFullAssociations();
	void SetSubAssociation(FeatureType* ft);
	void SetSubAssociation(InformationType* it);
	void SetFeatureAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName);
	void SetInformationAssociationFromSuperType(FeatureType* ft, std::wstring superTypeName, std::wstring roleName, std::wstring associationName);
	void SetInformationAssociationFromSuperType(InformationType* it, std::wstring superTypeName, std::wstring roleName, std::wstring associationName);

	void AddFeatureBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName);
	void AddInformationBinding(FeatureType* ft, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName);
	void AddInformationBinding(InformationType* it, std::wstring bindedTypeName, std::wstring roleName, std::wstring associationName);

	void NullCheckFieldOfApplication();
	
	
};