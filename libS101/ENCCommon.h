#pragma once

#include <unordered_map>
#include <string>

namespace GeoMetryLibrary
{

	enum ColorTable
	{
		Day = 0,
		Dusk,
		Night,
	};

	enum DisplayModeTable
	{
		base = 0,
		standard = 1,
		all = 2
	};

	enum UserMode
	{
		User_Mode,
		Developer_Mode,
		User_Developer_Mode
	};

	enum ReportingData
	{
		None, 
		PC,
		FC,
		DataSet
	};

	enum UnitDistance
	{
		unitDistance_km = 0,
		unitDistance_nautical_mile = 1,
	};

}

class ENCCommon
{
public:
	// true = 1  
	// false = 0 (default, 임시)
	static bool SymbolizedAreaBoundary;

	// true = 1  
	// false = 0 (default, 임시)
	static bool SeabedAreaType;

	static bool TWO_SHADES;
	static bool USE_SPATIAL_QUERY;
	static bool SHALLOW_PATTERN;

	//  true면 수심 표시
	// false면 수심 표시 안함
	static bool SOUNDING;

	//  true면 등대 불빛 표시
	// false면 등대 불빛 표시 안함
	static bool LIGHTS;

	//  ture면 Depth contour 표시
	// false면 Depth contour 표시 안함
	static bool CONTOUR_LABELS;

	static bool	TEXTOUT;

	static bool	SHOW_ISOLATED_DANGER_IN_SHALLOW_WATER;

	static bool	FULL_SECTORS;

	// true면 scale min 적용
	// false면 scale min 적용 안함
	static bool APPLY_SCALE_MIN;

	// true = National Object Name 표출
	static bool SHOW_NOBJNM;

	static bool AVOID_CLUTTER;

	static bool SHOW_LIST_ABBREVIATION;

	static bool WGS84_TEXT_TYPE;
	static bool	SHOW_USER_COAST_AREA;
	static bool	SHOW_USER_COAST_AREA_DETAIL;

	static double SAFETY_DEPTH;				// unit : meter [IMP PS [3]]

	static double SAFETY_CONTOUR;			// unit : meter (optional)
	static double SHALLOW_CONTOUR;			// unit : meter [IMP PS [3]]
	static double DEEP_CONTOUR;				// unit : meter (optional)

	static int DISTANCE_TAGS;
	static int DISPLAY_FONT_SIZE;

	static unsigned DrawingType;		    // paper  = 1
										// simple = 0
	static __int64 OVER_GROUP;

	static std::wstring DISPLAY_FONT_NAME;

	static int DISPLAY_MODE;
	static int m_eColorTable;
	static int UNIT_DISTANCE;
	static int m_ReportingData;
	static int m_UserMode;

	static std::unordered_map<int, bool> objectDisplaySettings;
	static std::unordered_map<std::wstring, bool> featureDisplaySettings;

	static bool DISPLAY_PLANE;

	// CUSTOM SETTINGS
	static bool SHOW_USER_WRECK;
	static bool SHOW_USER_MEMORIES;
	static bool SHOW_USER_FISHERY_ZONE;
	static bool SHOW_USER_IMAGE_LAYER;

	static double S111_SREF;
	static double S111_LREF;
	static double S111_SHIGH;
	static double S111_Alpha;

	static bool APPLY_DATA_COVERAGE_SCALE;

	static float DISPLAY_SYMBOL_SCALE;

	// true: 현재 화면의 중심점 (유동)
	// false: AREA 전체 영역의 중심점 (고정)
	static bool AREA_SYMBOL_DYNAMIC_POSITION_MODE;

	static int INTEROPERABILITY_LEVEL; //DISTANCE_TAGS;

	// Show Text Placement
	static BOOL	SHOW_TEXT_PLACEMENT;

	// Show wreck/obstruction/rock's uncertainty
	static BOOL SHOW_UNCERTAINTY;

	// Under/Over scale 시 심볼 크기 변경 Test
	static int T_APPLY_OVER_N_UNDER_SCALE;
	static int T_CURRENT_SCALE;

	// DETECTION 모드 설정
	static bool DETECTION_MODE;


	static bool SIMPLIFIED_POINT_SYMBOL;
	static bool AUTOSELECTION_CATALOGUE;
	static bool S111_SHOW_NODATA;
	static bool Show_INFORM01; //inform01 정보를 표출하거나 숨깁니다
	
	static bool S111_THINNING;

	static bool USING_MTP;

public:
	//static __int64 GetOverGroup(int viewGroup);
	//static bool Save(std::wstring filePath);
	//static bool Open(std::wstring filePath);
};