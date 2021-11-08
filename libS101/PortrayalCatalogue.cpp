#include "pch.h"
#include "PortrayalCatalogue.h"
#include "S100_PointInstruction.h"
#include "S100_LineInstruction.h"
#include "S100_AreaInstruction.h"
#include "S100_TextInstruction.h"
#include "S100_ColorFill.h"
#include "S100_ColorProfiles.h"

#include "..\\DLL_MessageProcess\\DLL_MessageProcess.h"
#include "..\\S100_SVG_D2D1_DLL\\UnitOfCSS.h"
#include "..\\S100_SVG_D2D1_DLL\\CSSStyle.h"
#include "..\\S100_SVG_D2D1_DLL\\CSSStroke.h"
#include "..\\S100_SVG_D2D1_DLL\\SVGManager.h"
#include "..\\S100_SVG_D2D1_DLL\\CSS.h"
#include "..\\S100Engine\\S100PCManager.h"

#include <vector>
#include <string>

PortrayalCatalogue::PortrayalCatalogue()
{

}

PortrayalCatalogue::PortrayalCatalogue(std::wstring path) : PortrayalCatalogue()
{
	Open(path);
}


PortrayalCatalogue::~PortrayalCatalogue()
{
	Delete();
}


void PortrayalCatalogue::Open(std::wstring& path)
{
	ReadPortrayalCatalogueByPugiXML(path);

	m_svgSymbolManager = new S100_SVG_D2D1_DLL::SVGManager(path, GetCurrentPaletteName(), colorProfiles.GetColorProfile());

	s100PCManager->OpenS100Symbol(path + L"\\Symbols\\*.svg");
	s100PCManager->OpenS100ColorProfile(path + L"ColorProfiles\\" + GetColorProfile()->GetfileName());
	s100PCManager->CreateSVGGeometry(m_svgSymbolManager->GetD2Factory());
	s100PCManager->OpenS100AreaFills(path + L"AreaFills\\*.xml");
	s100PCManager->OpenS100LineStyles(path + L"LineStyles");
}


void PortrayalCatalogue::Init()
{
	s100PCManager = new S100PCManager();
}


