// S101toGML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "S101toGML.h"

#include "../libS101/S101.h"

#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int main(int argc, char* argv[])
{
	int nRetCode = 0;
	HMODULE hModule = ::GetModuleHandle(nullptr);

	CString filepath;
	CString savepath;


	if (argc < 3)
	{
		std::cout << " 경로를 모두 입력해주세요 " << std::endl;

		string file;
		std::cin>> file;

		string save;
		std::cin >> save;

		filepath = file.c_str();
		savepath = save.c_str();

		//return -1;
	}
	else {

		for (int i = 0; i < argc; i++)
		{
			std::cout << "Value : " << argv[i] << std::endl;
		}

		std::string file = argv[1];
		filepath = file.c_str();
		std::cout << "Value : " << filepath << std::endl;

		string save = argv[2];
		savepath = save.c_str();
		std::cout << "Value : " << savepath << std::endl;
	}



	if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
	{
		std::cout << "MFC initialization failed" << std::endl;
		nRetCode = 1;
	}
	else
	{
		libS101::S101 a;

		if (a.Open(filepath))
		{
			std::cout << "읽기완료 " << std::endl;
		}
		else 
		{
			std::cout << "읽기 실패,파일경로를 확인해주세요  " << std::endl;
			return -1;
		}

	
		a.Save(savepath, _T("dfs"));
		std::cout << "gml화 완료 ";

	}
	return 0;
}