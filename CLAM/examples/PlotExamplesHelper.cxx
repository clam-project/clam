#include "PlotExamplesHelper.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx" 
#include "FDFilterGen.hxx"
#include <cstdlib>
#include <ctime>

// The following code is here just for generating some data. Go directly
// to the main() function

void generateLowPassFilter( CLAM::Spectrum& spectrum )
{
	CLAM::FDFilterGenConfig myFilterGeneratorConfig;
	myFilterGeneratorConfig.SetType(CLAM::EFDFilterType::eLowPass);
	myFilterGeneratorConfig.SetSpectralRange(22050);
	myFilterGeneratorConfig.SetGain(1);
	myFilterGeneratorConfig.SetLowCutOff(1000);
	myFilterGeneratorConfig.SetStopBandSlope(12);
	
	CLAM::FDFilterGen myFilterGenerator;
	
	myFilterGenerator.Configure( myFilterGeneratorConfig );
	
	myFilterGenerator.Start();
	
	CLAM::SpecTypeFlags specFlags;
	
	specFlags.bMagPhase = 0;
	specFlags.bMagPhaseBPF=1;
	spectrum.SetType(specFlags);
	spectrum.SetSpectralRange(22050);
	spectrum.SetScale(CLAM::EScale::eLog);
	myFilterGenerator.Do(spectrum);
	
	myFilterGenerator.Stop();
}

void generateBandPassFilter( CLAM::Spectrum& spectrum )
{
	CLAM::FDFilterGenConfig myFilterGeneratorConfig;
	myFilterGeneratorConfig.SetType(CLAM::EFDFilterType::eBandPass);
	myFilterGeneratorConfig.SetSpectralRange(22050);
	myFilterGeneratorConfig.SetGain(1);
	myFilterGeneratorConfig.SetLowCutOff(2500);
	myFilterGeneratorConfig.SetHighCutOff( 6000 );
	myFilterGeneratorConfig.SetStopBandSlope(64);
	myFilterGeneratorConfig.SetPassBandSlope(14);
	
	CLAM::FDFilterGen myFilterGenerator;
	
	myFilterGenerator.Configure( myFilterGeneratorConfig );
	
	myFilterGenerator.Start();
	
	CLAM::SpecTypeFlags specFlags;

	specFlags.bMagPhase = 0;	
	specFlags.bMagPhaseBPF=1;
	spectrum.SetType(specFlags);
	spectrum.SetSpectralRange(22050);
	spectrum.SetScale(CLAM::EScale::eLog);
	myFilterGenerator.Do(spectrum);
	
	myFilterGenerator.Stop();
}

void generateHighPassFilter( CLAM::Spectrum& spectrum )
{
	CLAM::FDFilterGenConfig myFilterGeneratorConfig;
	myFilterGeneratorConfig.SetType(CLAM::EFDFilterType::eHighPass);
	myFilterGeneratorConfig.SetSpectralRange(22050);
	myFilterGeneratorConfig.SetGain(1);
	myFilterGeneratorConfig.SetHighCutOff( 6000 );
	myFilterGeneratorConfig.SetPassBandSlope(14);
	
	CLAM::FDFilterGen myFilterGenerator;
	
	myFilterGenerator.Configure( myFilterGeneratorConfig );
	
	myFilterGenerator.Start();
	
	CLAM::SpecTypeFlags specFlags;
	
	specFlags.bMagPhase = 0;
	specFlags.bMagPhaseBPF=1;
	spectrum.SetType(specFlags);
	spectrum.SetSpectralRange(22050);
	spectrum.SetScale(CLAM::EScale::eLog);
	myFilterGenerator.Do(spectrum);
	
	myFilterGenerator.Stop();

}

void measureRandRandomness( CLAM::Array< CLAM::TData >& measure )
{
	srand( time( NULL ) );

	measure.Resize( 64 );
	measure.SetSize( 64 );
	
	for ( unsigned i = 0; i < 10000; i++ )
	{
		measure[ rand()%64 ]+=1.0f;
	}

	for ( int j = 0; j < measure.Size(); j++ )
	{
		measure[j] *= 1e-4;
	}
}
