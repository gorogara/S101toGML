#pragma once

#ifdef LIBS101_EXPORTS
#define LIBS101_API __declspec(dllexport) 
#else
#define LIBS101_API __declspec(dllimport) 
#endif