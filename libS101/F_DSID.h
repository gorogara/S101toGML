#pragma once
#include "Field.h"
#include "RecordName.h"

#include <vector>

#undef _WINDOWS_
#include <afxext.h>
// Dataset Identification Field

class F_DSID : Field
{
public:
	F_DSID();
	virtual ~F_DSID();

public:
	RecordName m_name;
	CString m_ensp; //encodingSpecification(������ ����) //S-100 Part 10a
	CString m_ened; //encodingSpecificationEdition(������ ����) //1.1
	CString m_prsp;	//productIdentifier(������ ����) //INT.IHO.S-101.1.0
	CString m_pred; //productEdition(������ ����) //1.0
	CString m_prof; //applicationProfile(������ ����) //1
	CString m_dsnm; //datasetFileIdentifier
	CString m_dstl; //datasetTitle
	CString m_dsrd; //datasetReferenceDate
	CString m_dslg; //datasetLanguage(������ ����) //EN
	CString m_dsab; //datasetAbstract
	CString m_dsed; //dataset Edition
	CArray<int> m_dstc;

public:
	void ReadField(BYTE *&buf);
	BOOL Save(CFile *file);
	int GetFieldLength();
};