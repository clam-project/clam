#include "WindowGeneratorConfig.hxx"

namespace CLAM
{
	Enum::tEnumValue EWindowNormalize::sEnumValues[] = {
		{EWindowNormalize::eNone,"NoNormalization"},
		{EWindowNormalize::eAnalysis,"NormalizationForAnalysis"},
		{EWindowNormalize::eEnergy,"NormalizationForConstantEnergy"},
		{EWindowNormalize::eMax,"NormalizationForMaximizingMagnitude"},
		{0,NULL}
	};

	Enum::tValue EWindowNormalize::sDefault = EWindowNormalize::eAnalysis;


	void WindowGeneratorConfig::DefaultInit()
	{
		/* All Attributes are added */
		AddType();
		AddSize();
		AddMaxSize();
		AddUseTable();
		AddNormalize();
		AddInvert();

		UpdateData();
		DefaultValues();
	}

	void WindowGeneratorConfig::DefaultValues()
	{
		SetUseTable(true);
		SetSize(4097);
		SetNormalize(EWindowNormalize::eAnalysis);
		SetInvert(false);
		SetType(EWindowType::eHamming);
	}

}
