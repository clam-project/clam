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

#include "SMSAnalysis.hxx"
//test
//#include <iostream>
//#include "AudioSnapshot.hxx"


using namespace CLAM;

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

	GetPeakDetect().SetNumBands((GetSinWindowSize()-1)/2+1);

	GetPeakDetect().SetMaxPeaks(50);
	GetPeakDetect().SetMagThreshold(-60);
	
}


void SMSAnalysisConfig::SetSinWindowSize(TSize w)
{
	GetSinSpectralAnalysis().SetWindowSize(w);
	GetPeakDetect().SetNumBands(GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);
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
	GetPeakDetect().SetNumBands(GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
}

int SMSAnalysisConfig::GetSinZeroPadding() const
{
	return GetSinSpectralAnalysis().GetZeroPadding();
}

void SMSAnalysisConfig::SetHopSize(TSize h)
{
	GetSinSpectralAnalysis().SetHopSize(h);
	GetResSpectralAnalysis().SetHopSize(h);
	TSize w;
	if (GetSinWindowSize()>GetResWindowSize()) w=GetSinWindowSize();
	else w=GetResWindowSize();

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
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);
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

TSize SMSAnalysisConfig::GetInitialOffset()
{
	TSize largerWindowSize;
	if(GetSinWindowSize()>GetResWindowSize()) largerWindowSize=GetSinWindowSize();
	else largerWindowSize=GetResWindowSize();
			
	return -(largerWindowSize-1)/2+GetHopSize();
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*							SMSANALYSIS 							*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

SMSAnalysis::SMSAnalysis()
{
	AttachChildren();
	Configure(SMSAnalysisConfig());
}

SMSAnalysis::SMSAnalysis(SMSAnalysisConfig& cfg)
{
	AttachChildren();
	Configure(cfg);
}

SMSAnalysis::~SMSAnalysis()
{
}

bool SMSAnalysis::ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast)
{
	mConfig=dynamic_cast<const SMSAnalysisConfig&> (cfg);
	ConfigureChildren();
	ConfigureData();
	return true;
}


bool SMSAnalysis::ConfigureChildren()
{
	mPO_SinSpectralAnalysis.Configure(mConfig.GetSinSpectralAnalysis());
	mPO_ResSpectralAnalysis.Configure(mConfig.GetResSpectralAnalysis());
	mPO_PeakDetect.Configure(mConfig.GetPeakDetect());
	mPO_FundDetect.Configure(mConfig.GetFundFreqDetect());
	mPO_SinTracking.Configure(mConfig.GetSinTracking());
	mPO_SynthSineSpectrum.Configure(mConfig.GetSynthSineSpectrum());
	
	return true;

}

void SMSAnalysis::ConfigureData()
{
	TData samplingRate=mConfig.GetSamplingRate();

	// Spectrum used for temporary residual analysis
	SpectrumConfig scfg;
	scfg.SetSize(mConfig.GetResSpectralAnalysis().GetFFT().GetAudioSize()/2+1); // s.AudioFrameSize is the size of the generated frames
	scfg.SetSpectralRange(mConfig.GetSamplingRate()*0.5);
	mResSpec.Configure(scfg);
	// Spectrum used for temporary sinusoidal analysis
	scfg.SetSize(mConfig.GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	mSinSpec.Configure(scfg);
	
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
 	Scfg.SetSize(mResSpec.GetSize());
 	Scfg.SetSpectralRange(mConfig.GetSamplingRate()*0.5);
 	Spectrum tmpSpecIn(Scfg);    

	mPO_SpecSubstract.SetPrototypes(mResSpec,tmpSpecIn,mResSpec);

	
	// Fundamental
	mFund.AddCandidatesFreq();
	mFund.AddCandidatesErr();
	mFund.UpdateData();
	mFund.SetnMaxCandidates(1); // number of candidates wanted

	/*Initializing and configuring member circular buffers*/
	//First sinusoidal circular buffer
	
	//TODO: both hopsizes are supposed to be equal!
	
	TSize sinBufferSize=mConfig.GetSinSpectralAnalysis().GetWindowSize()-1+mConfig.GetSinSpectralAnalysis().GetHopSize();
	TSize resBufferSize=mConfig.GetResSpectralAnalysis().GetWindowSize()-1+mConfig.GetResSpectralAnalysis().GetHopSize();

	TSize bufferSize;
	if(sinBufferSize>resBufferSize) bufferSize=sinBufferSize;
	else	bufferSize=resBufferSize;

	mSinCircularBuffer.SetBufferSize(bufferSize);
	mSinCircularBuffer.SetReadSize(mConfig.GetSinSpectralAnalysis().GetWindowSize()-1);
	mSinCircularBuffer.SetWriteSize(mConfig.GetSinSpectralAnalysis().GetHopSize());
	mSinCircularBuffer.Init();
	mSinCircularBuffer.IncreaseWriteIndex(bufferSize-2*mConfig.GetSinSpectralAnalysis().GetHopSize());
	mSinCircularBuffer.IncreaseReadIndex(((bufferSize-mConfig.GetSinSpectralAnalysis().GetHopSize()+1)-(mConfig.GetSinSpectralAnalysis().GetWindowSize()))/2);

	//now residual circular buffer
	//if(mConfig.GetResBufferSize()==-1) mConfig.SetResBufferSize(mConfig.GetResSpectralAnalysis().GetWindowSize()-1+mConfig.GetResSpectralAnalysis().GetHopSize());
	mResCircularBuffer.SetBufferSize(bufferSize);
	mResCircularBuffer.SetReadSize(mConfig.GetResSpectralAnalysis().GetWindowSize()-1);
	mResCircularBuffer.SetWriteSize(mConfig.GetResSpectralAnalysis().GetHopSize());
	mResCircularBuffer.Init();
	mResCircularBuffer.IncreaseWriteIndex(bufferSize-2*mConfig.GetResSpectralAnalysis().GetHopSize());
	mResCircularBuffer.IncreaseReadIndex(((bufferSize-mConfig.GetResSpectralAnalysis().GetHopSize()+1)-(mConfig.GetResSpectralAnalysis().GetWindowSize()))/2);

	//let's try to use streambuffers
/*	mSinWriter=mSinStreamBuffer.NewWriter(mConfig.GetSinSpectralAnalysis().GetHopSize(),mConfig.GetSinSpectralAnalysis().GetHopSize());
	mResWriter=mResStreamBuffer.NewWriter(mConfig.GetResSpectralAnalysis().GetHopSize(),mConfig.GetResSpectralAnalysis().GetHopSize());
	mSinReader=mSinStreamBuffer.NewReader(mConfig.GetSinSpectralAnalysis().GetHopSize(),mConfig.GetSinSpectralAnalysis().GetWindowSize()-1);
	mResReader=mResStreamBuffer.NewReader(mConfig.GetResSpectralAnalysis().GetHopSize(),mConfig.GetResSpectralAnalysis().GetWindowSize()-1);
	//I don't understand this number (windowsize*5) taken from the example, could be optimized using the same as circular buffer?
	mSinStreamBuffer.Configure(mConfig.GetSinSpectralAnalysis().GetWindowSize()*5);
	mResStreamBuffer.Configure(mConfig.GetResSpectralAnalysis().GetWindowSize()*5);
*/
	mWriter=mStreamBuffer.NewWriter(mConfig.GetSinSpectralAnalysis().GetHopSize(),mConfig.GetSinSpectralAnalysis().GetHopSize());
	mSinReader=mStreamBuffer.NewReader(mConfig.GetSinSpectralAnalysis().GetHopSize(),mConfig.GetSinSpectralAnalysis().GetWindowSize()-1);
	mResReader=mStreamBuffer.NewReader(mConfig.GetResSpectralAnalysis().GetHopSize(),mConfig.GetResSpectralAnalysis().GetWindowSize()-1);
	//I don't understand this number (windowsize*5) taken from the example, could be optimized using the same as circular buffer?
	mStreamBuffer.Configure(mConfig.GetSinSpectralAnalysis().GetWindowSize()*2);

	//We will now write biggestWindowSize/2 zeros so we have the first half of the window full
	Audio tmpAudio,tmpAudio2;
	tmpAudio2.SetSize(mConfig.GetSinSpectralAnalysis().GetHopSize());
	TSize hopsInHalfWindow=0.5*mConfig.GetSinSpectralAnalysis().GetWindowSize()/mConfig.GetSinSpectralAnalysis().GetHopSize();
	int i;
 	for(i=0;i<hopsInHalfWindow;i++)
	{
		mStreamBuffer.GetAndActivate(mWriter,tmpAudio);
		tmpAudio.GetBuffer()=tmpAudio2.GetBuffer();
		mStreamBuffer.LeaveAndAdvance(mWriter);
	}


	//configure internal audio members used for convinience
	//mSinAudioFrame.SetSize(mConfig.GetSinSpectralAnalysis().GetWindowSize()-1);
	mSinAudioFrame.SetSampleRate(mConfig.GetSamplingRate());
	//mResAudioFrame.SetSize(mConfig.GetResSpectralAnalysis().GetWindowSize()-1);
	mResAudioFrame.SetSampleRate(mConfig.GetSamplingRate());

	//now we will Attach input and output ports of Spectral Analysis
	mPO_SinSpectralAnalysis.Attach(mSinAudioFrame,mSinSpec);
	mPO_ResSpectralAnalysis.Attach(mResAudioFrame,mResSpec);

	mInitialOffset=0;
	mEndingOffset=0;
}

void SMSAnalysis::AttachChildren()
{
	mPO_SinSpectralAnalysis.SetParent(this);
	mPO_ResSpectralAnalysis.SetParent(this);
	mPO_PeakDetect.SetParent(this);
	mPO_FundDetect.SetParent(this);
	mPO_SinTracking.SetParent(this);
	mPO_SpecSubstract.SetParent(this);
}

bool SMSAnalysis::Do(Audio& in, Spectrum& outGlobalSpec,SpectralPeakArray& outPk,Fundamental& outFn,Spectrum& outResSpec,Spectrum& outSinSpec)
{
	//Synchronizing spectral ranges of other spectrums
	outGlobalSpec.SetSpectralRange(mResSpec.GetSpectralRange());
	outSinSpec.SetSpectralRange(mResSpec.GetSpectralRange());
	outResSpec.SetSpectralRange(mResSpec.GetSpectralRange());
	
	/* Input audio frame is writen onto circular buffer */
//	mSinCircularBuffer.WriteAudio(in);
	/* WindowSize-1 samples are read and put into helper mAudioFrame data */
//	mSinCircularBuffer.NonCopyReadAudio(mSinAudioFrame);
	/* Read index is then decreased according to hop size */
//	mSinCircularBuffer.DecreaseReadIndex(mConfig.GetSinSpectralAnalysis().GetWindowSize()-mConfig.GetSinSpectralAnalysis().GetHopSize()-1);


	//OK, let's try using these stream buffers
	mStreamBuffer.GetAndActivate(mSinReader,mSinAudioFrame);

	//test
	//mSinAudioFrame.SetSampleRate(mConfig.GetSamplingRate());
	//showSnapshotAudio(mSinAudioFrame,"SinusoidalFrame");
	
	//Analyzing sinusoidal component
	mPO_SinSpectralAnalysis.Do();

	//OK, let's try using these stream buffers
	mStreamBuffer.LeaveAndAdvance(mSinReader);
	outGlobalSpec=mSinSpec;
	SinusoidalAnalysis(mSinSpec,outPk,outFn);
	
	
	//Analyzing residual component
	
	/* Input audio frame is writen onto circular buffer */
//	mResCircularBuffer.WriteAudio(in);
	/* WindowSize-1 samples are read and put into helper mAudioFrame data */
//	mResCircularBuffer.NonCopyReadAudio(mResAudioFrame);
	/* Read index is then decreased according to hop size */
//	mResCircularBuffer.DecreaseReadIndex(mConfig.GetResSpectralAnalysis().GetWindowSize()-mConfig.GetResSpectralAnalysis().GetHopSize()-1);
	
	//First we synthesize Sinusoidal Spectrum
	mPO_SynthSineSpectrum.Do(outPk,outSinSpec);
	
	//Then we analyze the spectrum of the whole audio using residual config
	
    //stream buffers, now for residual
	mStreamBuffer.GetAndActivate(mResReader,mResAudioFrame);
	mPO_ResSpectralAnalysis.Do();
	//test
	//mResAudioFrame.SetSampleRate(mConfig.GetSamplingRate());
	//showSnapshotAudio(mResAudioFrame,"ResidualFrame");
	//end test
	
	mStreamBuffer.LeaveAndAdvance(mResReader);


	//Finally we substract mSpectrum-SinusoidalSpectrum
	
	outResSpec.SetSize(mResSpec.GetSize());
	mPO_SpecSubstract.Do(mResSpec,outSinSpec,outResSpec);



	return true;

}


bool SMSAnalysis::SinusoidalAnalysis(Spectrum& outSp, SpectralPeakArray& pkArray,Fundamental& outFn)
{
	      
	// Convert Spectrum to dB
	outSp.ToDB();
	
	// Peak Detection
	SpectralPeakArray tmpPk;
	mPO_PeakDetect.Do(outSp,tmpPk);

	// If not possible to detect anything with this peak information, FundDetect will return a false
	bool fundFreqFound = mPO_FundDetect.Do(tmpPk,outFn);

	// Sinusoidal Tracking
	mPO_SinTracking.Do(tmpPk,pkArray,outFn.GetFreq(0));
	
	// Convert Spectrum back to linear
	outSp.ToLinear();
	return true;
}


bool SMSAnalysis::Do(Frame& in)
{
	static count=0;
	in.AddResidualAudioFrame();
	in.AddSpectrum();
	in.AddSpectralPeakArray();
	in.AddFundamental();
	in.AddSinusoidalSpec();
	in.AddResidualSpec();
	in.AddIsHarmonic();
	in.UpdateData();

	Spectrum tmpSpec;
	tmpSpec.SetSize(mConfig.GetSinSpectralAnalysis().GetFFT().GetAudioSize()/2+1);
	in.SetSpectrum(tmpSpec);
	in.SetFundamental(mFund);

	//OK, let's try using these stream buffers
	Audio tmpAudio;
	mStreamBuffer.GetAndActivate(mWriter,tmpAudio);
	tmpAudio.GetBuffer()=in.GetAudioFrame().GetBuffer();
	
	//test
	//tmpAudio.SetSampleRate(mConfig.GetSamplingRate());
	//showSnapshotAudio(tmpAudio,"inputAudioFrame");
	
	mStreamBuffer.LeaveAndAdvance(mWriter);

	//Residual stream buffer
/*	Audio tmpAudio2;
	mResStreamBuffer.GetAndActivate(mResWriter,tmpAudio2);
	tmpAudio2.GetBuffer()=in.GetAudioFrame().GetBuffer();
	mResStreamBuffer.LeaveAndAdvance(mResWriter);
*/

	

	bool result=false;

	
	if (count>8)
	{
		if(count==9)//have to advance residual reader so center time is the same as sinusoidal
		{
			/*mResStreamBuffer.GetAndActivate(mResReader,mResAudioFrame);
			mResStreamBuffer.LeaveAndAdvance(mResReader);
			mResStreamBuffer.GetAndActivate(mResReader,mResAudioFrame);
			mResStreamBuffer.LeaveAndAdvance(mResReader);*/
			mStreamBuffer.GetAndActivate(mResReader,mResAudioFrame);
			mStreamBuffer.LeaveAndAdvance(mResReader);
			mStreamBuffer.GetAndActivate(mResReader,mResAudioFrame);
			mStreamBuffer.LeaveAndAdvance(mResReader);
		}
		result=Do(in.GetAudioFrame(),in.GetSpectrum(),in.GetSpectralPeakArray(),in.GetFundamental(),in.GetResidualSpec(),in.GetSinusoidalSpec());
		in.SetIsHarmonic(in.GetFundamental().GetFreq(0)>0);
	}
	else
	{
		mInitialOffset++;
		mEndingOffset++;

	}
	count++;
	return result;
}

bool SMSAnalysis::Do(Segment& in)
{
	int frameIndex=in.mCurrentFrameIndex;

	int step=mConfig.GetHopSize();
	int sinFrameSize=mConfig.GetSinSpectralAnalysis().GetWindowSize()-1;
	int resFrameSize=mConfig.GetResSpectralAnalysis().GetWindowSize()-1;


	TData samplingRate=mConfig.GetSamplingRate();
	
	TSize audioCenterSample=(mInitialOffset+frameIndex)*step;
	TTime audioCenterTime=audioCenterSample/samplingRate;

	TTime frameCenterTime=frameIndex*step/samplingRate;
	
	//Adding a new frame to segment, this frame will have the audiochunk as audioframe
	Frame tmpFrame;
	tmpFrame.SetDuration(step/samplingRate);
	
	/*	Note: if center time is negative, it should not be used in synthesis. Here the frame
		is passed just for filling up the analysis input circular buffer. */
	tmpFrame.SetCenterTime(TData(frameCenterTime));
	tmpFrame.AddAudioFrame();
	tmpFrame.UpdateData();
	Audio tmpAudio;
	tmpAudio.SetBeginTime(frameIndex*step);
	tmpAudio.SetSampleRate(in.GetAudio().GetSampleRate());
	
	/*	Note: Here we are just taking the "new" audio belonging to each frame. That is, the
	HopSize samples centered around CenterTime */
	in.GetAudio().GetAudioChunk(audioCenterSample-step/2,audioCenterSample+step/2,tmpAudio,true);

	//test
	//showSnapshotAudio(tmpAudio,"Original audio chunk");
	
	tmpFrame.SetAudioFrame(tmpAudio);

	bool hasProcessed=Do(tmpFrame);
	if(hasProcessed){
		in.mCurrentFrameIndex++;
		in.AddFrame(tmpFrame);
	}

	bool result=true;
	if(tmpAudio.GetSize()<step){//have reached end of input audio
		mEndingOffset--;
		if(mEndingOffset==0) result=false;}
	return result;
}



