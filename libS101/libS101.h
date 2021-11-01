#pragma once

#include <string>

#ifdef LIBS101_EXPORTS
#define LIBS101_API __declspec(dllexport) 
#else
#define LIBS101_API __declspec(dllimport) 
#endif

namespace libS101
{

	extern LIBS101_API bool Convert(std::wstring sourcePath, std::wstring destinationPath);

}