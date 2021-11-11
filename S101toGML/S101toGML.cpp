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

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);


    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {

            libS101::S101 a;

            CString filepath;
            CString savepath;

            char path[128];
          
            if (GetCurrentDirectoryA(128, path) > 0)
            {
                filepath.Format(_T("%S\\File\\101KR005X01SW.000"),path);
                savepath.Format(_T("%S\\File\\test.gml"), path);
            }

            a.Open(filepath);
            a.Save(savepath,_T("dfs"));

            std::cout << "완료했습니다 ";
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
