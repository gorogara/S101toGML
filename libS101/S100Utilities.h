#pragma once

#include <string>

class S100Utilities
{
public:
	// XML 파일이 S-100 ExchangeSet Catalog인지 판단
	//static bool IsS100Catalog(std::wstring path);
	//static int GetLevel(std::wstring path);
	static int GetScaleByLevel(int level);
};