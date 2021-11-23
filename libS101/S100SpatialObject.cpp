#include "pch.h"
#include "S100SpatialObject.h"
//#include "S100Layer.h"

S100SpatialObject::S100SpatialObject()
{
}


S100SpatialObject::~S100SpatialObject()
{
}


void S100SpatialObject::SetS100Product(S100::S100_DataProduct value)
{
	S100Product = value;
}


S100::S100_DataProduct S100SpatialObject::GetS100Product()
{
	return S100Product;
}