void PortrayalCatalogue::Delete()
{
	for (auto i = s100_lineStyles.begin(); i != s100_lineStyles.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	s100_lineStyles.clear();

	for (auto i = s100_symbolFill.begin(); i != s100_symbolFill.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	s100_symbolFill.clear();

	for (auto i = s100_transform.begin(); i != s100_transform.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
	s100_transform.clear();

	//for (auto i = mainRules.begin(); i != mainRules.end(); i++)
	//{
	//	delete i->second;
	//	i->second = nullptr;
	//}
	//mainRules.clear();

	delete m_svgSymbolManager;
	m_svgSymbolManager = nullptr;

	delete s100PCManager;
	s100PCManager = nullptr;
}

bool PortrayalCatalogue::ReadPortrayalCatalogueByPugiXML(std::wstring& path)
{
	CoInitialize(nullptr);

	KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC data Reading start", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::PC);
	std::wstring PCPath = path + _T("portrayal_catalogue.xml");

	if (!PCPath.empty())
	{
		auto itor = PCPath.find_last_of('\\');
		rootPath = PCPath.substr(0, ++itor);

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(PCPath.c_str()); ///파일을 읽습니다.
		pugi::xml_node displayList = doc.child("portrayalCatalog");

		auto productId = displayList.attribute("productId");
		if (productId)
		{
			product = pugi::as_wide(productId.value());
		}

		auto versionId = displayList.attribute("version");
		if (versionId)
		{
			version = pugi::as_wide(versionId.value());
		}

		GetContents(displayList);
	}
	else
	{
		KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_ERROR, "PC Datafile is empty", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::PC);
		return false;
	}

	GetLineStylesByPugiXml();
	GetMainRuleFile();

	if (colorProfiles.ReadColorProfiles(path) == false)
	{
		KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"PC Failed to read color profiles", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
	}

	return true;
}

void PortrayalCatalogue::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_ERROR, "PC: reading Data is null", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::PC);
		return;
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "pixmaps"))
		{
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"PC Node name is pixmaps", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}
		else if (!strcmp(instructionName, "colorProfiles")) //색상 읽는곳
		{
			colorProfiles.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, L"PC Node name is colorProfiles", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}
		else if (!strcmp(instructionName, "symbols"))
		{
			//심볼을 읽지 않습니다.
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is symbols", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}

		else if (!strcmp(instructionName, "lineStyles"))
		{
			lineStyles.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is lineStyles", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}

		else if (!strcmp(instructionName, "areaFills"))
		{
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is areaFills", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
			areaFills.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "fonts"))
		{
			//내용이없습니다
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is fonts", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}

		else if (!strcmp(instructionName, "viewingGroups"))
		{
			//내용을 읽지않습니다.
			viewingGroups.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is viewingGroups", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}

		else if (!strcmp(instructionName, "foundationMode"))
		{
			foundationMode.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is foundationMode", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}

		else if (!strcmp(instructionName, "viewingGroupLayers"))
		{
			viewingGroupLayers.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is viewingGroupLayers", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}

		else if (!strcmp(instructionName, "displayModes"))
		{
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is displayModes", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}

		else if (!strcmp(instructionName, "displayPlane"))
		{
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is displayPlane", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}

		else if (!strcmp(instructionName, "context"))
		{
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node name is context", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}

		else if (!strcmp(instructionName, "rules"))
		{
			rules.GetContents(instruction);
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC Node rules Setting Clear", KRS_MSG_PROCESS::Developer_Mode, KRS_MSG_PROCESS::PC);
		}
		else
		{
			std::string value(instructionName);
			value + ":없는 Context\n";

			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, value.c_str(), KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::PC);
		}
	}
}

void PortrayalCatalogue::SetRootPath(std::wstring& value)
{
	rootPath = value;
}

void PortrayalCatalogue::SetPortrayalRuleType(PortrayalRuleType value)
{
	portrayalRuleType = value;
}
void PortrayalCatalogue::SetProduct(std::wstring& value)
{
	product = value;
}
void PortrayalCatalogue::SetVersion(std::wstring& value)
{
	version = value;
}
void PortrayalCatalogue::SetRules(S100_Rules* value)
{
	rules = *value;
}
void PortrayalCatalogue::SetFoundationMode(FoundationMode* value)
{
	foundationMode = *value;
}

void PortrayalCatalogue::SetViewingGroupLayers(ViewingGroupLayers* value)
{
	viewingGroupLayers = *value;
}

void PortrayalCatalogue::SetVewingGroups(ViewingGroups* value)
{
	viewingGroups = *value;
}

void PortrayalCatalogue::SetDisplaModes(DisplayModes* value)
{
	displayModes = *value;
}

void PortrayalCatalogue::SetDisplayPlanes(DisplayPlanes* value)
{
	displayPlanes = *value;
}

void PortrayalCatalogue::SetContext(Context* value)
{
	context = *value;
}

void PortrayalCatalogue::SetColorProfiles(S100_ColorProfiles* value)
{
	colorProfiles = *value;
}

void PortrayalCatalogue::SetAreaFills(S100_AreaFills* value)
{
	areaFills = *value;
}

void PortrayalCatalogue::SetColorProfile(S100_ColorProfile* value)
{
	s100_colorProfile = *value;
}

void PortrayalCatalogue::SetLineStyles(std::wstring& key, S100_LineStyleBase* value)
{
	s100_lineStyles.insert({ key, value });
}

void PortrayalCatalogue::SetSymbolFill(std::wstring& key, S100_SymbolFill* value)
{
	s100_symbolFill.insert({ key,value });
}

void PortrayalCatalogue::SetTransform(S100_Transform* value)
{
	s100_transform.push_back(value);
}

void PortrayalCatalogue::SetS100PCManager(S100PCManager* value)
{
	s100PCManager = value;
}

void PortrayalCatalogue::SetS100Render(S100Render* value)
{
	s100Render = *value;
}

std::wstring PortrayalCatalogue::GetRootPath()
{
	return rootPath;
}

PortrayalRuleType PortrayalCatalogue::GetPortrayalRuleType() 
{
	return portrayalRuleType;
}

std::wstring PortrayalCatalogue::GetProduct() 
{
	return product;
}

std::wstring PortrayalCatalogue::GetVersion()
{
	return version;
}

S100_Rules* PortrayalCatalogue::GetRules() 
{
	return &rules;
}

ViewingGroups* PortrayalCatalogue::GetViewingGroups()
{
	return &viewingGroups;
}

FoundationMode* PortrayalCatalogue::GetFoundationMode()
{
	return &foundationMode;
}

ViewingGroupLayers* PortrayalCatalogue::GetViewingGroupLayers()
{
	return &viewingGroupLayers;
}

DisplayModes* PortrayalCatalogue::GetDisplaModes() 
{
	return &displayModes;
}

DisplayPlanes* PortrayalCatalogue::GetDisplayPlanes() 
{
	return &displayPlanes;
}

Context* PortrayalCatalogue::GetContext() 
{
	return &context;
}

S100_ColorProfiles* PortrayalCatalogue::GetColorProfiles() 
{
	return &colorProfiles;
}

S100_LineStyleBase* PortrayalCatalogue::GetLineStyles(std::wstring& key) 
{
	return s100_lineStyles[key];
}

std::unordered_map<std::wstring, S100_LineStyleBase*> PortrayalCatalogue::GetLineStyles()
{
	return s100_lineStyles;
}

S100_SymbolFill* PortrayalCatalogue::GetSymbolFill(std::wstring& key)
{
	return s100_symbolFill[key];
}

std::unordered_map<std::wstring, S100_SymbolFill*> PortrayalCatalogue::GetSymbolFill()
{
	return s100_symbolFill;
}

S100_Transform* PortrayalCatalogue::GetTransform(int index)
{
	return s100_transform.at(index);
}

std::vector<S100_Transform*> PortrayalCatalogue::GetTransform()
{
	return s100_transform;
}

S100_RuleFile* PortrayalCatalogue::GetMainRules(std::wstring& key)
{
	return mainRules[key];
}

std::unordered_map<std::wstring, S100_RuleFile*> PortrayalCatalogue::GetMainRules()
{
	return mainRules;
}

S100Render PortrayalCatalogue::GetS100Render()
{
	return s100Render;
}

S100_ColorProfile* PortrayalCatalogue::GetColorProfile()
{
	auto colorpro = colorProfiles.GetColorProfiles();
	if (colorpro.begin() == colorpro.end())
	{
		return nullptr;
	}
	auto result = colorpro.begin()->second;
	return result;
}

S100_ColorProfile* PortrayalCatalogue::GetColorProfile(std::wstring& id)
{
	auto colorpro = colorProfiles.GetColorProfiles();
	if (colorpro.find(id) == colorpro.end())
	{
		return nullptr;
	}

	auto result = colorpro[id];
	return result;
}

void PortrayalCatalogue::GetLineStylesByPugiXml()
{
	auto lineStyleFiles = lineStyles.GetLineStyleFiles();

	for (auto itor = lineStyleFiles.begin(); itor != lineStyleFiles.end(); itor++)
	{
		S100_LineStyleFile *lineStyleFile = itor->second;
		std::wstring path = lineStyleFile->GetFileName();

		std::wstring head = rootPath + L"LineStyles\\";
		path.insert(path.begin(), head.begin(), head.end());

		if (!path.empty())
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(path.c_str()); ///파일을 읽습니다.

			if (!result)
			{
				KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "PC VARIANT_BOOL is null ", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::PC);
				continue;
			}

			for (pugi::xml_node instruction = doc.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "lineStyle"))
				{
					S100_LineStyle* lineStyle = new S100_LineStyle();
					lineStyle->GetContents(instruction);
					s100_lineStyles[lineStyleFile->GetDescription()->Getname()] = lineStyle;
				}
				else if (!strcmp(instructionName, "compositeLineStyle"))
				{
					S100_CompositeLineStyle* cls = new S100_CompositeLineStyle();
					cls->GetContents(instruction);
					s100_lineStyles[lineStyleFile->GetDescription()->Getname()] = cls;
				}
				else
				{
					std::string unValue(instructionName);
					unValue + "is unValue Context";
					KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, unValue.c_str(), KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::PC);
				}
			}
		}
	}
}

