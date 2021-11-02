#pragma once

#include "Field.h"
#include "RecordName.h"

#include <string>
// Dataset Identification Field

class F_DSID : Field
{
public:
	F_DSID();
	virtual ~F_DSID();

public:
	RecordName m_name;
	std::wstring m_ensp; //encodingSpecification(������ ����) //S-100 Part 10a
	std::wstring m_ened; //encodingSpecificationEdition(������ ����) //1.1
	std::wstring m_prsp;	//productIdentifier(������ ����) //INT.IHO.S-101.1.0
	std::wstring m_pred; //productEdition(������ ����) //1.0
	std::wstring m_prof; //applicationProfile(������ ����) //1
	std::wstring m_dsnm; //datasetFileIdentifier
	std::wstring m_dstl; //datasetTitle
	std::wstring m_dsrd; //datasetReferenceDate
	std::wstring m_dslg; //datasetLanguage(������ ����) //EN
	std::wstring m_dsab; //datasetAbstract
	std::wstring m_dsed; //dataset Edition
	std::vector<int> m_dstc;

public:
	void ReadField(BYTE *&buf);
	BOOL Save(CFile *file);
	int GetFieldLength();
};