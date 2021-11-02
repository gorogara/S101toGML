#pragma once

#include "Record.h"
#include "F_DSID.h"
#include "F_DSSI.h"
#include "F_CodeWithNumericCode.h"

#include <string>

class F_ATTR;
class DRDirectoryInfo;

// Dataset General Information Record
class R_DSGIR : Record
{
public:
	R_DSGIR();
	virtual ~R_DSGIR();

public:
	F_DSID m_dsid;
	F_DSSI m_dssi;
	F_CodeWithNumericCode *m_atcs = nullptr;
	F_CodeWithNumericCode *m_itcs = nullptr;
	F_CodeWithNumericCode *m_ftcs = nullptr;
	F_CodeWithNumericCode *m_iacs = nullptr;
	F_CodeWithNumericCode *m_facs = nullptr;
	F_CodeWithNumericCode *m_arcs = nullptr;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool Save(CFile *file);

	// 뉴메릭 코드로 코드를 가져오는 함수
	CString GetFeatureCode(int numericCode);
	CString GetInformationCode(int numericCode);
	CString GetAttributeCode(int numericCode);
	CString GetInformationAssociationCode(int numericCode);
	CString GetFeatureAssociationCode(int numericCode);
	CString GetAssociationRoleCode(int numericCode);

	//void Update(R_DSGIR* record);
};