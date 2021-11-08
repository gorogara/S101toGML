// LibMFCUtil.h : main header file for the LibMFCUtil DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include <vector>
#include <atlimage.h>
#define DELETE_NULL(x) { delete[] x; x = nullptr; }

// CLibMFCUtilApp
// See LibMFCUtil.cpp for the implementation of this class
//

#ifdef LIBMFC_API_EXPORT
	#define LIBMFC_API __declspec(dllexport)
#else
	#define LIBMFC_API __declspec(dllimport)
#endif

class CTreeCtrl;

class LIBMFC_API CLibMFCUtilApp : public CWinApp
{
public:
	CLibMFCUtilApp();
	LARGE_INTEGER sPos;
	LARGE_INTEGER ePos;
	LARGE_INTEGER freq;

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

namespace LibMFCUtil
{
	extern LIBMFC_API bool ResizeCImage(CString _pathSrc, CString _pathDst, int _newWidth, int _newHeight);
	extern LIBMFC_API bool ResizeCImage(CImage& _src, CImage& _dst, int _newWidth, int _newHeight);
	extern LIBMFC_API bool CheckFileKhoaDmsFormat(CString _path);   // D-M-S, D-M-S 형태의 좌표 파일인지 검사
	extern LIBMFC_API bool CheckKhoaDmsFormat(CString _str);        // D-M-S, D-M-S 형태인지 검사
	extern LIBMFC_API int CheckKhoaDMSForm(std::wstring str);
	extern LIBMFC_API CString GetClipboardText(HWND hWnd);
	extern LIBMFC_API std::wstring EraseAllSpace(std::wstring str);
	extern LIBMFC_API char* ConvertWCtoC(wchar_t* str); // 반환된 스트링 해제 해줘야 함
	extern LIBMFC_API wchar_t* ConvertCtoWC(char* str); // 반환된 스트링 해제 해줘야 함
	extern LIBMFC_API HTREEITEM FindTreeItem(CTreeCtrl* pTree, HTREEITEM hItem, DWORD data);
	extern LIBMFC_API int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	extern LIBMFC_API CString GetMD5(BYTE* pBuf, UINT nLength);
	extern LIBMFC_API CString GetMD5(CFile& File);
	extern LIBMFC_API CString GetMD5(const CString& strFilePath);
	extern LIBMFC_API void StartPerformanceTimer();
	extern LIBMFC_API void EndPerformanceTimer();
	extern LIBMFC_API __int64 GetPerformanceTimeByMS();
	extern LIBMFC_API __int64 GetPerformanceTimeByMicrosecond();
	extern LIBMFC_API void OutputPerformanceTimeByMS();
	extern LIBMFC_API void OutputPerformanceTimeByMicrosecond();

	extern LIBMFC_API SYSTEMTIME Convert_2_SYSTEMTIME(time_t value);
	extern LIBMFC_API SYSTEMTIME Convert_2_SYSTEMTIME(tm value);

	extern LIBMFC_API POINT RotatePoint(POINT& pt, double radian, int centerX, int centerY);
	extern LIBMFC_API RECT RotateRect(RECT& rect, double radian, int centerX, int centerY);
	extern LIBMFC_API std::string GetHDF5SplittedFilePath(std::string path);

	//Gisviewer
	//extern __declspec(dllexport) CString IntToCString(int value);
	//extern __declspec(dllexport) CString DoubleToCString(double value);
	//extern __declspec(dllexport) CString CharToCString(char *value, int length = 0);
	//extern __declspec(dllexport) CString CharToCString(const char* value);
	//extern __declspec(dllexport) CString CharToCString(wchar_t *value, int length);
	//extern __declspec(dllexport) int Or(std::vector<int> &values);
	//extern __declspec(dllexport) int GetXthDigit(int value, int Xth);

	//extern __declspec(dllexport) bool S52AttributeMatching(CString lookupTableAttribute, CString objectAttribute);

	//// value - 소수자리가 1자리 일 때 사용 가능
	//extern __declspec(dllexport) int GetFractionValue(double value);

	// 반환 예 : C:\Users\gmjo\Desktop
	extern LIBMFC_API CString GetDesktopPath();

	extern LIBMFC_API std::string CStringToString(CString str);
	extern LIBMFC_API std::string WStringToString(std::wstring str);

	// 마지막 '.' 뒤의 문자열을 newExtension으로 바꾼다
	extern LIBMFC_API CString ReplaceExtension(CString path, CString newExtention);

	// 파일 경로에서 확장자만 가져온다(예 : txt, jpg)
	extern LIBMFC_API CString GetExtension(CString path);

	// 파일 경로에서 파일 이름만 가져온다();
	extern LIBMFC_API CString GetFileName(CString path);

	//extern __declspec(dllexport) CString GetFileNameWithoutExt(CString path);
	extern LIBMFC_API CString GetFolderPathFromFilePath(CString path);

	// 폴더에서 특정 확장자를 가진 파일 경로를 전부 가져온다
	// extension에 ""을 넣어주면 전부 가져온다.
	// folderPath는 C:\FolderName처럼 마지막에 \는 필요 없다.
	extern LIBMFC_API std::vector<CString> GetFilePathFromFolder(CString folderPath, CString extension = _T(""));
	//extern __declspec(dllexport) std::vector<CString> GetFilePathFromFolderByRecursively(CString folderPath, CString extension = _T(""));

	extern LIBMFC_API void MercatorProjectionForward(double lon, double lat, double* x, double* y);
	extern LIBMFC_API void MercatorProjectionReverse(double x, double y, double* lon, double* lat);
	extern LIBMFC_API double MercatorProjectionForwardX(double lon);
	extern LIBMFC_API double MercatorProjectionForwardY(double lat);
	extern LIBMFC_API double MercatorProjectionReverseX(double x);
	extern LIBMFC_API double MercatorProjectionReverseY(double y);

	extern LIBMFC_API CString INIReadString(CString strAppName, CString strKeyName, CString strFilePath);
	extern LIBMFC_API void INIWriteString(CString strAppName, CString strKeyName, CString strValue, CString strFilePath);

	extern LIBMFC_API bool CheckDirExistAndCreate(CString path);

	extern LIBMFC_API bool CopyTextToClipboard(CString str);

	extern LIBMFC_API bool SaveImage(CString path, HDC& hdc, int width, int height);

	extern LIBMFC_API bool SaveImage2(CString path, CImage& image, HDC& hdc, int width, int height);

	extern LIBMFC_API CString FullPathToPath(CString fullPath);

	extern LIBMFC_API void createDirectoryRecursively(const std::wstring &directory);

	/*extern __declspec (dllexport) bool IsEqual(double a, double b);

	extern __declspec(dllexport) bool GetFilePathAndFileName(CString path, CString& filePath, CString& fileName);
*/
	
}