#pragma once
#include "S100_Rules.h"
#include "S100_LineStyles.h"
#include "S100_AreaFills.h"
#include "S100_Transform.h"
#include "S100_SymbolFill.h"
#include "S100_ColorProfile.h"
#include "S100_ColorProfiles.h"
#include "Enum_PortrayalRuleType.h"
#include "ViewingGroups.h"
#include "ViewingGroupLayers.h"
#include "DisplayModes.h"
#include "FoundationMode.h"
#include "DisplayPlanes.h"
#include "Context.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"
#include "..\\S100_SVG_D2D1_DLL\\SVGManager.h"
#include "..\\S100Engine\\S100Render.h"
#include "..\\GeoMetryLibrary\\ENCCommon.h"

#include <unordered_map>
#include <string>

using namespace Portrayal;

class S100PCManager;

class PortrayalCatalogue
{
public:
	PortrayalCatalogue();
	PortrayalCatalogue(std::wstring path);
	virtual ~PortrayalCatalogue();

private:
	// PC�� ���Ե� ���� (��: ..\ProgramData\\S101_Portrayal\\)
	std::wstring rootPath;

	PortrayalRuleType portrayalRuleType = XSLT;

	// �� : S-101
	std::wstring product;

	// �� : 1.1.1
	std::wstring version;

	// Rules (������ �ʿ� ����)
	S100_Rules rules;

	// ViewingGroups (���� �ʿ�)- �����ʽ��ϴ�
	ViewingGroups viewingGroups;

	// FoundationMode (����)- �����ʽ��ϴ�
	FoundationMode foundationMode;
	
	//ViewingGroupLayers ���� - �����ʽ��ϴ�
	ViewingGroupLayers viewingGroupLayers;

	//DisplayModes���� - �����ʽ��ϴ�
	DisplayModes displayModes;

	//Displayplane����- �����ʽ��ϴ�
	DisplayPlanes displayPlanes;

	//Context ���� - �����ʽ��ϴ�
	Context context;
	// Pixmaps (����) - �����ʽ��ϴ�.

	//Symbols ���� - �����ʽ��ϴ�

	// ColorProfiles (������ �ʿ� ����)
	S100_ColorProfiles colorProfiles; //������ �д°�

	//lineStyle������ �ʿ����
	S100_LineStyles lineStyles;
	
	//AreaFills ������ �ʿ����
	S100_AreaFills areaFills;

 	//Fonts ���� - �����ʽ��ϴ�

	// FOR Contents
	S100_ColorProfile s100_colorProfile;//���� �صΰ� �Ͽ��ϰ� ���� ���� �ڵ�

	std::unordered_map<std::wstring, S100_LineStyleBase*> s100_lineStyles; //linestyle �� ������ ����

	std::unordered_map<std::wstring, S100_SymbolFill*> s100_symbolFill; //AreaFills �� ������ ���� 

	std::vector<S100_Transform*> s100_transform; //rule�� ������ ����

	// FOR SENC
	std::unordered_map<std::wstring, S100_RuleFile*> mainRules;

	S100_SVG_D2D1_DLL::SVGManager *m_svgSymbolManager = nullptr;
	
	std::wstring currentPalette = L"Day";
	S100PCManager *s100PCManager = new S100PCManager();
	S100Render s100Render;

public:
	void Open(std::wstring& path);
	void Init();
	void Delete();
	bool ReadPortrayalCatalogueByPugiXML(std::wstring& path);	//pugixml�� ��ȯ
	void GetContents(pugi::xml_node& node);

	//set
	void SetRootPath(std::wstring& value);
	void SetPortrayalRuleType(PortrayalRuleType value);
	void SetProduct(std::wstring& value);
	void SetVersion(std::wstring& value);
	void SetRules(S100_Rules* value);
	void SetVewingGroups(ViewingGroups* value);
	void SetFoundationMode(FoundationMode* value);
	void SetViewingGroupLayers(ViewingGroupLayers* value);
	void SetDisplaModes(DisplayModes* value);
	void SetDisplayPlanes(DisplayPlanes* value);
	void SetContext(Context* value);
	void SetColorProfiles(S100_ColorProfiles* value);
	void SetAreaFills(S100_AreaFills* value);
	void SetColorProfile(S100_ColorProfile* value);
	void SetLineStyles(std::wstring& key, S100_LineStyleBase* value);
	void SetSymbolFill(std::wstring& key, S100_SymbolFill* value);
	void SetTransform(S100_Transform* value);
	void SetCurrentPaletteName(std::wstring& paletteName);
	void SetCurrentPalette(GeoMetryLibrary::ColorTable value);
	void SetS100PCManager(S100PCManager* value);
	void SetS100Render(S100Render* value);
	void SetSVGManager(S100_SVG_D2D1_DLL::SVGManager* value);

	//get
	std::wstring GetRootPath();
	PortrayalRuleType GetPortrayalRuleType();
	std::wstring GetProduct();
	std::wstring GetVersion();
	S100_Rules* GetRules();
	ViewingGroups* GetViewingGroups();
	FoundationMode* GetFoundationMode();
	ViewingGroupLayers* GetViewingGroupLayers();
	DisplayModes* GetDisplaModes();
	DisplayPlanes* GetDisplayPlanes();
	Context* GetContext();
	S100_ColorProfiles* GetColorProfiles();
	void GetAreaFills();
	S100_ColorProfile* GetColorProfile();
	S100_ColorProfile* GetColorProfile(std::wstring& id);
	S100_LineStyleBase* GetLineStyles(std::wstring& key);
	std::unordered_map<std::wstring, S100_LineStyleBase*> GetLineStyles();

	S100_SymbolFill* GetSymbolFill(std::wstring& key);
	std::unordered_map<std::wstring, S100_SymbolFill*> GetSymbolFill();

	S100_Transform* GetTransform(int index);
	std::vector<S100_Transform*> GetTransform();

	S100_RuleFile* GetMainRules(std::wstring& key);
	std::unordered_map<std::wstring, S100_RuleFile*> GetMainRules();

	S100Render GetS100Render();
	S100PCManager* GetS100PCManager();
	S100_SVG_D2D1_DLL::SVGManager* GetSVGManager();

	void GetLineStylesByPugiXml();

	void GetMainRuleFile();
	
	std::wstring GetCurrentPaletteName();

	void SetRuleType(PortrayalRuleType value);
	PortrayalRuleType GetRuleType();

	void CreatePatternImages(ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke);
	void DeletePatternImage();

	void CreateLineImages(ID2D1Factory1* d2Factory, IWICImagingFactory* imageFactory, ID2D1StrokeStyle1* stroke);
	void DeleteLineImages();
};