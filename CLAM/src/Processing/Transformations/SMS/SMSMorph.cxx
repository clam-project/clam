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
		tmpBPF.SetValue(0,0.49);
		tmpBPF.SetXValue(0,0);
		tmpBPF.SetValue(1,0.49);
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
	if(mConfig.HasHybSinSpectralShape())
		mHarmSpectralShapeMorph=true;
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


bool SMSMorph::Do(SpectralPeakArray& in1,const SpectralPeakArray& in2, SpectralPeakArray& out)
{
	//TODO: kludge. In1 is the one loaded and always in linear
	in1.TodB();
	
	int nPeaks1=in1.GetnPeaks();
	int nPeaks2=in2.GetnPeaks();

	bool noPeaks1=false;
	bool noPeaks2=false;

	if(nPeaks1==0) noPeaks1=true;
	if(nPeaks2==0) noPeaks2=true;

	out.SetnPeaks(nPeaks1);

	int i , j;
	DataArray& in1Mag=in1.GetMagBuffer();
	DataArray& in2Mag=in2.GetMagBuffer();
	DataArray& outMag=out.GetMagBuffer();

	DataArray& in1Freq=in1.GetFreqBuffer();
	DataArray& in2Freq=in2.GetFreqBuffer();
	DataArray& outFreq=out.GetFreqBuffer();

	IndexArray& in1Index=in1.GetIndexArray();
	IndexArray& in2Index=in2.GetIndexArray();

	DataArray& in1Phase=in1.GetPhaseBuffer();
	DataArray& outPhase=out.GetPhaseBuffer();
		
	/*BPF in2BPF;

	//Initializes BPF for spectral shape morph
	if(mHarmSpectralShapeMorph)
	{
		for(i=0;i<nPeaks2;i++)
			in2BPF.Insert(in2Freq[i] , in2Mag[i]);
	}*/

	
	////////////////////
	/*Different Morphs*/
	////////////////////
	if(noPeaks1)
	{
		return true;
	}
	else if(noPeaks2)
	{
		for (i=0;i<nPeaks1;i++)
		{
			outMag[i]=in1Mag[i];
			outFreq[i]=in1Freq[i];
			outPhase[i]=in1Phase[i];
		}
	}
	else if(mHarmonicMorph)
	{
		TIndex pos=0;
		bool harmonicFound;
		i=0;
		do//for(i=0;i<nPeaks1;i++)//
		{
			harmonicFound=false;
			for(j=pos;j<nPeaks2;j++)
			{
				if(in2Index[j]==in1Index[i])
				{
					pos=j;
					harmonicFound=true;
					break;
				}
			}
			if(harmonicFound)
			{
				outMag[i]=in1Mag[i]*(1-magInterpFactor)+in2Mag[j]*magInterpFactor;
				outFreq[i]=((in1Freq[i]/mPitch1)*(1-freqInterpFactor)+(in2Freq[j]/mPitch2)*freqInterpFactor)*mNewPitch;
				outPhase[i]=in1Phase[i];
				i++;
			}
			/*else
			{
				outMag[i]=in1Mag[i]*(1-magInterpFactor);
				outFreq[i]=(in1Freq[i]/mPitch1)*mNwPitch;
				outPhase[i]=in1Phase[i];
			}*/
			/*else if(mHarmSpectralShapeMorph)
			{
				outMag[i]=in1Mag[i]*(1-magInterpFactor)+in2BPF.GetValue(in1Freq[i])*magInterpFactor;
				i++;
			}*/
			else
			{
				out.DeleteSpectralPeak(i);
				nPeaks1=nPeaks1-1;
				out.SetnPeaks(nPeaks1);
			}
			/*else
			{
				outMag[i]=in1Mag[i];
				outFreq[i]=in1Freq[i];
				outPhase[i]=in1Phase[i];
			}*/
		}while(i<nPeaks1);
	}
	//Morphing Using Harmonic No*/
	//else Non harmonic morphing
	//Morphing peaks by position in the peak array//
	//always keeping No peaks equal to No of peaks in original segment
	else
	{
		TData factor2=nPeaks2/nPeaks1;
		for (i=0;i<nPeaks1;i++)
		{
			outMag[i]=in1Mag[i]*(1-magInterpFactor)+in2Mag[i*factor2]*magInterpFactor;
			outFreq[i]=in1Freq[i]*(1-freqInterpFactor)+in2Freq[i*factor2]*freqInterpFactor;
			outPhase[i]=in1Phase[i];
		}
	}
	//Morphing peaks by position in the peak array//*/
	
	return true;
}

