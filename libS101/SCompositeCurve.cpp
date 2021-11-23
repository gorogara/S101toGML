#include "pch.h"
#include "SCompositeCurve.h"
#include "SCurve.h"

SCompositeCurve::SCompositeCurve()
{
	type = 2;
}


SCompositeCurve::~SCompositeCurve()
{
}


void SCompositeCurve::SetMBR()
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		m_mbr.CalcMBR((*i).GetCurve()->GetMBRRef());
	}
}