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
#include <iostream>
#include <fstream>
#include "SMSMorph.hxx"

using namespace CLAM;


SMSMorph::SMSMorph():
	mAmountCtrl("Amount",this),  
	mHybBPF("MorphFactor",this),
	mSynchronizeTime("Time", this),
	mHybSinAmp("SinAmp", this),
	mHybSinSpectralShape("SinShape", this),
	mHybSinShapeW1("SinShapeW1", this),
	mHybSinShapeW2("SinShapeW2", this),
	mHybPitch("Pitch", this),
	mHybSinFreq("SinFreq", this),
	mHybResAmp("ResAmp", this),
	mHybResSpectralShape("ResShape", this),
	mHybResShapeW("ResShapeW", this),
	mHybResPhase("ResPhase", this),
	mInput2("Input2",this,1)
{
		mHaveInternalSegment=false;
}

SMSMorph::SMSMorph(const SMSMorphConfig &c):
	mAmountCtrl("Amount",this),
	mHybBPF("MorphFactor",this),
	mSynchronizeTime("Time", this),
	mHybSinAmp("SinAmp", this),
	mHybSinSpectralShape("SinShape", this),
	mHybSinShapeW1("SinShapeW1", this),
	mHybSinShapeW2("SinShapeW2", this),
	mHybPitch("Pitch", this),
	mHybSinFreq("SinFreq", this),
	mHybResAmp("ResAmp", this),
	mHybResSpectralShape("ResShape", this),
	mHybResShapeW("ResShapeW", this),
	mHybResPhase("ResPhase", this),
	mInput2("Input2",this,1)
{
	mHaveInternalSegment=false;
	Configure(c);
}

bool SMSMorph::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
	mConfig=dynamic_cast<const SMSMorphConfig&>(c);
	mHaveInternalSegment=false;

	if(mConfig.HasFileName())
	{
		LoadSDIF(mConfig.GetFileName(),mSegment);
		mInput2.Attach(mSegment);
		mHaveInternalSegment=true;
	}
	
	if(!mConfig.HasHybBPF())
	{
		mConfig.AddHybBPF();
		mConfig.UpdateData();
		
		BPF tmpBPF(2);
		tmpBPF.SetValue(0,0);
		tmpBPF.SetXValue(0,0);
		tmpBPF.SetValue(1,1);
		tmpBPF.SetXValue(1,1);
		mConfig.SetHybBPF(tmpBPF);
	}
	if(!mConfig.HasInterpolateFrame())
	{
		mConfig.AddInterpolateFrame();
		mConfig.UpdateData();
		mConfig.SetInterpolateFrame(false);
	}
	if(!mConfig.HasHybSinFreq() && mConfig.HasHybBPF())
	{
		mConfig.AddHybSinFreq();
		mConfig.UpdateData();
		mConfig.SetHybSinFreq(mConfig.GetHybBPF());
	}
	if(!mConfig.HasHybSinAmp() && mConfig.HasHybBPF())
	{
		mConfig.AddHybSinAmp();
		mConfig.UpdateData();
		mConfig.SetHybSinAmp(mConfig.GetHybBPF());
	}
//	if(mConfig.HasHybSinSpectralShape())
//		mHarmSpectralShapeMorph=true;
	if(!mConfig.HasHybPitch())
	{
		mConfig.AddHybPitch();
		mConfig.UpdateData();
		mConfig.SetHybPitch(mConfig.GetHybSinFreq());
	}
	if(!mConfig.HasHybResAmp())
	{
		mConfig.AddHybResAmp();
		mConfig.UpdateData();
		mConfig.SetHybResAmp(mConfig.GetHybBPF());
	}
	if(!mConfig.HasSynchronizeTime())
	{
		mConfig.AddSynchronizeTime();
		mConfig.UpdateData();
		BPF tmpBPF(2);
		tmpBPF.SetXValue(0,0);
		tmpBPF.SetValue(0,0);
		tmpBPF.SetXValue(1,1);
		tmpBPF.SetValue(1,1);
		mConfig.SetSynchronizeTime(tmpBPF);
	}
	
	return UpdateControlValueFromBPF(0);
}

