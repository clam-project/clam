/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "Frame.hxx"
#include "Segment.hxx"
#include "Fundamental.hxx"
#include "SpectrumConfig.hxx"
#include "SMSAnalysisCore.hxx"

namespace CLAM
{

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*					SMSANALYSIS CONFIGURATION						*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////



void SMSAnalysisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SMSAnalysisConfig::DefaultValues()
{
	SetprSamplingRate(44100);

	GetSinSpectralAnalysis().SetWindowType(EWindowType::eHamming);
	GetResSpectralAnalysis().SetWindowType(EWindowType::eBlackmanHarris92);

	GetPeakDetect().SetMagThreshold(-60);
	
}


void SMSAnalysisConfig::SetSinWindowSize(TSize w)
{
	GetSinSpectralAnalysis().SetWindowSize(w);
}

TSize SMSAnalysisConfig::GetSinWindowSize() const
{
	return GetSinSpectralAnalysis().GetWindowSize();
}

/** Analysis Window type*/
void SMSAnalysisConfig::SetSinWindowType(const EWindowType& t)
{
	GetSinSpectralAnalysis().SetWindowType(t);
}

const EWindowType& SMSAnalysisConfig::GetSinWindowType() const
{
	return GetSinSpectralAnalysis().GetWindowType();
}

/** Zero padding factor*/
void SMSAnalysisConfig::SetSinZeroPadding(int z)
{
	GetSinSpectralAnalysis().SetZeroPadding(z);
}

int SMSAnalysisConfig::GetSinZeroPadding() const
{
	return GetSinSpectralAnalysis().GetZeroPadding();
}

void SMSAnalysisConfig::SetHopSize(TSize h)
{
	GetSinSpectralAnalysis().SetHopSize(h);
	GetResSpectralAnalysis().SetHopSize(h);
}

TSize SMSAnalysisConfig::GetHopSize() const
{
	return GetSinSpectralAnalysis().GetHopSize();
}

/*****Configuration for residual component analysis*****/

void SMSAnalysisConfig::SetResWindowSize(TSize w)
{
	GetResSpectralAnalysis().SetWindowSize(w);
	GetSynthSineSpectrum().SetSpectrumSize(GetResSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
}

TSize SMSAnalysisConfig::GetResWindowSize() const
{
	return GetResSpectralAnalysis().GetWindowSize();
}

/** Analysis Window type*/
void SMSAnalysisConfig::SetResWindowType(const EWindowType& t)
{
	GetResSpectralAnalysis().SetWindowType(t);
}

const EWindowType& SMSAnalysisConfig::GetResWindowType() const
{
	return GetResSpectralAnalysis().GetWindowType();
}

/** Zero padding factor*/
void SMSAnalysisConfig::SetResZeroPadding(int z)
{
	GetResSpectralAnalysis().SetZeroPadding(z);
}

int SMSAnalysisConfig::GetResZeroPadding() const
{
	return GetResSpectralAnalysis().GetZeroPadding();
}

/** Sampling rate of the input audio*/
void SMSAnalysisConfig::SetSamplingRate(TData sr)
{
	SetprSamplingRate(sr);
	GetSinSpectralAnalysis().SetSamplingRate(sr);
	GetResSpectralAnalysis().SetSamplingRate(sr);
	GetSynthSineSpectrum().SetSamplingRate(sr);
}

TData SMSAnalysisConfig::GetSamplingRate() const
{
	return TData(GetprSamplingRate());
}


TInt32 SMSAnalysisConfig::PowerOfTwo(TInt32 size)
{
	int tmp = size;
	int outputSize = 1;
	while (tmp) 
	{
	 	outputSize=outputSize << 1;
	 	tmp=tmp >> 1;
	}
	if(outputSize == size << 1)
		outputSize = outputSize >> 1;
	return outputSize;
}

TSize SMSAnalysisConfig::GetInitialOffset() const
{
	TSize largerWindowSize;
	if(GetSinWindowSize()>GetResWindowSize()) largerWindowSize=GetSinWindowSize();
	else largerWindowSize=GetResWindowSize();
			
	return -(largerWindowSize-1)/2+GetHopSize();
}

TSize SMSAnalysisConfig::GetHopsInBiggerWindow() const
{
	if(GetSinWindowSize()>GetResWindowSize()) return (GetSinWindowSize()-1)/GetHopSize();
	else return (GetResWindowSize()-1)/GetHopSize();
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*							SMSANALYSIS 							*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

SMSAnalysisCore::SMSAnalysisCore()
	: mInputAudio("Input Audio",this ),
	mOutputResSpectrum("Residual Spectral Analysis",this ),
	mOutputSinSpectrum("Sinusoidal Spectral Analysis",this ),
	mOutputSpectralPeaks("Sinusoidal Tracking",this ),
	mOutputFundamental("Fundamental",this ),
	mOutputSubstractedSpectrum("Substracted Spectral Analysis", this )
{
	AttachChildren();
	ConnectAndPublishPorts();
	Configure(SMSAnalysisConfig());
}

SMSAnalysisCore::SMSAnalysisCore(SMSAnalysisConfig& cfg)
	: mInputAudio("Input Audio",this ),
	mOutputResSpectrum("Residual Spectral Analysis",this ),
	mOutputSinSpectrum("Sinusoidal Spectral Analysis",this ),
	mOutputSpectralPeaks("Sinusoidal Tracking",this ),
	mOutputFundamental("Fundamental",this ),
	mOutputSubstractedSpectrum("Substracted Spectral Analysis", this )
{
	AttachChildren();
	ConnectAndPublishPorts();
	Configure(cfg);
}



SMSAnalysisCore::~SMSAnalysisCore()
{
}

bool SMSAnalysisCore::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConfig,cfg);
	ConfigureChildren();
	ConfigureData();
	return true;
}


bool SMSAnalysisCore::ConfigureChildren()
{
	mSinSpectralAnalysis.Configure(mConfig.GetSinSpectralAnalysis());
	mResSpectralAnalysis.Configure(mConfig.GetResSpectralAnalysis());
	mPeakDetect.Configure(mConfig.GetPeakDetect());
	mFundDetect.Configure(mConfig.GetFundFreqDetect());
	mSinTracking.Configure(mConfig.GetSinTracking());
	mSynthSineSpectrum.Configure(mConfig.GetSynthSineSpectrum());

	/*Initializing and configuring member circular buffers*/
		
	TSize frameSize=mConfig.GetHopSize();
	TSize sinWindowSize=mConfig.GetSinWindowSize();
	TSize resWindowSize=mConfig.GetResWindowSize();
	
	mInputAudio.SetSize( frameSize );
	mInputAudio.SetHop( frameSize );

	mSinSpectralAnalysis.GetInPort("Input").SetSize( sinWindowSize - 1 );
	mSinSpectralAnalysis.GetInPort("Input").SetHop( frameSize );
	mResSpectralAnalysis.GetInPort("Input").SetSize( resWindowSize - 1 );
	mResSpectralAnalysis.GetInPort("Input").SetHop( frameSize );

	return true;

}

void SMSAnalysisCore::ConfigureData()
{
	// XR: TODO - Get Sampling Rate from input audio?
	TData samplingRate=mConfig.GetSamplingRate();

	Spectrum resSpec;
	Spectrum sinSpec;

	// Spectrum used for temporary residual analysis
	SpectrumConfig scfg;
	scfg.SetSize(mConfig.GetResSpectralAnalysis().GetFFT().GetAudioSize()/2+1); // s.AudioFrameSize is the size of the generated frames
	scfg.SetSpectralRange(mConfig.GetSamplingRate()*0.5);
	resSpec.Configure(scfg);
	
	
	// Spectrum used for temporary sinusoidal analysis
	scfg.SetSize(mConfig.GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	sinSpec.Configure(scfg);
	
	/* Now we set prototype of SpectrumSubstracter: we want to substract two spectrums: 
	the first on in MagPhase format, the second in Complex format and get the result back
	in Mag Phase.*/

 	SpectrumConfig Scfg; 
 	SpecTypeFlags sflags;
   	sflags.bComplex = 1;
 	sflags.bPolar = 0;
 	sflags.bMagPhase = 0;
 	sflags.bMagPhaseBPF = 0;
 	Scfg.SetType(sflags);
 	Scfg.SetSize(resSpec.GetSize());
 	Scfg.SetSpectralRange(mConfig.GetSamplingRate()*0.5);
 	Spectrum tmpSpecIn(Scfg);    

	mSpecSubstracter.SetPrototypes(resSpec,tmpSpecIn,resSpec);
}

void SMSAnalysisCore::AttachChildren()
{
	mSinSpectralAnalysis.SetParent(this);
	mResSpectralAnalysis.SetParent(this);
	mPeakDetect.SetParent(this);
	mFundDetect.SetParent(this);
	mSinTracking.SetParent(this);
	mSpecSubstracter.SetParent(this);
}

bool SMSAnalysisCore::Do()
{
	if( mSinSpectralAnalysis.CanDoUsingPorts() )
	{
		mSinSpectralAnalysis.Do();
		mPeakDetect.Do();
		mFundDetect.Do();
		mSinTracking.Do();
		mSynthSineSpectrum.Do();
	}

	if( mResSpectralAnalysis.CanDoUsingPorts() )
	{
		mResSpectralAnalysis.Do();
	}
	
	if( mSpecSubstracter.CanDoUsingPorts() )
	{
		mSpecSubstracter.Do();
	}

}

void SMSAnalysisCore::ConnectAndPublishPorts()
{
	mInputAudio.PublishInPort( mSinSpectralAnalysis.GetInPort("Input"));
	mInputAudio.PublishInPort( mResSpectralAnalysis.GetInPort("Input"));
		
	mSinSpectralAnalysis.GetOutPort("Output").ConnectToIn( mPeakDetect.GetInPort("Input spectrum"));
	mPeakDetect.GetOutPort( "Output spectral peak array" ).ConnectToIn( mFundDetect.GetInPort("Input"));
	mPeakDetect.GetOutPort( "Output spectral peak array" ).ConnectToIn( mSinTracking.GetInPort( "Input" ));
	mFundDetect.GetOutControl( "Fund Freq Value").AddLink( &(mSinTracking.GetInControl( "Fund Freq Value" )));
	mSinTracking.GetOutPort( "Output").ConnectToIn( mSynthSineSpectrum.GetInPort( "Input" ));

	mResSpectralAnalysis.GetOutPort("Output").ConnectToIn( mSpecSubstracter.GetInPort( "Input 1"));
	mSynthSineSpectrum.GetOutPort("Output").ConnectToIn( mSpecSubstracter.GetInPort( "Input 2"));

	mOutputResSpectrum.PublishOutPort( mResSpectralAnalysis.GetOutPort("Output") );
	mOutputSinSpectrum.PublishOutPort( mSinSpectralAnalysis.GetOutPort("Output") );
	mOutputSpectralPeaks.PublishOutPort( mSinTracking.GetOutPort( "Output") );
	mOutputFundamental.PublishOutPort( mFundDetect.GetOutPort( "Output" ) );
	mOutputSubstractedSpectrum.PublishOutPort( mSpecSubstracter.GetOutPort( "Output") );
}

} // namespace CLAM

