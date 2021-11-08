// LibMFCUtil.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "LibMFCUtil.h"

#include <algorithm>
#include <regex>
#include <gdiplus.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "MD5ChecksumDefines.h"
#include "MD5Checksum.h"

#include "../LatLonUtility/TimeUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEG2RAD (M_PI / 180.0); 
#define RAD2DEG (180.0 / M_PI);

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CLibMFCUtilApp

BEGIN_MESSAGE_MAP(CLibMFCUtilApp, CWinApp)
END_MESSAGE_MAP()


// CLibMFCUtilApp construction

CLibMFCUtilApp::CLibMFCUtilApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	QueryPerformanceFrequency(&freq);
}


// The one and only CLibMFCUtilApp object

CLibMFCUtilApp theApp;


// CLibMFCUtilApp initialization

BOOL CLibMFCUtilApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


bool LibMFCUtil::ResizeCImage(CString _pathSrc, CString _pathDst, int _newWidth, int _newHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CImage srcImage;
	HRESULT res = srcImage.Load(_pathSrc);

	if (SUCCEEDED(res))
	{
		// 새 이미지 생성
		CImage dstImage;
		dstImage.CreateEx(_newWidth, _newHeight, 32, BI_RGB, NULL, dstImage.createAlphaChannel);

		// StretchBlt 모드 설정
		SetStretchBltMode(dstImage.GetDC(), COLORONCOLOR);
		dstImage.ReleaseDC();

		// StretchBlt 실행
		srcImage.SetTransparentColor(RGB(255, 255, 255));
		srcImage.StretchBlt(dstImage.GetDC(), 0, 0, _newWidth, _newHeight, SRCCOPY);
		dstImage.ReleaseDC();

		dstImage.Save(_pathDst);

		return true;
	}
	else
	{
		
	}

	return false;
}


bool LibMFCUtil::ResizeCImage(CImage& _src, CImage& _dst, int _newWidth, int _newHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// 새 이미지 생성
	_dst.CreateEx(_newWidth, _newHeight, 32, BI_RGB, NULL, _dst.createAlphaChannel);

	// StretchBlt 모드 설정
	SetStretchBltMode(_dst.GetDC(), COLORONCOLOR);
	_dst.ReleaseDC();

	// StretchBlt 실행
	_src.SetTransparentColor(RGB(255, 255, 255));
	_src.StretchBlt(_dst.GetDC(), 0, 0, _newWidth, _newHeight, SRCCOPY);
	_dst.ReleaseDC();

	return true;
}


bool LibMFCUtil::CheckFileKhoaDmsFormat(CString _path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStdioFile file;

	bool bCheck = true;

	if (file.Open(_path, CFile::modeRead))
	{
		CString line;

		while (file.ReadString(line)) {
			bCheck = LibMFCUtil::CheckKhoaDmsFormat(line);
			if (!bCheck)
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

#pragma warning(disable:4129)
bool LibMFCUtil::CheckKhoaDmsFormat(CString _str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::wregex rx(L"^\\s*\\d+\\.?\\d*(\\s*\\-\\s*\\d+\\.?\\d*\\s*){0,2}\\s*[NSns]{1}\\s*\,?\\s*\\d+\\.?\\d*(\\s*\\-\\s*\\d+\\.?\\d*\\s*){0,2}\\s*[EWew]{1}\\s*$");
	std::wstring wstr(_str);

	if (!std::regex_match(wstr, rx))
	{
		return false;
	}

	return true;
}


int LibMFCUtil::CheckKhoaDMSForm(std::wstring str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::wregex rx1(L"^\\d+\\.?\\d*(\\-\\d+\\.?\\d*){0,2}[NSns]{1}\\,?\\d+\\.?\\d*(\\-\\d+\\.?\\d*){0,2}[EWew]{1}$");
	std::wregex rx2(L"^\\d+\\.?\\d*(\\-\\d+\\.?\\d*){0,2}\\,{1}\\d+\\.?\\d*(\\-\\d+\\.?\\d*){0,2}$");

	if (std::regex_match(str, rx1))
	{
		return 1;
	}
	else if (std::regex_match(str, rx2))
	{
		return 2;
	}

	return 0;
}


CString LibMFCUtil::GetClipboardText(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!OpenClipboard(hWnd))
	{
		OutputDebugString(_T("Can't open clipboard!\n"));
		return _T("");
	}

	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	WCHAR* buffer = (WCHAR*)GlobalLock(hData);
	CString text;
	text.Format(_T("%s"), buffer);
	GlobalUnlock(hData);
	CloseClipboard();

	return text;
}


std::wstring LibMFCUtil::EraseAllSpace(std::wstring str)
{
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	return str;
}


char* LibMFCUtil::ConvertWCtoC(wchar_t* str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//반환할 char* 변수 선언
	char* pStr = nullptr;

	//입력받은 wchar_t 변수의 길이를 구함
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);

	if (strSize <= 0)
	{
		OutputDebugString(_T("Failed to WideCharToMultiByte()\n"));
		return nullptr;
	}

	//char* 메모리 할당
	pStr = new char[strSize];

	//형 변환 
	WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
	return pStr;
}


wchar_t* LibMFCUtil::ConvertCtoWC(char* str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//wchar_t형 변수 선언
	wchar_t* pStr = nullptr;
	//멀티 바이트 크기 계산 길이 반환
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);

	if (strSize <= 0)
	{
		OutputDebugString(_T("Failed to MultiByteToWideChar()\n"));
		return nullptr;
	}

	//wchar_t 메모리 할당
	pStr = new WCHAR[strSize];
	//형 변환
	MultiByteToWideChar(CP_ACP, 0, str, (int)strlen(str) + 1, pStr, strSize);
	return pStr;

}