bool SMSMorph::Do(const Frame& in1,const Frame& in2, Frame& out)
{
	int i;

	
	///////////////////////
	/*Sinusoidal Morphing*/
	///////////////////////
	TData morphFactor=mHybBPF.GetLastValue();
	TData magFactor=mHybSinAmp.GetLastValue();
	TData freqFactor=mHybSinFreq.GetLastValue();
	TData pitchFactor=mHybPitch.GetLastValue();

	if(morphFactor<=0.5)
	{	
		if(mHarmonicMorph)
		{
			mPitch1=in1.GetFundamentalFreq();
			mPitch2=in2.GetFundamentalFreq();
			TData pitchInterpFactor=pitchFactor;
			mNewPitch=mPitch1*(1-pitchInterpFactor)+mPitch2*pitchInterpFactor;
		}

		magInterpFactor=magFactor;
		freqInterpFactor=freqFactor;

		Do(in1.GetSpectralPeakArray(),in2.GetSpectralPeakArray(),out.GetSpectralPeakArray());

	}
	else
	{	
		if(mHarmonicMorph)
		{
			mPitch1=in2.GetFundamentalFreq();
			mPitch2=in1.GetFundamentalFreq();
			TData pitchInterpFactor=1-pitchFactor;
			mNewPitch=mPitch1*(1-pitchInterpFactor)+mPitch2*pitchInterpFactor;
		}

		magInterpFactor=1-magFactor;
		freqInterpFactor=1-freqFactor;

		SpectralPeakArray& temp=in2.GetSpectralPeakArray();

		Do(in2.GetSpectralPeakArray(),in1.GetSpectralPeakArray(),temp);//out.GetSpectralPeakArray());

		out.SetSpectralPeakArray(in2.GetSpectralPeakArray());
	}

	//Sets new fund freq
	if(mHarmonicMorph)
		out.SetFundamentalFreq(0,mNewPitch);
	else
		out.SetFundamentalFreq(0,0);


	///////////////////////
	/**Residual Morphing**/
	///////////////////////
	TData resAmpFactor=mHybResAmp.GetLastValue();
	//TData resPhase=mHybResPhase.GetLastValue();

	/** Xavier: we cannot be sure that incoming spectrum is in MagPhase format.
	We will do it the "slow way" and try to optimize later (TODO)
	
	DataArray& in1ResSpecAmp=in1.GetResidualSpec().GetMagBuffer();
	DataArray& in2ResSpecAmp=in2.GetResidualSpec().GetMagBuffer();
	DataArray& outResSpecAmp=out.GetResidualSpec().GetMagBuffer();
	
	*/
	Spectrum &inRes1=in1.GetResidualSpec();
	Spectrum &inRes2=in2.GetResidualSpec();
	Spectrum &outRes=out.GetResidualSpec();
	
	TSize specSize=out.GetResidualSpec().GetSize();

	for(i=0;i<specSize;i++)
		outRes.SetMag(i,inRes1.GetMag(i)*(1-resAmpFactor)+inRes2.GetMag(i)*resAmpFactor);
		//outResSpecAmp[i]=in1ResSpecAmp[i]*(1-resAmpFactor)+in2ResSpecAmp[i]*resAmpFactor;
	if(morphFactor>0.5)
		for(i=0;i<specSize;i++)
			outRes.SetPhase(i,inRes2.GetPhase(i));
			//out.GetResidualSpec().SetPhaseBuffer(in2.GetResidualSpec().GetPhaseBuffer());

	return true;
}

bool SMSMorph::Do(const Frame& in1,const Frame& in2, Frame& out , TData morphFactor)
{
	int i;

	
	///////////////////////
	/*Sinusoidal Morphing*/
	///////////////////////
	if(morphFactor<=0.5)
	{	
		if(mHarmonicMorph)
		{
			mPitch1=in1.GetFundamentalFreq();
			mPitch2=in2.GetFundamentalFreq();
			mNewPitch=mPitch1*(1-morphFactor)+mPitch2*morphFactor;
		}
		
		magInterpFactor=morphFactor;
		freqInterpFactor=morphFactor;

		SpectralPeakArray& temp=in2.GetSpectralPeakArray();
	
		Do(in1.GetSpectralPeakArray(),in2.GetSpectralPeakArray(),temp);
	
		out.SetSpectralPeakArray(in1.GetSpectralPeakArray());
	}
	else
	{
		if(mHarmonicMorph)
		{
			mPitch1=in2.GetFundamentalFreq();
			mPitch2=in1.GetFundamentalFreq();
			TData pitchInterpFactor=1-morphFactor;
			mNewPitch=mPitch1*(1-pitchInterpFactor)+mPitch2*pitchInterpFactor;
		}
			
		magInterpFactor=1-morphFactor;
		freqInterpFactor=1-morphFactor;
		
		SpectralPeakArray& temp=in2.GetSpectralPeakArray();

		Do(in2.GetSpectralPeakArray(),in1.GetSpectralPeakArray(),temp);

		out.SetSpectralPeakArray(in2.GetSpectralPeakArray());
	}
	
	//Sets new fund freq
	if(mHarmonicMorph)
		out.SetFundamentalFreq(0,mNewPitch);
	else
		out.SetFundamentalFreq(0,0);

	
	
	///////////////////////
	/**Residual Morphing**/
	///////////////////////
	TData resAmpFactor=mHybResAmp.GetLastValue();
	//TData resPhase=mHybResPhase.GetLastValue();

	DataArray& in1ResSpecAmp=in1.GetResidualSpec().GetMagBuffer();
	DataArray& in2ResSpecAmp=in2.GetResidualSpec().GetMagBuffer();
	DataArray& outResSpecAmp=out.GetResidualSpec().GetMagBuffer();
	
	TSize specSize=out.GetResidualSpec().GetSize();

	for(i=0;i<specSize;i++)
		outResSpecAmp[i]=in1ResSpecAmp[i]*(1-resAmpFactor)+in2ResSpecAmp[i]*resAmpFactor;

	//for the moment always take original sound residual phase
	//if(resPhase)
	//	out.GetResidualSpec().SetPhaseBuffer(in2.GetResidualSpec().GetPahseBuffer();
		
	return true;
}

