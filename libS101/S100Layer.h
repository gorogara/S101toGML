#pragma once

#include "Layer.h"
#include <string>

class Catalog;
class PortrayalCatalogue;
class S100SpatialObject;
class FeatureCatalogue;

class S100Layer : public Layer
{
public:
	S100Layer();
	S100Layer(Catalog *fc, PortrayalCatalogue *pc);
	virtual ~S100Layer();

private:
	Catalog *featureCatalogue = nullptr;
	PortrayalCatalogue *portrayalCatalogue = nullptr;
	bool hasIndividualFC = false;
	bool hasIndividualPC = false;

public:
	void SetFC(Catalog *value);
	Catalog* GetFC();
	FeatureCatalogue* GetFeatureCatalog();
	void SetPC(PortrayalCatalogue *value);
	PortrayalCatalogue* GetPC();
	void SetIndividualFC(bool value);
	bool GetIndividualFC();
	void SetIndividualPC(bool value);
	bool GetIndividualPC();
	bool OpenFC(CString path);
	bool OpenPC(CString path);
	void DeleteCatalog();
	//void DrawInfo(HDC &hDC, Scaler *scaler);
	S100SpatialObject* GetS100SpatialObject();

public: // virtual
	virtual int GetProductNumber();
	virtual std::wstring GetChartName() { return L""; }
	virtual std::wstring GetEditionNumberAsWstring() { return L""; }
	virtual std::wstring GetUpdateNumberAsWstring() { return L""; }
	virtual std::wstring GetIssueDateAsWstring() { return L""; }
};