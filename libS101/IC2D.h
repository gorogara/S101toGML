#pragma once

struct IC2D
{
public:
	/*
	* Coordinate in Y axis
	* Format		: b24
	* Description	: Y-coordinate or latitude
	*/
	int m_ycoo;
	/*
	* Coordinate in X axis
	* Format		: b24
	* Description	: X-coordinate or longitude
	*/
	int m_xcoo;

	static int GetSize();
};