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

		BPF weightDefault;
		weightDefault.Insert( 0.0, 1.0 );
		weightDefault.Insert( 1.0, 1.0 );

		SetHybSinShapeW1( weightDefault );
		SetHybSinShapeW2( weightDefault );
		
		SetHybResShapeW1( weightDefault );
		SetHybResShapeW2( weightDefault );

	}
}