HTREEITEM LibMFCUtil::FindTreeItem(CTreeCtrl* pTree, HTREEITEM hItem, DWORD data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HTREEITEM hItemFind, hItemChild, hItemSibling;
	hItemFind = hItemChild = hItemSibling = NULL;

	if (pTree->GetItemData(hItem) == data)
	{
		hItemFind = hItem;
	}
	else
	{
		hItemChild = pTree->GetChildItem(hItem);
		if (hItemChild)
		{
			hItemFind = FindTreeItem(pTree, hItemChild, data);
		}
	
		hItemSibling = pTree->GetNextSiblingItem(hItem);
		if (!hItemFind && hItemSibling)
		{
			hItemFind = FindTreeItem(pTree, hItemSibling, data);	
		}
	}

	return hItemFind;
}


CString LibMFCUtil::ReplaceExtension(CString path, CString newExtention)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int indexLastDot = path.ReverseFind(_T('.'));

	int lengthExtension = path.GetLength() - indexLastDot;

	path.Delete(indexLastDot + 1, lengthExtension);

	path.Append(newExtention);

	return path;
}

//확장자를 추출합니다: 전체 경로와 이름+확장자 모두 작동할 수 있도록 수정
CString LibMFCUtil::GetExtension(CString path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString ext = _T("");

	int length = path.GetLength();

	if (length <= 0) 
		return ext;

	int indexOfLastReverseSlash = path.ReverseFind('\\'); 
	if (indexOfLastReverseSlash >= 0)//전체 경로를 불러온경우
	{
		int indexOfLastDot = path.ReverseFind('.');
		if (indexOfLastDot >= 0)
		{
			int lengthOfExtension = path.GetLength() - indexOfLastDot - 1;
			ext = path.Right(lengthOfExtension);
			return ext;
		}
	}
	else //이름과 확장자만 불러온경우
	{
		int indexOfLastDot = path.ReverseFind('.');
		if (indexOfLastDot >= 0)
		{
			int lengthOfExtension = path.GetLength() - indexOfLastDot - 1;
			ext = path.Right(lengthOfExtension);
			return ext;
		}
	}

	
	return ext;
}


CString LibMFCUtil::GetFileName(CString path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString name = path;

	int length = path.GetLength();

	if (length <= 0)
		return name;

	int indexOfLastReverseSlash = path.ReverseFind('\\');
	if (indexOfLastReverseSlash >= 0)
	{
		int indexOfLastDot = path.ReverseFind('.');
		if (indexOfLastDot >= 0)
		{
			return path.Mid(indexOfLastReverseSlash + 1, indexOfLastDot - indexOfLastReverseSlash - 1);
		}
	}
	else
	{
		//13
		int indexOfLastDot = path.ReverseFind('.');
		int nameLength = path.GetLength();
		if (indexOfLastDot >= 0)
		{
			return path.Mid(0, indexOfLastDot);
		}
	}

	return name;
}


