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

#include "SMSTimeStretch.hxx"
#include "Factory.hxx"

using namespace CLAM;

SMSTimeStretch::SMSTimeStretch()
{
	mSynthesisTime=0;
	mAnalysisTime=0;
	mIndex=-1;
	//@todo: this should not be hardwired!
	mHopSize=256;
	mSampleRate=22050;
}

bool SMSTimeStretch::ConcreteConfigure(const ProcessingConfig& cfg)
{
	SMSTransformationTmpl<Frame>::ConcreteConfigure(cfg);
	mPO_FrameInterpolator.Configure(FrameInterpConfig());
	return true;
}

bool SMSTimeStretch::ConcreteStart()
{
	mnSynthesisFrames=0;
	mPO_FrameInterpolator.Start();
	return true;
}

bool SMSTimeStretch::Do(const Frame& in, Frame& out)
{
	TData interpFactor= (mAnalysisTime-mLeftFrame.GetCenterTime())/(mHopSize/mSampleRate);
	out.SetCenterTime(mSynthesisTime);
	mSynthesisTime+=(TData)mHopSize/mSampleRate;
	if(interpFactor>1.01)
	{
		out.GetSpectralPeakArray().SetnPeaks(0);
		Spectrum& tmpSpec=out.GetResidualSpec();
		int i;
		int specSize=tmpSpec.GetSize();
		TData value;
		if(tmpSpec.GetScale()==EScale::eLinear)
			value=0.000000001;
		else value=-200;
		for(i=0;i<specSize;i++)
			tmpSpec.SetMag(i,value);
		return true;
	}
	else
	{
		mnSynthesisFrames++;
	}
	mPO_FrameInterpolator.mFrameInterpolationFactorCtl.DoControl(interpFactor);
	mPO_FrameInterpolator.Do(in,mLeftFrame,out);
	
	return true;
}


bool SMSTimeStretch::Do(const Segment& in, Segment& out)
{
	if(mIndex>-1)
	{
		while(mIndex<in.mCurrentFrameIndex&&!HaveFinished())
		{
			SMSTransformationTmpl<Frame>::Do(in,out);
		}
	}
	else mIndex++;
	return true;
}

void SMSTimeStretch::UpdateTimeAndIndex(const Segment& in)
{
	mAnalysisTime+=(TData)mHopSize*mAmountCtrl.GetLastValue()/mSampleRate;
	while(mAnalysisTime>mLeftFrame.GetCenterTime()+mHopSize/mSampleRate&&mIndex<=in.GetnFrames())
	{
		mLeftFrame=in.GetFrame(mIndex);
		mIndex++;
	}
}

const Frame& SMSTimeStretch::UnwrapProcessingData(const Segment& in,Frame*)
{
	UpdateTimeAndIndex(in);	
	return in.GetFrame(mIndex);
}

Frame& SMSTimeStretch::UnwrapProcessingData(Segment& out,Frame*)
{
	if(mnSynthesisFrames>out.GetnFrames())
		out.AddFrame(out.GetFrame(mnSynthesisFrames-1));
	return out.GetFrame(mnSynthesisFrames);
}

bool SMSTimeStretch::HaveFinished()
{
	return mIndex>mInput.GetData().GetnFrames();
}

bool SMSTimeStretch::IsLastFrame()
{
	bool isLast=HaveFinished();
	if(isLast)
	{
		while(mOutput.GetData().GetnFrames()>mnSynthesisFrames)
		{
					mOutput.GetData().DeleteFrame(mOutput.GetData().GetnFrames()-1);
		}
	}
	return isLast;
}

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
static ProcessingFactory::Registrator<CLAM::SMSTimeStretch> regtSMSTimeStretch( "SMSTimeStretch" );
