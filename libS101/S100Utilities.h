#pragma once

#include <string>

class S100Utilities
{
public:
	// XML ������ S-100 ExchangeSet Catalog���� �Ǵ�
	//static bool IsS100Catalog(std::wstring path);
	//static int GetLevel(std::wstring path);
	static int GetScaleByLevel(int level);
};