#include "stdafx.h"
#include "StringTokenizer.h"


StringTokenizer::StringTokenizer(const std::string& inputstring, const std::string& seperator)
: _input(inputstring), _delimiter(seperator)
{
	split();
}

StringTokenizer::~StringTokenizer()
{

}

size_t StringTokenizer::countTokens()
{
	return token.size();
}

bool StringTokenizer::hasMoreTokens()
{
	return index != token.end();
}

std::string StringTokenizer::nextToken()
{
	if (index != token.end()) return *(index++);
	else return "";
}

void StringTokenizer::split()
{
	std::string::size_type lastPos = _input.find_first_not_of(_delimiter, 0); //�����ڰ� ��Ÿ���� �ʴ� ��ġ
	std::string::size_type Pos = _input.find_first_of(_delimiter, lastPos); //�����ڰ� ��Ÿ���� ��ġ

	while (std::string::npos != Pos || std::string::npos != lastPos)
	{
		token.push_back(_input.substr(lastPos, Pos - lastPos));
		lastPos = _input.find_first_not_of(_delimiter, Pos); //�����ڰ� ��Ÿ���� �ʴ� ��ġ
		Pos = _input.find_first_of(_delimiter, lastPos); //�����ڰ� ��Ÿ���� ��ġ
	}

	index = token.begin();
}
