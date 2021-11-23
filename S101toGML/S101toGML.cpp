// S101toGML.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "pch.h"
#include "framework.h"
#include "S101toGML.h"

#include "../libS101/S101.h"

#include <iostream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
		std::cout << " Please enter all the routes. " << std::endl;

		std::cout << "openFilePath : ";
		string file;
		std::cin>> file;

		std::cout << "saveFilePath : ";
		string save;
		std::cin >> save;

		filepath = file.c_str();
		savepath = save.c_str();
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
			std::cout << "Open Success" << std::endl;
		}
		else 
		{
			std::cout << "Open Fail,Please check the file path." << std::endl;
			return -1;
		}

	
		a.Save(savepath, _T(""));
		std::cout << "gml Success"<<std::endl;

	}
	return 0;
}