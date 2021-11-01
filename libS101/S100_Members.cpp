#include "stdafx.h"
#include "S100_Members.h"

#include <sstream>

namespace S100
{

	std::wstring GetProductAsString(S100_DataProduct product)
	{
		std::wstring ret;
		switch (product)
		{
		case  S_101TDS: ret = L"S-101TDS"; break;
		default:
			if (product > 100)
		{
			std::wstringstream wss;
			wss << L"S-" << product;
			ret = wss.str();
		}
		else
			ret = L"S-100"; break;
		}

		return ret;
	};


	std::wstring NameConvertMemberWstring(std::wstring value) 
	{
		auto index=value.find(L"S-");
		if (index<0) 
		{
			return L"S-XXX";
		}
		std::wstring member=value.substr(index +2,3);

		auto result=GetProductAsString((S100_DataProduct)_wtoi(member.c_str()));
		return result;
	}
}