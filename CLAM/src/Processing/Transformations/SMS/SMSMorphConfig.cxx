#include "SMSMorphConfig.hxx"

namespace CLAM
{
	SMSMorphConfig::~SMSMorphConfig()
	{
	}

	void SMSMorphConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		DefaultValues();
	}

	void SMSMorphConfig::DefaultValues()
	{
		BPF tmpBPF(2);
		tmpBPF.SetValue(0,0);
		tmpBPF.SetXValue(0,0);
		tmpBPF.SetValue(1,1);
		tmpBPF.SetXValue(1,1);
		SetHybBPF(tmpBPF);

		SetInterpolateFrame(true);
		SetHybSinFreq(GetHybBPF());
		SetHybSinAmp(GetHybBPF());
		SetHybPitch(GetHybSinFreq());

		SetHybResAmp(GetHybBPF());

		SetSynchronizeTime(tmpBPF);

		BPF defaultWeight;
		defaultWeight.Insert( 0.0, 1.0 );
		defaultWeight.Insert( 1.0, 1.0 );

		SetHybSinShapeW1( defaultWeight );
		SetHybSinShapeW2( defaultWeight );
		
		SetHybResShapeW1( defaultWeight );
		SetHybResShapeW2( defaultWeight );

		SetHybSinSpectralShape( defaultWeight );

	}
}