CString LibMFCUtil::GetFolderPathFromFilePath(CString path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CFile file;
	if (file.Open(path, CFile::modeRead ))
	{
		CT2CA pszConvertedAnsiString(file.GetFilePath());
		std::string s(pszConvertedAnsiString);
		int itor = (int)s.find_last_of('\\');
		std::string result = s.substr(0,itor)+"\\";

		CString resultC(result.c_str());
		return resultC;
		return file.GetFilePath();
	}
	return _T("");
}


int LibMFCUtil::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}


std::vector<CString> LibMFCUtil::GetFilePathFromFolder(CString folderPath, CString extension)
{
	CFileFind finder;

	BOOL bWorking = finder.FindFile(folderPath + _T("\\*.") + extension);

	std::vector<CString> vecFilePath;
	CString filePath;

	while (bWorking)
	{
		bWorking = finder.FindNextFileW();

		//if (finder.IsArchived())
		{
			vecFilePath.push_back(finder.GetFilePath());
		}
	}

	return vecFilePath;
}


CString LibMFCUtil::GetMD5(BYTE* pBuf, UINT nLength)
{
	return CMD5Checksum::GetMD5(pBuf, nLength);
}


CString LibMFCUtil::GetMD5(CFile& File)
{
	return CMD5Checksum::GetMD5(File);
}


CString LibMFCUtil::GetMD5(const CString& strFilePath)
{
	return CMD5Checksum::GetMD5(strFilePath);
}


void LibMFCUtil::MercatorProjectionForward(double lon, double lat, double* x, double* y)
{
	*x = MercatorProjectionForwardX(lon);
	*y = MercatorProjectionForwardY(lat);
}


void LibMFCUtil::MercatorProjectionReverse(double x, double y, double* lon, double* lat)
{
	*lon = MercatorProjectionReverseX(x);
	*lat = MercatorProjectionReverseY(y);
}


double LibMFCUtil::MercatorProjectionForwardX(double lon)
{
	return lon * DEG2RAD;
}


double LibMFCUtil::MercatorProjectionForwardY(double lat)
{
	if (lat <= -85.0f) {
		lat = -85.0f;
	}
	else if (lat >= 85.0f)
	{
		lat = 85.0f;
	}

	lat *= DEG2RAD;
	lat = log(tan((lat / 2.0) + M_PI_4));

	return lat;
}


double LibMFCUtil::MercatorProjectionReverseX(double x)
{
	return x * RAD2DEG;
}


double LibMFCUtil::MercatorProjectionReverseY(double y)
{
	y = 2 * (atan(exp(y)) - M_PI_4);
	return y * RAD2DEG;
}


std::string LibMFCUtil::CStringToString(CString str)
{
	CT2CA convertedString(str);
	return std::string(convertedString);
}

std::string LibMFCUtil::WStringToString(std::wstring str) 
{
	std::string Value;
	Value.assign(str.begin(), str.end());

	return Value;
}


void LibMFCUtil::StartPerformanceTimer()
{
	QueryPerformanceCounter(&theApp.sPos);
}


void LibMFCUtil::EndPerformanceTimer()
{
	QueryPerformanceCounter(&theApp.ePos);
}


__int64 LibMFCUtil::GetPerformanceTimeByMS()
{
	return (theApp.ePos.QuadPart - theApp.sPos.QuadPart) / (theApp.freq.QuadPart / 1000);
}


__int64 LibMFCUtil::GetPerformanceTimeByMicrosecond()
{
	return (theApp.ePos.QuadPart - theApp.sPos.QuadPart) / (theApp.freq.QuadPart / 1000000);
}


void LibMFCUtil::OutputPerformanceTimeByMS()
{
	CString str;
	str.Format(_T("Performance Time (ms) : %d\n"), GetPerformanceTimeByMS());
	OutputDebugString(str);
}