bool SMSMorph::ConcreteStart()
{
	mPO_SpectrumInterpolator.Start();
	return true;
}

bool SMSMorph::InterpolateSinusoidal(const Frame& in1,const Frame& in2, Frame& out, TData magFactor, TData freqFactor, TData pitchFactor)
{
		///////////////////////
	/*Sinusoidal Morphing*/
	///////////////////////
	TData pitch1=in1.GetFundamentalFreq();
	TData pitch2=in2.GetFundamentalFreq();

	TData newPitch=pitch1*(1-pitchFactor)+pitch2*pitchFactor;

	InterpolateSpectralPeaks(in1.GetSpectralPeakArray(),in2.GetSpectralPeakArray(),out.GetSpectralPeakArray(), magFactor, freqFactor, pitchFactor, pitch1,pitch2);
	//Sets new fund freq
	out.GetFundamental().SetnCandidates(0);
	if(mHarmonicMorph)
		out.GetFundamental().AddElem(0,newPitch);
	else
		out.GetFundamental().AddElem(0,0);
	return true;
}

bool SMSMorph::InterpolateResidual(const Frame& in1,const Frame& in2, Frame& out,TData resFactor)
{
	///////////////////////
	/**Residual Morphing**/
	///////////////////////
	
	/** Xavier: we cannot be sure that incoming spectrum is in MagPhase format.
	We will do it the "slow way" and try to optimize later (TODO)
	*/
	Spectrum &inRes1=in1.GetResidualSpec();
	Spectrum &inRes2=in2.GetResidualSpec();
	Spectrum &outRes=out.GetResidualSpec();
	
	outRes.SetSpectralRange(inRes1.GetSpectralRange());
	//TODO: should set prototypes at the beginning to enhance speed
	mPO_SpectrumInterpolator.Do(inRes1,inRes2,outRes);

/*	TSize specSize=out.GetResidualSpec().GetSize();

	int i;
	for(i=0;i<specSize;i++)
		outRes.SetMag(i,inRes1.GetMag(i)*(1-resFactor)+inRes2.GetMag(i)*resFactor);
	if(resFactor>0.5)
		for(i=0;i<specSize;i++)
			outRes.SetPhase(i,inRes2.GetPhase(i));
*/
	return true;
}


bool SMSMorph::InterpolateFrames(const Frame& in1,const Frame& in2, Frame& out, TData frameFactor=-1)
{
	if(in1.GetFundamentalFreq()!=0 && in2.GetFundamentalFreq()!=0 )
		mHarmonicMorph=true;
	else mHarmonicMorph=false;

	TData magFactor,freqFactor,pitchFactor,resFactor;
	
	if(frameFactor==-1)//No Frame Interpolation
	{
		frameFactor=mHybBPF.GetLastValue();
		magFactor=mHybSinAmp.GetLastValue();
		freqFactor=mHybSinFreq.GetLastValue();
		pitchFactor=mHybPitch.GetLastValue();
	}
	else
	{
		magFactor=freqFactor=pitchFactor=frameFactor;
	}
	
	InterpolateSinusoidal(in1,in2,out,magFactor, freqFactor, pitchFactor);
	
	resFactor=mHybResAmp.GetLastValue();
	InterpolateResidual(in1,in2,out,resFactor);
	
	return true;
}

