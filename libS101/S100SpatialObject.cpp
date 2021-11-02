#include "pch.h"
#include "S100SpatialObject.h"
//#include "PCOutputSchemaManager.h"
#include "S100Layer.h"

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


//void S100SpatialObject::SetMBR(MBR& value)
//{
//	if (nullptr != m_pLayer)
//	{
//		m_pLayer->SetMBR(value);
//	}
//}
//
//
//MBR S100SpatialObject::GetMBR()
//{
//	if (nullptr != m_pLayer)
//	{
//		return m_pLayer->GetMBR();
//	}
//
//	return MBR();
//}


//void S100SpatialObject::SetPCOutputManager(PCOutputSchemaManager* value)
//{
//	pcManager = value;
//}
//
//
//PCOutputSchemaManager* S100SpatialObject::GetPCOutputManager()
//{
//	return pcManager;
//}

S100Layer* S100SpatialObject::GetS100Layer() 
{
	return (S100Layer*)m_pLayer;
}