void PortrayalCatalogue::GetAreaFills()
{
	auto areaFillFiles = areaFills.GetAreaFillFiles();
	for (auto itor = areaFillFiles.begin(); itor != areaFillFiles.end(); itor++)
	{
		S100_AreaFillFile *areaFillFile = itor->second;

		std::wstring path = areaFillFile->GetFileName();

		S100_SymbolFill* areaFill = new S100_SymbolFill();

		if (!areaFill->ReadFile(rootPath + L"AreaFills\\" + path))
		{
			delete areaFill;
			KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_WARNING, "areaFills read fail", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::PC);
			continue;
		}

		s100_symbolFill.insert({ areaFillFile->GetDescription()->Getname(), areaFill });
		KRS_MSG_PROCESS::SendMessageToTargetWindow(KRS_MSG_INFO, "areaFill add", KRS_MSG_PROCESS::User_Developer_Mode, KRS_MSG_PROCESS::PC);
	}
}

void PortrayalCatalogue::GetMainRuleFile()
{
	auto ruleFiles = rules.GetRuleFiles();
	for (auto itor = ruleFiles.begin(); itor != ruleFiles.end(); itor++) //여기서 메인룰을 추가해줍니다.
	{
		//main.lua파일을 불러옵니다.
		S100_RuleFile* rf = itor->second;

		if (true == rf->IsTypeLevelTemplate())
		{
			mainRules[rf->GetFileName()] = rf;
			if (true == rf->IsLua())
			{
				SetRuleType(LUA);
			}
		}
	}
}

