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
#include "Factory.hxx"

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
		if(LoadSDIF(mConfig.GetFileName(),mSegment))
		{
			mInput2.Attach(mSegment);
			mHaveInternalSegment=true;
		}
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
	mPO_FrameInterpolator.Start();
	return true;
}

void SMSMorph::UpdateFrameInterpolatorFactors(bool useFrameFactor=false)
{
	if(useFrameFactor)
	{
		mPO_FrameInterpolator.mFrameInterpolationFactorCtl.DoControl(mHybBPF.GetLastValue());
	}
	else//No Global Factor
	{
		mPO_FrameInterpolator.mMagInterpolationFactorCtl.DoControl(mHybSinAmp.GetLastValue());
		mPO_FrameInterpolator.mFreqInterpolationFactorCtl.DoControl(mHybSinFreq.GetLastValue());
		mPO_FrameInterpolator.mPitchInterpolationFactorCtl.DoControl(mHybPitch.GetLastValue());
		mPO_FrameInterpolator.mResidualInterpolationFactorCtl.DoControl(mHybResAmp.GetLastValue());
	}
}

bool SMSMorph::Do(const Frame& in1, Frame& out)
{
	TSize nFrames2=mInput2.GetData().GetnFrames();
	TData synchroTimeFactor=mSynchronizeTime.GetLastValue()*nFrames2;
	
	if(mSynchronizeTime.GetLastValue()<0.0001||mSynchronizeTime.GetLastValue()>0.9999)
	{
		//it means we are at the boudaries of segment to morph
		out=in1;
		return true;
	}

	Frame tempFrame2;

	//With Frame Interpolation
	if(mConfig.GetInterpolateFrame())
	{
		FindInterpolatedFrameFromSegment2Morph(tempFrame2);
		//Morphing
		UpdateFrameInterpolatorFactors();
		mPO_FrameInterpolator.Do(in1,tempFrame2,out);
	}
	//Without Frame Interpolation
	else
	{
		UpdateFrameInterpolatorFactors();
		mPO_FrameInterpolator.Do(in1,mInput2.GetData().GetFrame(int(synchroTimeFactor)),out);
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
	
	//Interpolating
	TData frameFactor=synchroTimeFactor-frameNo1;
	mHybBPF.DoControl(frameFactor);
	UpdateFrameInterpolatorFactors(true);
	return mPO_FrameInterpolator.Do(mInput2.GetData().GetFrame(frameNo1) , mInput2.GetData().GetFrame(frameNo2) , interpolatedFrame);			
}

bool SMSMorph::Do(const Segment& in1, Segment& out)
{
	if(!mHaveInternalSegment) return false;
	return SMSTransformationTmpl<Frame>::Do(in1,out);
}

bool SMSMorph::Do(const Segment& in1,Segment& in2, Segment& out)
{
	mInput2.Attach(in2);
	return Do(in1,out);
}

bool SMSMorph::UpdateControlValueFromBPF(TData pos)
{
	bool ret=true;

	TData globalFactor;
	
	if(mConfig.HasHybBPF())
	{
		globalFactor=mConfig.GetHybBPF().GetValue(pos);
		mAmountCtrl.DoControl(mConfig.GetHybBPF().GetValue(pos));
		mHybBPF.DoControl(globalFactor);
	}
	else
		ret=false;
	if(mConfig.HasSynchronizeTime() && mConfig.GetSynchronizeTime().Size() )
	{
		pos=mConfig.GetSynchronizeTime().GetValue(pos);
		mSynchronizeTime.DoControl(pos);
	}

	if(mConfig.HasHybSinAmp() && mConfig.GetHybSinAmp().Size())
		mHybSinAmp.DoControl(mConfig.GetHybSinAmp().GetValue(pos));
	else
		mHybSinAmp.DoControl(globalFactor);

	if(mConfig.HasHybSinSpectralShape() && mConfig.GetHybSinSpectralShape().Size())
		mHybSinSpectralShape.DoControl(mConfig.GetHybSinSpectralShape().GetValue(pos));
	else
		mHybSinSpectralShape.DoControl(globalFactor);

	if(mConfig.HasHybSinShapeW1() && mConfig.GetHybSinShapeW1().Size())
		mHybSinShapeW1.DoControl(mConfig.GetHybSinShapeW1().GetValue(pos));
	else
		mHybSinShapeW1.DoControl(globalFactor);

	if(mConfig.HasHybSinShapeW2() && mConfig.GetHybSinShapeW2().Size())
		mHybSinShapeW2.DoControl(mConfig.GetHybSinShapeW2().GetValue(pos));	
	else
		mHybSinShapeW2.DoControl(globalFactor);

	if(mConfig.HasHybPitch() && mConfig.GetHybPitch().Size() )
		mHybPitch.DoControl(mConfig.GetHybPitch().GetValue(pos));
	else
		mHybPitch.DoControl(globalFactor);

	if(mConfig.HasHybSinFreq() && mConfig.GetHybSinFreq().Size())
		mHybSinFreq.DoControl(mConfig.GetHybSinFreq().GetValue(pos));
	else
		mHybSinFreq.DoControl(globalFactor);

	if(mConfig.HasHybResAmp() && mConfig.GetHybResAmp().Size() )
		mHybResAmp.DoControl(mConfig.GetHybResAmp().GetValue(pos));
	else
		mHybResAmp.DoControl(globalFactor);

	if(mConfig.HasHybResSpectralShape() && mConfig.GetHybResSpectralShape().Size())
		mHybResSpectralShape.DoControl(mConfig.GetHybResSpectralShape().GetValue(pos));
	else
		mHybResSpectralShape.DoControl(globalFactor);

	if(mConfig.HasHybResShapeW1() && mConfig.GetHybResShapeW1().Size())
		mHybResShapeW.DoControl(mConfig.GetHybResShapeW1().GetValue(pos));
	else
		mHybResShapeW.DoControl(globalFactor);

	return ret;
}

bool SMSMorph::LoadSDIF( std::string fileName, Segment& segment )
{
	SDIFInConfig cfg;
	cfg.SetMaxNumPeaks( 100 );
	cfg.SetFileName( fileName );
	cfg.SetEnableResidual( true );
	if(!mSDIFReader.Configure( cfg )) return false;//wrong filename or non-existing sdif
		
	segment.AddAll(  );
	segment.UpdateData(  );
	mSDIFReader.Output.Attach( segment );

	try{
		mSDIFReader.Start(  );}
	catch (Err)
	{
		return false;//wrong filename or non-existing sdif
	}
	while( mSDIFReader.Do() ) {  }
	mSDIFReader.Stop(  );
	
	return true;
}

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::SMSMorph> regtSMSMorph( "SMSMorph" );
