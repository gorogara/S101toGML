#pragma once
#include "MBR.h"

class Scaler;

class Symbol;

class Geometry
{
public:
	Geometry();
	virtual ~Geometry();

public:
	MBR m_mbr;

public:
	MBR& GetMBRRef();
};