std::wstring PortrayalCatalogue::GetCurrentPaletteName()
{
	return currentPalette;
}

void PortrayalCatalogue::SetCurrentPaletteName(std::wstring& paletteName)
{
	currentPalette = paletteName;

	if (nullptr != s100PCManager)
	{
		s100PCManager->GetS100ColorProfile()->ChangePalette(paletteName);
	}
}

void PortrayalCatalogue::SetCurrentPalette(GeoMetryLibrary::ColorTable value)
{
	if (value == GeoMetryLibrary::ColorTable::Day)
	{
		std::wstring value = L"Day";
		SetCurrentPaletteName(value);
	}
	else if (value == GeoMetryLibrary::ColorTable::Dusk)
	{
		std::wstring value = L"Dusk";
		SetCurrentPaletteName(value);
	}
	else if (value == GeoMetryLibrary::ColorTable::Night)
	{
		std::wstring value = L"Night";
		SetCurrentPaletteName(value);
	}
}

S100PCManager* PortrayalCatalogue::GetS100PCManager()
{
	return s100PCManager;
}

void PortrayalCatalogue::SetRuleType(PortrayalRuleType value)
{
	portrayalRuleType = value;
}

PortrayalRuleType PortrayalCatalogue::GetRuleType()
{
	return portrayalRuleType;
}

void PortrayalCatalogue::SetSVGManager(S100_SVG_D2D1_DLL::SVGManager* value)
{
	m_svgSymbolManager = value;
}

S100_SVG_D2D1_DLL::SVGManager* PortrayalCatalogue::GetSVGManager()
{
	return m_svgSymbolManager;
}

void PortrayalCatalogue::CreatePatternImages(ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke)
{
	if (nullptr != s100PCManager)
	{
		s100PCManager->CreateBitmapImage(d2Factory, imageFactory, stroke, GetCurrentPaletteName());
	}
}

void PortrayalCatalogue::DeletePatternImage()
{
	if (nullptr != s100PCManager)
	{
		s100PCManager->DeleteBitmapImage();
	}
}

void PortrayalCatalogue::CreateLineImages(ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke)
{
	if (nullptr != s100PCManager)
	{
		s100PCManager->CreateLineImage(d2Factory, imageFactory, stroke, GetCurrentPaletteName());
	}
}

void PortrayalCatalogue::DeleteLineImages()
{
	if (nullptr != s100PCManager)
	{
		s100PCManager->DeleteLineImage();
	}
}