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

#include "SpectrumConfig.hxx"
#include "SMSSynthesis.hxx"

namespace CLAM
{

void SMSSynthesis::AttachChildren()
{
	mPO_SpectralSynthesis.SetParent(this);
	mPO_ResSpectralSynthesis.SetParent(this);
	mPO_SinSpectralSynthesis.SetParent(this);
	mPO_SynthSineSpectrum.SetParent(this);
	mPO_PhaseMan.SetParent(this);
	mPO_SpectrumAdder.SetParent(this);
	mPO_OverlapAddSin.SetParent(this);
	mPO_OverlapAddRes.SetParent(this);
	mPO_OverlapAddGlobal.SetParent(this);
}

SMSSynthesis::SMSSynthesis():
mInputSinSpectralPeaks("InputSinPeaks",this),
mInputResSpectrum("InputResSpectrum",this),
mOutputSinSpectrum("OutputSinSpectrum",this),
mOutputSpectrum("OutputSpectrum",this),
mOutputAudio("OutputAudio",this),
mOutputResAudio("OutputResAudio",this),
mOutputSinAudio("OutputSinAudio",this),
mCurrentTime("CurrentTime",this),
mCurrentPitch("CurrentPitch",this)
{
	Configure(SMSSynthesisConfig());
	AttachChildren();
}

SMSSynthesis::SMSSynthesis(const SMSSynthesisConfig& cfg):
mInputSinSpectralPeaks("InputSinPeaks",this),
mInputResSpectrum("InputResSpectrum",this),
mOutputSinSpectrum("OutputSinSpectrum",this),
mOutputSpectrum("OutputSpectrum",this),
mOutputAudio("OutputAudio",this),
mOutputResAudio("OutputResAudio",this),
mOutputSinAudio("OutputSinAudio",this),
mCurrentTime("CurrentTime",this),
mCurrentPitch("CurrentPitch",this)
{
	Configure(cfg);
	AttachChildren();
}

SMSSynthesis::~SMSSynthesis()
{
	
}

bool SMSSynthesis::ConfigureChildren()
{
//	mConfig.SetSpectrumSize( mAudioFrame.GetSize()/2+1 );

	//configure global spectral synthesis
	if(!mPO_SpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

	//configure residual spectral synthesis
	mConfig.GetSpectralSynth().SetResidual(true);
	if(!mPO_ResSpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

	//configure sinusoidal spectral synthesis
	mConfig.GetSpectralSynth().SetResidual(false);
	if(!mPO_SinSpectralSynthesis.Configure(mConfig.GetSpectralSynth()))
		return false;

	//SynthSineSpectrum
	if(!mPO_SynthSineSpectrum.Configure(mConfig.GetSynthSineSpectrum()))
		return false;

	//Phase Management
	if(!mPO_PhaseMan.Configure(mConfig.GetPhaseMan()))
		return false;

	mPO_PhaseMan.Init();
	
	//Spectrum Adder
	if(!mPO_SpectrumAdder.Configure(SpecAdder2Config()))
		return false;

	//Overlap and add PO
	if(!mPO_OverlapAddSin.Configure(mConfig.GetOverlapAddSin()))
		return false;
	if(!mPO_OverlapAddRes.Configure(mConfig.GetOverlapAddRes()))
		return false;
	if(!mPO_OverlapAddGlobal.Configure(mConfig.GetOverlapAddGlobal()))
		return false;

	return true;
}

void SMSSynthesis::ConfigureData()
{
	mAudioFrame.SetSize(mConfig.GetHopSize()*2);//audio used as input of the overlap and add
	mOutputAudio.SetSize( mAudioFrame.GetSize()/2 );
	mOutputSinAudio.SetSize( mAudioFrame.GetSize()/2 );
	mOutputResAudio.SetSize( mAudioFrame.GetSize()/2 );

	mOutputAudio.SetHop( mConfig.GetHopSize() );
	mOutputSinAudio.SetHop( mConfig.GetHopSize() );
	mOutputResAudio.SetHop( mConfig.GetHopSize() );

	mOutputSpectrum.GetData().SetSize( mAudioFrame.GetSize()/2+1);
	mOutputSinSpectrum.GetData().SetSize( mAudioFrame.GetSize()/2+1);
}


bool SMSSynthesis::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	//CONFIGURE CHILDREN AND DATA
	ConfigureChildren();

	ConfigureData();
	return true;
}

bool SMSSynthesis::SinusoidalSynthesis(const SpectralPeakArray& in,Audio& out)
{
	SpectrumConfig tmpcfg;
	tmpcfg.SetScale(EScale::eLinear);
	Spectrum tmpSpec(tmpcfg);
	return SinusoidalSynthesis(in,tmpSpec,out);
	
}

/** Sinusoidal synthesis, gives also the output spectrum */
bool SMSSynthesis::SinusoidalSynthesis(const SpectralPeakArray& in,Spectrum& outSpec,Audio& outAudio)
{
//	outSpec.SetSize(mConfig.GetSpectrumSize());
	mPO_SynthSineSpectrum.Do(in,outSpec);

	mPO_SinSpectralSynthesis.Do(outSpec,mAudioFrame);
	//Finally the overlap and add is accomplished
	
	return mPO_OverlapAddSin.Do(mAudioFrame, outAudio);

}


bool SMSSynthesis::Do(void)
{
	bool result =  Do(mInputSinSpectralPeaks.GetData(),mInputResSpectrum.GetData(),
		mOutputSinSpectrum.GetData(),mOutputSpectrum.GetData(),
		mOutputAudio.GetAudio(),mOutputSinAudio.GetAudio(),mOutputResAudio.GetAudio());


	
	mInputSinSpectralPeaks.Consume();
	mInputResSpectrum.Consume();

	mOutputSinSpectrum.Produce();
	mOutputSpectrum.Produce();
	mOutputAudio.Produce();
	mOutputSinAudio.Produce();
	mOutputResAudio.Produce();


	return result;
}


bool SMSSynthesis::Do(SpectralPeakArray& inputSinusoidalPeaks, Spectrum& inputResidualSpectrum, 
			Audio& outputAudio, Audio& outputSinusoidalAudio, Audio& outputResidualAudio)
{
	//This may need to be initialized?
	Spectrum tmpOutputSinSpec;
	Spectrum tmpOutputSpec;
	
	return Do(inputSinusoidalPeaks,inputResidualSpectrum,tmpOutputSinSpec,tmpOutputSpec,
		outputAudio,outputSinusoidalAudio,outputResidualAudio);

}

bool SMSSynthesis::Do(SpectralPeakArray& inputSinusoidalPeaks,Spectrum& inputResidualSpectrum,
		Spectrum& outputSinusoidalSpectrum,	Spectrum& outputSpectrum,
		Audio& outputAudio, Audio& outputSinusoidalAudio, Audio& outputResidualAudio)
{
	//First we do the phase managing. Note that if the Do(frame) overload is not used,
	//the time and pitch controls in this processing should be set by hand before this
	//method is used
	mPO_PhaseMan.mCurrentTime.DoControl(mCurrentTime.GetLastValue());
	mPO_PhaseMan.mCurrentPitch.DoControl(mCurrentPitch.GetLastValue());
	mPO_PhaseMan.Do(inputSinusoidalPeaks);

	//We synthesize the sinusoidal component 	
	SinusoidalSynthesis(inputSinusoidalPeaks,outputSinusoidalSpectrum,outputSinusoidalAudio);
	
	outputSpectrum.SetSize( inputResidualSpectrum.GetSize() );
	
	//We add Residual spectrum in the input frame plus the synthesized sinusoidal spectrum
	mPO_SpectrumAdder.Do(outputSinusoidalSpectrum,inputResidualSpectrum,outputSpectrum);
	
	//We synthesize to audio the resulting summed spectrum
	mPO_SpectralSynthesis.Do(outputSpectrum,mAudioFrame);


	//We do the overlap and add
	mPO_OverlapAddGlobal.Do(mAudioFrame, outputAudio);



	//Now we synthesize only the residual spectrum
	mPO_ResSpectralSynthesis.Do(inputResidualSpectrum,mAudioFrame);
	//And we do the overlap and add process for the residual

	
	mPO_OverlapAddRes.Do(mAudioFrame, outputResidualAudio);

	/* Note: although sinusoidal spectrum is already available from the analysis phase, we 
	need to store it again in the frame because the original peak array may have been
	transformed
	*/
	return true;
}

bool SMSSynthesis::Do(Frame& in)
{
	if(in.GetCenterTime()<0) return false;//such frames should not be synthesized	
	
//We initialize input frame, adding necessary attributes
	InitFrame(in);
//First we set the controls 
	mCurrentTime.DoControl(in.GetCenterTime());
	mCurrentPitch.DoControl(in.GetFundamental().GetFreq(0));
	

//We make sure that spectrums in input frame has the appropiate size and spectral range.
//Note that the only spectrum we can be sure has the correct spectral range is the residual 
//because it its the only one that has been stored in the analysis process.
	in.GetOutSpec().SetSize(mConfig.GetSpectrumSize());
	in.GetOutSpec().SetSpectralRange(in.GetResidualSpec().GetSpectralRange());
	in.GetSinusoidalSpec().SetSpectralRange(in.GetResidualSpec().GetSpectralRange());

	return Do(in.GetSpectralPeakArray(),in.GetResidualSpec(),in.GetSinusoidalSpec(),in.GetOutSpec(),
		in.GetSynthAudioFrame(),in.GetSinusoidalAudioFrame(),in.GetResidualAudioFrame());

}

bool SMSSynthesis::Do(Segment& in)
{
	return Do(in.GetFrame(in.mCurrentFrameIndex++));
}


void SMSSynthesis::InitFrame(Frame& in)
{
	in.AddOutSpec();
	in.AddSinusoidalSpec();
	in.AddSinusoidalAudioFrame();
	in.AddResidualAudioFrame();
	in.AddSynthAudioFrame();
	in.UpdateData();

	in.GetSinusoidalAudioFrame().SetSize(mConfig.GetFrameSize());
	in.GetResidualAudioFrame().SetSize(mConfig.GetFrameSize());
	in.GetSynthAudioFrame().SetSize(mConfig.GetFrameSize());
	
}

} // namespace CLAM