bool SMSMorph::Do(const Frame& in1, Frame& out)
{
	TSize nFrames2=mInput2.GetData().GetnFrames();
	
	TData synchroTimeFactor=mSynchronizeTime.GetLastValue()*nFrames2;
	
	Frame tempFrame2;

	//With Frame Interpolation
	if(mConfig.GetInterpolateFrame())
	{
		FindInterpolatedFrameFromSegment2Morph(tempFrame2);
		//Morphing
		InterpolateFrames(in1,tempFrame2,out);
	}
	//Without Frame Interpolation
	else
	{
		InterpolateFrames(in1,mInput2.GetData().GetFrame(int(synchroTimeFactor)),out);
	}
					
	return true;


}

bool SMSMorph::FindInterpolatedFrameFromSegment2Morph(Frame& interpolatedFrame)
{
	TSize nFrames2=mInput2.GetData().GetnFrames();
	TData synchroTimeFactor=mSynchronizeTime.GetLastValue()*nFrames2;
	
	//Initializes interpolated frame 
	interpolatedFrame=mInput2.GetData().GetFrame(mInput2.GetData().mCurrentFrameIndex);
	//Interpolation data
	int frameNo1=floor(synchroTimeFactor);
	int frameNo2=ceil(synchroTimeFactor);
	
	if(mInput2.GetData().GetFrame(frameNo1).GetFundamentalFreq()!=0 && mInput2.GetData().GetFrame(frameNo2).GetFundamentalFreq()!=0 ) 
		mHarmonicMorph=true;

	//Interpolating
	TData frameFactor=synchroTimeFactor-frameNo1;
	return InterpolateFrames(mInput2.GetData().GetFrame(frameNo1) , mInput2.GetData().GetFrame(frameNo2) , interpolatedFrame, frameFactor);			
}

bool SMSMorph::Do(const Segment& in1, Segment& out)
{
	CLAM_ASSERT(mHaveInternalSegment, "SMSMorph::Do: you cannot call this overload if internal segment has not been previously lodade");
	TSize nFrames=in1.GetnFrames();
	int currentFrameIndex=in1.mCurrentFrameIndex;

	UpdateControlValueFromBPF((TData)currentFrameIndex/nFrames);
	mHarmonicMorph=false;
	
	return Do(in1.GetFrame(currentFrameIndex),out.GetFrame(currentFrameIndex));
}

bool SMSMorph::Do(const Segment& in1,Segment& in2, Segment& out)
{
	mInput2.Attach(in2);
	return Do(in1,out);
}


bool SMSMorph::FindHarmonic(const IndexArray& indexArray,int index,int& lastPosition)
{
	int i;
	bool found=false;
	int nPeaks=indexArray.Size();
	for(i=lastPosition;i<nPeaks;i++)
	{
		if(indexArray[i]==index)
		{
			lastPosition=i;
			found=true;
			break;
		}
	}
	return found;

}


bool SMSMorph::InterpolateSpectralPeaks(const SpectralPeakArray& in1,const SpectralPeakArray& in2, 
										SpectralPeakArray& out,
										TData magFactor, TData freqFactor, TData pitchFactor, 
										TData pitch1=0, TData pitch2=0)
{
	//we need to copy input peak arrays to convert them to linear
	SpectralPeakArray tmpIn1=in1;
	SpectralPeakArray tmpIn2=in2;
	tmpIn1.ToLinear();
	tmpIn2.ToLinear();

	int nPeaks1=in1.GetnPeaks();
	int nPeaks2=in2.GetnPeaks();

	if(nPeaks1==0)
	{
		out=in1;
		return true;
	}
	if(nPeaks2==0)
	{
		out=in1;
		return true;
	}

	//We initialize out with tmpIn1
	out=tmpIn1;
	
	DataArray& in1Mag=tmpIn1.GetMagBuffer();
	DataArray& in2Mag=tmpIn2.GetMagBuffer();
	DataArray& outMag=out.GetMagBuffer();

	DataArray& in1Freq=tmpIn1.GetFreqBuffer();
	DataArray& in2Freq=tmpIn2.GetFreqBuffer();
	DataArray& outFreq=out.GetFreqBuffer();

	IndexArray& in1Index=tmpIn1.GetIndexArray();
	IndexArray& in2Index=tmpIn2.GetIndexArray();
	
	//TODO: this computation is duplicated
	TData newPitch=pitch1*(1-pitchFactor)+pitch2*pitchFactor;
	
	TData factor2=nPeaks2/nPeaks1;
	int pos=0,i=0;
	do
	{
		if(!mHarmonicMorph)
		{
			outMag[i]=in1Mag[i]*(1-magFactor)+in2Mag[i*factor2]*magFactor;
			outFreq[i]=in1Freq[i]*(1-freqFactor)+in2Freq[i*factor2]*freqFactor;
		}
		else if(FindHarmonic(in2Index,in1Index[i],pos))
		{
			//Morphing Using Harmonic No*/
			outMag[i]=in1Mag[i]*(1-magFactor)+in2Mag[pos]*magFactor;
			outFreq[i]=((in1Freq[i]/pitch1)*(1-freqFactor)+(in2Freq[pos]/pitch2)*freqFactor)*newPitch;
		}
		else
		{
			outMag[i]=0.0000000001;
		}
		i++;
	}while(i<nPeaks1);
	
	//Finally we convert output to dB
	out.TodB();

	return true;
}