void LibMFCUtil::OutputPerformanceTimeByMicrosecond()
{
	CString str;
	str.Format(_T("Performance Time (microsecond) : %d\n"), GetPerformanceTimeByMicrosecond());
	OutputDebugString(str);
}


CString LibMFCUtil::INIReadString(CString strAppName, CString strKeyName, CString strFilePath)
{
	wchar_t szReturnString[1024] = { 0, };

	memset(szReturnString, NULL, sizeof(wchar_t) * 1024);

	GetPrivateProfileString(strAppName, strKeyName, _T(""), szReturnString, 1024, strFilePath);

	CString str;

	str.Format(_T("%s"), szReturnString);

	return str;
}

void LibMFCUtil::INIWriteString(CString strAppName, CString strKeyName, CString strValue, CString strFilePath)
{
	WritePrivateProfileString(strAppName, strKeyName, strValue, strFilePath);

}


CString LibMFCUtil::GetDesktopPath()
{
	TCHAR Path[MAX_PATH];

	SHGetSpecialFolderPath(NULL, Path, CSIDL_DESKTOP, FALSE);

	CString str(Path);

	return str;
}


bool LibMFCUtil::CheckDirExistAndCreate(CString path)
{
	CFileFind file;
	CString strFile = _T("*.*");
	BOOL bResult = file.FindFile(path + strFile);

	if (!bResult)
	{
		bResult = CreateDirectory(path + _T("\\"), NULL);
		if (!bResult)
		{
			/*Error*/
			return false;
		}
		return true;
	}
	return true;
}


bool LibMFCUtil::CopyTextToClipboard(CString str)
{
	// 저장할 문자열의 길이를 구한다. ('\0'까지 포함한 크기)
	int string_length = str.GetLength() + 1;
	int memSize = string_length * sizeof(TCHAR);

	// 클리보드로 문자열을 복사하기 위하여 메모리를 할당한다.
	// 클립보드에는 핸들을 넣는 형식이라서 HeapAlloc 함수 사용이 블가능하다.
	HANDLE h_data = GlobalAlloc(GHND | GMEM_MOVEABLE, memSize);

	// 할당된 메모리에 문자열을 복사하기 위해서 사용 가능한 주소를 얻는다.
	char *p_data = (char *)GlobalLock(h_data);

	if (NULL != p_data) 
	{
		std::wstring wstr(str);
		char* cstr = ConvertWCtoC((wchar_t *)wstr.c_str());

		// 할당된 메모리 영역에 삽입할 문자열을 복사한다.
		memcpy(p_data, cstr, memSize);
		// 문자열을 복사하기 위해서 Lock 했던 메모리를 해제한다. 
		GlobalUnlock(h_data);
		if (OpenClipboard(NULL)) 
		{
			// 클립보드를 연다. 
			EmptyClipboard();
			// 클립보드에 저장된 기존 문자열을 삭제한다. 
			SetClipboardData(CF_TEXT, h_data);
			// 클립보드로 문자열을 복사한다. 
			CloseClipboard(); // 클립보드를 닫는다.

			delete[] cstr;

			return true;
		}

		delete[] cstr;
	}

	return false;
}

POINT LibMFCUtil::RotatePoint(POINT& pt, double radian, int centerX, int centerY)
{
	POINT newPt;
	//newPt.x = (pt.x - centerX) * cos(radian) - (pt.y - centerY) * sin(radian) + centerX;
	//newPt.y = (pt.x - centerX) * sin(radian) - (pt.y + centerY) * cos(radian) + centerY;
	newPt.x = (LONG)(centerX + (pt.x - centerX) * cos(radian) - (pt.y - centerY) * sin(radian));
	newPt.y = (LONG)(centerY + (pt.x - centerX) * sin(radian) - (pt.y - centerY) * cos(radian));
	return newPt;
}


RECT LibMFCUtil::RotateRect(RECT& rect, double radian, int centerX, int centerY)
{
	RECT newRect;

	POINT pt;
	pt.x = rect.left;
	pt.y = rect.top;

	pt = RotatePoint(pt, radian, centerX, centerY);
	newRect.left = pt.x;
	newRect.top = pt.y;

	pt.x = rect.right;
	pt.y = rect.bottom;

	pt = RotatePoint(pt, radian, centerX, centerY);
	newRect.right= pt.x;
	newRect.bottom = pt.y;

	return newRect;
}