bool SMSMorph::Do()
{
	return Do(mInput1.GetData(),mInput2.GetData(),mOutput.GetData());

}

bool SMSMorph::Do(const Segment& in1, Segment& out)
{
	CLAM_ASSERT(mHaveInternalSegment, "SMSMorph::Do: you cannot call this overload if internal segment has not been previously lodade");
	return Do(in1,mInput2.GetData(),out);
}

bool SMSMorph::Do(const Segment& in1,const Segment& in2, Segment& out)
{
	//int i , j;
	int i=in1.mCurrentFrameIndex;
	
	TSize nFrames1=in1.GetnFrames();
	TSize nFrames2=in2.GetnFrames();
	TSize nFrames;
	nFrames=nFrames1;
	//if(nFrames1>nFrames2) nFrames=nFrames1;
	//else nFrames=nFrames2;
	//TData factor1=(TData)nFrames1/nFrames;
	//TData factor2=(TData)nFrames2/nFrames;
	
	TData synchroTimeFactor; 

	std::ofstream storePeaks("peaks.dat" , std::ios::out);

	TSize maxPeaks=0;
//	for(i=0;i<nFrames;i++)
//	{
		UpdateControlValueFromBPF((TData)i/nFrames);
		synchroTimeFactor=mSynchronizeTime.GetLastValue()*nFrames2;
		mHarmonicMorph=false;
		
		//With Frame Interpolation
		if(mConfig.GetInterpolateFrame())
		{
			//Initializes interpolated frame 
			Frame tempFrame2;
			tempFrame2.AddSpectralPeakArray();
			tempFrame2.AddFundamental();
			tempFrame2.AddResidualSpec();
			tempFrame2.UpdateData();
			tempFrame2.GetSpectralPeakArray().AddIndexArray();
			tempFrame2.GetSpectralPeakArray().UpdateData();
			
			//Interpolation data
			int frameNo1=floor(synchroTimeFactor);
			int frameNo2=ceil(synchroTimeFactor);
			TData frameInterpFactor=synchroTimeFactor-frameNo1;
			tempFrame2.GetResidualSpec().SetSize(in2.GetFrame(frameNo1).GetResidualSpec().GetSize());
			tempFrame2.GetFundamental().AddElem(0,0);
		
			if(in2.GetFrame(frameNo1).GetFundamentalFreq()!=0 && in2.GetFrame(frameNo2).GetFundamentalFreq()!=0 ) 
				mHarmonicMorph=true;

			//Interpolating
			Do(in2.GetFrame(frameNo1) , in2.GetFrame(frameNo2) , tempFrame2 , frameInterpFactor);			
			mHarmonicMorph=false;

			//Morphing
			if(in1.GetFrame(i).GetFundamentalFreq()!=0 && tempFrame2.GetFundamentalFreq()!=0 )
				mHarmonicMorph=true;
				
			Do(in1.GetFrame(i),tempFrame2,out.GetFrame(i));
			
		}
		//Without Frame Interpolation
		else
		{
			if(in1.GetFrame(i).GetFundamentalFreq()!=0 && in2.GetFrame(int(synchroTimeFactor)).GetFundamentalFreq()!=0 )
				mHarmonicMorph=true;
			Do(in1.GetFrame(i),in2.GetFrame((int)(synchroTimeFactor)),out.GetFrame(i));
		}
//	}
	
					
//	for(i=0;i<nFrames;i++)
//	{
		if(out.GetFrame(i).GetSpectralPeakArray().GetnPeaks()>maxPeaks)
			maxPeaks=out.GetFrame(i).GetSpectralPeakArray().GetnPeaks();
//	}

	//stores peaks
/*	Array<DataArray> peakmatrix(nFrames);
	peakmatrix.SetSize(nFrames);
	for(i=0;i<nFrames;i++)
	{
		peakmatrix[i].Resize(maxPeaks);
		peakmatrix[i].SetSize(maxPeaks);
		for(j=0;j<out.GetFrame(i).GetSpectralPeakArray().GetnPeaks();j++)
			peakmatrix[i][j]=out.GetFrame(i).GetSpectralPeakArray().GetFreq(j);
		
		storePeaks << i << "	" << out.GetFrame(i).GetFundamentalFreq() << "		";
		for(j=0;j<maxPeaks;j++)
			storePeaks << peakmatrix[i][j] << "		";
		storePeaks << "\n";
	}

	std::cout << "\n\n\n";
	std::cout << "\nFinished dude\n\n";
*/	return true;
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