bool SMSMorph::LoadSDIF( std::string fileName, Segment& segment )
{
	SDIFInConfig cfg;
	cfg.SetMaxNumPeaks( 100 );
	cfg.SetFileName( fileName );
	cfg.SetEnableResidual( true );
	mSDIFReader.Configure( cfg );
		
	segment.AddAll(  );
	segment.UpdateData(  );
	mSDIFReader.Output.Attach( segment );

	try{
		mSDIFReader.Start(  );
		while( mSDIFReader.Do() ) {  }
		mSDIFReader.Stop(  );
	} catch (Err e)
	{
		std::cout << e.what() << std::endl;
	}

	return true;
}


bool SMSMorph::UpdateControlValueFromBPF(TData pos)
{
	bool ret=true;


	if(mConfig.HasHybBPF())
	{
		mAmountCtrl.DoControl(mConfig.GetHybBPF().GetValue(pos));
		mHybBPF.DoControl(mConfig.GetHybBPF().GetValue(pos));
	}
	else
		ret=false;
	if(mConfig.HasSynchronizeTime())
		mSynchronizeTime.DoControl(mConfig.GetSynchronizeTime().GetValue(pos));
	if(mConfig.HasHybSinAmp())
		mHybSinAmp.DoControl(mConfig.GetHybSinAmp().GetValue(pos));
	if(mConfig.HasHybSinSpectralShape())
		mHybSinSpectralShape.DoControl(mConfig.GetHybSinSpectralShape().GetValue(pos));
	if(mConfig.HasHybSinShapeW1())
		mHybSinShapeW1.DoControl(mConfig.GetHybSinShapeW1().GetValue(pos));
	if(mConfig.HasHybSinShapeW2())
		mHybSinShapeW2.DoControl(mConfig.GetHybSinShapeW2().GetValue(pos));	
	if(mConfig.HasHybPitch())
		mHybPitch.DoControl(mConfig.GetHybPitch().GetValue(pos));
	if(mConfig.HasHybSinFreq())
		mHybSinFreq.DoControl(mConfig.GetHybSinFreq().GetValue(pos));
	if(mConfig.HasHybResAmp())
		mHybResAmp.DoControl(mConfig.GetHybResAmp().GetValue(pos));
	if(mConfig.HasHybResSpectralShape())
		mHybResSpectralShape.DoControl(mConfig.GetHybResSpectralShape().GetValue(pos));
	if(mConfig.HasHybResShapeW1())
		mHybResShapeW.DoControl(mConfig.GetHybResShapeW1().GetValue(pos));
	if(mConfig.HasHybResPhase())
		mHybResPhase.DoControl(mConfig.GetHybResPhase().GetValue(pos));
	return ret;
}