//작성자 : 성유빈
std::string LibMFCUtil::GetHDF5SplittedFilePath(std::string path)
{
	//
	auto Extensionsindex = path.find_last_of(".");
	auto Extensions = path.substr(Extensionsindex, path.size()); //확장자 추출

	auto underbarIndex = path.find_last_of("_"); //가장 마지막으로 있는값을 찾아냅니다
	auto OutputValue = path.substr(0, underbarIndex);

	OutputValue += "_%d"+ Extensions;

	return OutputValue;
}

bool LibMFCUtil::SaveImage(CString path, HDC& hdc, int width, int height)
{
	auto folderPath = FullPathToPath(path);
	LibMFCUtil::createDirectoryRecursively(std::wstring(folderPath));

	CImage capImage;
	// 캡쳐하고자하는 process의 HWND를 넣어준다.

	CRect rect(0, 0, width, height);

	if (!capImage.Create(rect.Width(), rect.Height(), 32))
	{
		return false;
	}

	HDC hDC = capImage.GetDC();

	BitBlt(hDC, 0, 0, rect.Width(), rect.Height(), hdc, 0, 0, SRCCOPY);
	capImage.Save(path);
	ReleaseDC(0, hDC);
	capImage.ReleaseDC();
	capImage.Destroy();

	return true;
}

bool LibMFCUtil::SaveImage2(CString path, CImage& image, HDC& hdc, int width, int height)
{
	auto folderPath = FullPathToPath(path);
	LibMFCUtil::createDirectoryRecursively(std::wstring(folderPath));

	HDC hDC = image.GetDC();

	BitBlt(hDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	image.Save(path);

	return true;
}

CString LibMFCUtil::FullPathToPath(CString fullPath)
{
	return fullPath.Left(fullPath.ReverseFind('\\'));
}

void LibMFCUtil::createDirectoryRecursively(const std::wstring &directory) {
	static const std::wstring separators(L"\\/");

	// If the specified directory name doesn't exist, do our thing
	DWORD fileAttributes = ::GetFileAttributesW(directory.c_str());
	if (fileAttributes == INVALID_FILE_ATTRIBUTES) {

		// Recursively do it all again for the parent directory, if any
		std::size_t slashIndex = directory.find_last_of(separators);
		if (slashIndex != std::wstring::npos) {
			LibMFCUtil::createDirectoryRecursively(directory.substr(0, slashIndex));
		}

		// Create the last directory on the path (the recursive calls will have taken
		// care of the parent directories by now)
		BOOL result = ::CreateDirectoryW(directory.c_str(), nullptr);
		if (result == FALSE) {
			throw std::runtime_error("Could not create directory");
		}

	}
	else { // Specified directory name already exists as a file or directory

		bool isDirectoryOrJunction =
			((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
			((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

		if (!isDirectoryOrJunction) {
			throw std::runtime_error(
				"Could not create directory because a file with the same name exists"
			);
		}

	}
}

SYSTEMTIME LibMFCUtil::Convert_2_SYSTEMTIME(time_t value)
{
	SYSTEMTIME result;

	LatLonUtility::TimeUtil timeUtil;
	timeUtil.Set(value);

	result.wYear = timeUtil.year;
	result.wMonth = timeUtil.mon;
	result.wDay = timeUtil.day;
	result.wHour = timeUtil.hour;
	result.wMinute = timeUtil.min;
	result.wSecond = timeUtil.sec;
	result.wMilliseconds = timeUtil.ms;

	return result;
}

SYSTEMTIME LibMFCUtil::Convert_2_SYSTEMTIME(tm value)
{
	SYSTEMTIME result;

	LatLonUtility::TimeUtil timeUtil;
	timeUtil.Set(value);

	result.wYear = timeUtil.year;
	result.wMonth = timeUtil.mon;
	result.wDay = timeUtil.day;
	result.wHour = timeUtil.hour;
	result.wMinute = timeUtil.min;
	result.wSecond = timeUtil.sec;
	result.wMilliseconds = timeUtil.ms;

	return result;
}