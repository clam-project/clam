#include "TickExtractorConfig.hxx"

namespace CLAM
{
	void TickExtractorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetFromAudio(true);
		SetComputeBeats(true);
		SetSaveIOIHistData(false);
		SetDownsamplingFactor_IOIHist(100.0);
		SetThreshold_IOIHistPeaks(0.0);//in percents
		SetTempoLimInf(50);
		SetTempoLimSup(220);
		SetTickLimInf(0.6);
		SetTickLimSup(0.09);
		SetDeviationPenalty(1);
		SetOverSubdivisionPenalty(5); // bigger --> favor large ticks
		SetGaussianWindowSize(0.2);
		SetScope(0.10); //in seconds
		SetAdjustWithOnsets(true);
		SetNTrans(500);
		SetTransHop(100);
	}

}
