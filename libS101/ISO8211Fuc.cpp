#include "pch.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

#undef _WINDOWS_
#include <afxext.h>
#include <cmath>
#include <atlbase.h>
#include <atlconv.h>
#include <algorithm>
#include <bitset> // Convert the 32-bit binary encoding into hexadecimal



BYTE tBYTEArr[1024];
void buf2charArr(CString &dest, BYTE*& buf)
{
	USES_CONVERSION;
	int i = 0;
	int endLoc = 0;
	
	while (buf[endLoc] != NonPrintableCharacter::unitTerminator)
	{
		endLoc++;
	}
	for(i = 0; i < endLoc; i++)
	{
		tBYTEArr[i] = *(buf++);
	}
	tBYTEArr[i] = NULL;
	dest = CA2W((char*)tBYTEArr, CP_UTF8);
	buf++;
}

void buf2charArr(std::wstring &dest, BYTE*& buf)
{
	//USES_CONVERSION;
	int i = 0;
	int endLoc = 0;
	
	while(buf[endLoc] != 0x1F)
	{
		endLoc++;
	}
	for(i = 0; i < endLoc; i++)
	{
		tBYTEArr[i] = *(buf++);
		//dest.AppendChar(*(buf++));
	}
	tBYTEArr[i] = NULL;
	dest = ATL::CA2W((char*)tBYTEArr, CP_UTF8);
	buf++;
}

void buf2charArr(CString &dest, BYTE*& buf, int len)
{
	for(int i = 0; i < len; i++)
	{
		dest.AppendChar(*(buf++));
	}
}


void buf2charArr(std::wstring &dest, BYTE*& buf, int len)
{
//	USES_CONVERSION;
	int i = 0;
	for(i = 0; i < len; i++)
	{
		tBYTEArr[i] = *(buf++);
		//dest.AppendChar(*(buf++));
	}
	tBYTEArr[i] = NULL;
	dest = ATL::CA2W((char*)tBYTEArr, CP_UTF8);
}

DWORD buf2uint(BYTE*& buf, int size)
{
	DWORD ret = 0;
	//for(int i = 0; i < size; i++)
	//{
	//	ret |= ((*(buf++))<<(8*i))&(0xff<<(8*i));
	//	//ret = ret << 8 | *buf;
	//	//buf++;
	//}
	memcpy_s(&ret, size, buf, size);

	buf += size;
	return ret;
}

DWORD buf2int(BYTE*& buf, int size)
{
	DWORD ret = 0;
	//for(int i = 0; i < size; i++)
	//{
	//	ret |= ((*(buf++))<<(8*i))&(0xff<<(8*i));
	//	//ret = ret << 8 | *buf;
	//	//buf++;
	//}
	memcpy_s(&ret, size, buf, size);

	buf += size;
	return ret;
}

char TEMP_BUF256[1024];
DWORD atoi(BYTE*& buf, int len)
{
	DWORD ret = 0;
	for(int i=0; i< len;i++)
	{
		ret = ret * 10 + *(buf++) - '0';
	}
	return ret;
}

double buf2double(BYTE*& buf, int size)
{
	double ret = 0;
	memcpy_s(&ret, 8, buf, 8);
	buf += 8;
	return ret;
	//std::string Binary;

	//for (int i = 0; i < size; i++)
	//{
	//	for (int j = 0; j < 8; j++)
	//	{
	//		char c = buf[0] & (0x01 < (7 - j));
	//		if (c & 0xFF)
	//		{
	//			Binary.append("1");
	//		}
	//		else
	//		{
	//			Binary.append("0");
	//		}
	//	}
	//	buf++;
	//}

	//std::bitset<32> set(Binary);
	//int HexNumber = set.to_ulong();
	//bool negative = !!(HexNumber & 0x80000000);
	//int  exponent = (HexNumber & 0x7f800000) >> 23;
	//int sign = negative ? -1 : 1;
	//// Subtract 127 from the exponent     
	//exponent -= 127;
	//// Convert the mantissa into decimal using the     
	//// last 23 bits     
	//int power = -1;
	//float total = 0.0;
	//for (int i = 0; i < 23; i++)     {
	//	int c = Binary[i + 9] - '0';
	//	total += (float)c * (float)pow(2.0, power);
	//	power--;
	//}
	//total += 1.0;
	//float value = sign * (float)pow(2.0, exponent) * total;
	////float value = 0;

	//return value;

	//std::string s(reinterpret_cast<char const*>(buf), 8);

	//ret = GetFloat32(s);
	//str.append(buf, 8);

	////ret =  *((double*)buf);
	////ret = *reinterpret_cast<double*>(buf);
	////buf+=8;
	//DOUBLE a = 0;
	//for(int i = 0; i< size; i++)
	//{
	//	a = *(buf++) << 8*i;
	//	ret += a;
	//}

	//int sign = b & 0x80? 1 : 0;
	//int quotient = b & 0x7F << 4;
	//
	//b = *(buf++);
	//quotient |= b&0xF0 >> 4;

	//int mantissa = b&0x0F << 48;

	//for(int i = 0; i < size; i++)
	//{
	//	mantissa |= ((*(buf++))<<(8*i))&(0xff<<(8*i));
	//	//ret = ret << 8 | *buf;
	//	//buf++;
	//}

	//return ret;
}

int asNumeric( wchar_t wch )
{
	int result = -1;
	for ( wchar_t const* const* p = std::begin( digitTables );
		p != std::end( digitTables ) && result == -1;
		++ p ) {
			wchar_t const* q = std::find( *p, *p + 10, wch );
			if ( q != *p + 10 ) {
				result = (int)(q - *p);
			}
	}
	return result;
}
