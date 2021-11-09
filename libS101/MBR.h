#pragma once
#define DBL_MAX          1.7976931348623158e+308 // max value

class MBR
{
public:
	MBR();
	MBR(double _xmin, double _ymin, double _xmax, double _ymax);
	virtual ~MBR();

public:
	double xmin = DBL_MAX;
	double ymin = DBL_MAX;
	double xmax = -DBL_MAX;
	double ymax = -DBL_MAX;

public:
	void SetXMin(double value);
	double GetXMin();

	void SetYMin(double value);
	double GetYMin();

	void SetXMax(double value);
	double GetXMax();

	void SetYMax(double value);
	double GetYMax();

	void SetMBR(MBR mbr);
	void SetMBR(double _xmin, double _ymin, double _xmax, double _ymax);

	double GetWidth();
	double GetHeight();

	/*
	** ���� : ������ �ִ� MBR���� ���ڷ� ����
	**       MBR���� ���ؼ� ���ο� MBR�� ����Ѵ�.
	*/
	void ReMBR(MBR newMBR);
	void CalcMBR(double x, double y);
	void CalcMBR(MBR& mbr);
	void InitMBR();
	bool PtInMBR(double x, double y);
	void Scale(double Scale);
	bool operator== (MBR& _mbr);

	// static 
public:
	static bool CheckOverlap(MBR screenMBR, MBR objMBR);
	static bool IntersectMBR(const MBR& _mbr1, const MBR& _mbr2, MBR& _intersectMBR);
};