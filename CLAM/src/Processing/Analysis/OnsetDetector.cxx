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

#include "OnsetDetector.hxx"
#include "Point.hxx"
#include "OD_AudioDecimation.hxx"
#include "CLAM_Math.hxx"

namespace CLAM
{
	OnsetDetector::OnsetDetector()
	{
		AttachChildren();
		Configure(OnsetDetectorConfig());
	}

	OnsetDetector::OnsetDetector(OnsetDetectorConfig& c)
	{
		AttachChildren();
		Configure(c);
	}

	bool OnsetDetector::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig( mConfig, c );

		mnBands = mConfig.GetnBands();
		mLowFreq = mConfig.GetLowFreq();
		mHighFreq = mConfig.GetHighFreq();
		mBandThreshold = mConfig.GetBandThreshold();
		mMinPeakDist = mConfig.GetMinPeakDist();
		mGlobalThreshold = mConfig.GetGlobalThreshold();
		mNoiseThreshold = mConfig.GetNoiseThreshold();
		mComputeOffsets = mConfig.GetComputeOffsets();
		mOffsetThreshold = mConfig.GetOffsetThreshold();
		
		//Computes the smoothing filter coefficients
		mWinSize = mConfig.GetSmoothFiltSize();
		mWinCoef.Resize(mWinSize);
		mWinCoef.SetSize(mWinSize);

		TData dPhi = ( 2.0 * M_PI ) / ( TData(mWinSize)*2.0 );
		TData phi = 0.0;
		
		for(int i=0; i<mWinSize; i++, phi += dPhi )
			mWinCoef[i]=0.5+0.5*std::cos(phi);

		
		ConfigureChildren();
	
		return true;
	}


	bool OnsetDetector::ConfigureChildren()
	{
		//Filter Bank Configuration
		GT_FilterBankConfig bankcfg;
		bankcfg.SetnChannels(mnBands);
		bankcfg.SetLowFreq(mLowFreq);
		bankcfg.SetHighFreq(mHighFreq);
		bankcfg.SetSampleRate(22050);
		mFilterBank.Configure(bankcfg);

		//Normalisation Configuration
		NormalizationConfig NCfg;
		NCfg.SetType(3);
		mNorm.Configure(NCfg);	

		return true;
	}


	void OnsetDetector::AttachChildren()
	{
		mFilterBank.SetParent(this);
		mNorm.SetParent( this );
	}

	bool OnsetDetector::Do(Segment &originalSegment, Array<TimeIndex>& out)
	{
		CLAM_ASSERT( int(originalSegment.GetAudio().GetSampleRate()) == 44100,
			     "This onset detection algorithm only works for signals sampled at a 44.1kHz rate" );

		//Normalization and downsampling to 22.05 kHz
		mNorm.Do(originalSegment.GetAudio());



		mAudio.SetSize(originalSegment.GetAudio().GetSize()/2);
		mAudio.SetSampleRate(originalSegment.GetAudio().GetSampleRate()/2);

		mDecimator.DecimateFrom44100To22050( originalSegment.GetAudio().GetBuffer(),
						     mAudio.GetBuffer() );
		

		//Filter bank output computation
		mFilterBankOutputs.Resize(mnBands);
		mFilterBankOutputs.SetSize(mnBands);
		
		CalcFilterBankOutput( );
		mnSamples = mFilterBankOutputs[0].Size();

		//Onset detection	
		Algorithm( originalSegment, out );
	
		return true;

	}


	///////////////
	//FILTER BANK//
	///////////////
	bool OnsetDetector::CalcFilterBankOutput()
	{
		//Filter Bank


		const TSize bandSize=mAudio.GetSize();

		for ( int band = 0; band < mnBands; band++ )
		{
			mFilterBankOutputs[band].Resize(bandSize/90);
			mFilterBankOutputs[band].SetSize(bandSize/90);			
		}

		mFilterBank.Do( mAudio, mFilterBankOutputs );

		mSampleRate = 245;
	
		return true;
	}


////////////////////////////////////////
////////SEGMENTATION ALGORITHM//////////
////////////////////////////////////////
	void OnsetDetector::Algorithm( Segment& s , Array<TimeIndex>& finalOnsets )
	{
	
		//cout << "\nOnset Detection per band...\n";
	
		Array< Array<double> > smoothedInput(mnBands);	//Smoothed Band Energy Array
		smoothedInput.SetSize(mnBands);

		Array< Array<double> > bandOnsetDetectData(mnBands);	//Data on which detection is performed 
		bandOnsetDetectData.SetSize(mnBands);

		Array< Array<TimeIndex> > bandCandidates(mnBands);		//Candidates positions per band
		bandCandidates.SetSize(mnBands);

	
		TimeIndex tempOnset;
		Array< Array<TimeIndex> > bandOnsets(mnBands);
		bandOnsets.SetSize(mnBands);
	

		DataArray onsetsArray(mnSamples);		//sum of cleaned onsets detected in each bands (Data)
		onsetsArray.SetSize(mnSamples);	


		mRevSmoothedEnergy.Resize( mnSamples );
		mRevSmoothedEnergy.SetSize( mnSamples );

		//////////////////////////
		//ONSET DETECTION PER BAND	
		for (int band=0 ; band<mnBands ; band++)
		{
		
			//smoothing
			Smoothing( mFilterBankOutputs[band] , smoothedInput[band] );

						
			//Detection Function Calculation
			DetectPosition( smoothedInput[band] , bandOnsetDetectData[band]);		

		
			//Candidates Detection
			DetectCandidates( bandOnsetDetectData[band] , smoothedInput[band], mBandThreshold , bandCandidates[band]);


			//weak onsets deletion
			DeleteWeakOnsets( bandCandidates[band] , 1 );


			//sums up band results
			for (int j=0 ; j<bandCandidates[band].Size() ; j++)
				onsetsArray[bandCandidates[band][j].GetPosition()] += bandCandidates[band][j].GetWeight();		
		}
	

		//Forms ordered array of onsets
		for (int i=0 ; i<mnSamples ; i++)
		{
			if (onsetsArray[i] < 1e-7 ) continue;
			tempOnset.SetPosition(i);
			tempOnset.SetWeight(onsetsArray[i]);
			mOnsets.AddElem(tempOnset);
		}
	

		//weak onsets deletion (weak onsets intensities are summed up
		//and added to the highest peak in a mMinPeakDist long window)
		DeleteWeakOnsets( mOnsets , 2 );

		//final thresholding (deletes onsets with intensities lower than mGlobalThreshold)

		float maxWeight = 0.0;
		for (int j=0 ; j<mOnsets.Size() ; j++)
		{
			if (mOnsets[j].GetWeight()<=mGlobalThreshold) continue;
			tempOnset.SetPosition(mOnsets[j].GetPosition());
			tempOnset.SetWeight(mOnsets[j].GetWeight());
			finalOnsets.AddElem(tempOnset);
			if(mOnsets[j].GetWeight() >= maxWeight)
				maxWeight = mOnsets[j].GetWeight();
		}

	
		/////////////////////////////////
		//Stores Boundaries Information//
		/////////////////////////////////
		if(finalOnsets.Size()>0)
		{
			if(mComputeOffsets)
			{
				// Checks for Offsets and stores Onset/Offset information in the Segment
				CheckOffset(s , finalOnsets);
			}
			else
			{
				// Stores segment boundaries (onsets) information	  
				for(int j=0; j<(finalOnsets.Size()-1); j++) 
				{
					Segment tmpSegment;
					tmpSegment.SetBeginTime(finalOnsets[j].GetPosition() / mSampleRate);
					tmpSegment.SetEndTime  (finalOnsets[j+1].GetPosition() / mSampleRate);
					tmpSegment.SetpParent(&s);
					s.GetChildren().AddElem(tmpSegment);
				}
				Segment tmpSegment;
				tmpSegment.SetBeginTime(finalOnsets[finalOnsets.Size()-1].GetPosition() /mSampleRate);
				tmpSegment.SetEndTime(s.GetEndTime());
				tmpSegment.SetpParent(&s);
				s.GetChildren().AddElem(tmpSegment);
			}
		}


		for(int j=0; j<(finalOnsets.Size()); j++) 
		{
			finalOnsets[j].SetPosition(finalOnsets[j].GetPosition() / mSampleRate);
			finalOnsets[j].SetWeight(finalOnsets[j].GetWeight() / maxWeight);
		}


	}


	void OnsetDetector::Smoothing(Array<double>& energy, Array<double>& smoothedEnergy )
	{
		int i, j, k;
		TData temp;


		//Rescaling factor
		TData sum=0;
		for(k=0; k<mWinCoef.Size(); k++)
			sum += mWinCoef [k];

		//convolution
		for(i=0; i<mnSamples;i++)
		{	
			temp=0;
			for(j=0; j<mWinSize; j++)
			{
				if(i-mWinSize+1+j>=0) 
				{
					temp+=energy[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];
				}
			}

			if ( (temp/sum) >mNoiseThreshold)
			{smoothedEnergy.AddElem(temp/sum);}
			else 
			{smoothedEnergy.AddElem(mNoiseThreshold);}

			mRevSmoothedEnergy[mnSamples-i-1]=smoothedEnergy[i];
		}

		//reverse convolution for zero-phase distortion
		for(i=0; i<mnSamples;i++)
		{	
			temp=0;
			for(j=0; j<mWinSize; j++)
			{
				if(i-mWinSize+1+j>=0) 
				{
					temp+=mRevSmoothedEnergy[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];
				}
			}
			smoothedEnergy[mnSamples-i-1]=temp/sum;		
		}

	}


////////////////////////////////
//////ONSET TIME DETECTION//////
////////////////////////////////
	void OnsetDetector::DetectPosition(Array<double>& in, Array<double>& ret)
	{
		int i;

		for (i=0 ; i<mnSamples ; i++) 
		{
			if (i==0 || i==mnSamples-1)	//Sets first and last values to zero
			{ret.AddElem(0);}
			else
			{ret.AddElem(log(in[i+1])-log(in[i-1]));}
		
		}
	}


////////////////////////////////
///////CANDIDATE DETECTION//////
////////////////////////////////
	void OnsetDetector::DetectCandidates(Array<double>& in, Array<double>& weight, TData threshold , Array<TimeIndex>& ret)
	{	
	
		//This function detect the positions and weights of candidates
		//The highest peak in the derivative of the log smoothed energy is taken as the position
		//(Instead, the position of the first peak or the highest peak in the derivative of the smoothed energy can be taken)
		//The weigth is the derivative of the smoothed energy at this position
		//(instead, the derivative of the log smoothed energy or the "dynamic" can be taken)	
	
	
		int i=1, length, begin, maxLogPos, maxLinPos;
		TData maxLog, maxLin, intDyn;	
		TimeIndex candidate;



		//takes peaks above the threshold
		while(i<in.Size()-3)
		{
			length=maxLog=maxLin=0;		

			if(in[i]>0)
			{


				begin=i;

				//while the derivative is positive (up to 2 negative values are allowed)
				while(i<in.Size()-3 && (in[i]>0 || in[i+1]>0 || in[i+2]>0))
				{
				

					//store the position and the value of the highest peak in the detection function (derivative of
					//the log smoothed energy)
					if(in[i]>maxLog)
					{
						maxLog=in[i];
						maxLogPos=i;
					}

					//store the position and the value of the highest peak in the derivative of
					//the smoothed energy
					if((weight[i+1]-weight[i-1])>maxLin)
					{
						maxLin=weight[i+1]-weight[i-1];
						maxLinPos=i;
					}
				
					length++;
					i++;
				}
			
				//onset "dynamic" could also be used
				//intDyn=(weight[i-1]-weight[begin]);

				if(maxLog>threshold)
				{
					candidate.SetPosition(maxLogPos);
					candidate.SetWeight(maxLin);
					ret.AddElem(candidate);

				}
			}
			else i++;
		}
	}



//////////////////////////////////////////
/////SMALL INTENSITY ONSETS DELETION//////
//PROCESSING OF TOO-CLOSELY SPACED PEAKS//
//////////////////////////////////////////
	void OnsetDetector::DeleteWeakOnsets(Array<TimeIndex> &in , int type)
	{
		TimeIndex newPeak;
		int j=0 , k=0;
		while ( j < in.Size()-1 ) {
			k=j;
		
			//finds too-closely spaced peaks
			int numMemPeaks=0;
			
			//type 1 performs peak deletion
			if ( type == 1 ) {
				while ( k<in.Size()-1  &&  (in[k+1].GetPosition() - in[k].GetPosition()) < mMinPeakDist ) {
					numMemPeaks++;
					k++;
				}
			}
			
			//type 2 performs peak summation
			if ( type == 2 ) {
				while ( k<in.Size()-1  &&  (in[k+1].GetPosition() - in[j].GetPosition()) < mMinPeakDist ) {
					numMemPeaks++;
					k++;
				}
			}
			
			if ( numMemPeaks!=0 ) {
				//stores too-closely spaced peaks
				Array<TimeIndex> memMat(numMemPeaks+1);
				memMat.SetSize(numMemPeaks+1);			
	
				int m=0, l;
				for (l=j ; l<=k ; l++) {
					memMat[m].SetPosition(in[l].GetPosition());
					memMat[m].SetWeight(in[l].GetWeight() );
					m++;
				}
			
				if ( type == 2 )
					//Sums the peaks
					PeakSummation( memMat , newPeak );
				
				if ( type == 1 ) 
					//Deletes the peaks
					PeakDeletion( memMat , newPeak );

				//Update the peak matrix
				for (l=k ; l>j-1 ; l--) {
					if (l==k) { 
						in[l].SetPosition(newPeak.GetPosition());
						in[l].SetWeight(newPeak.GetWeight());
					}
					else in.DeleteElem(l);
					
				}
				
			}
			else j++;	
		}
	}


//PEAK DELETION
	void OnsetDetector::PeakDeletion( Array<TimeIndex>& in , TimeIndex& ret )
	{
		TData max=-1;
		int pos , j;

		for (j=0 ; j<in.Size() ; j++) {
			if (in[j].GetWeight()>max) {
				max=in[j].GetWeight();
				pos=in[j].GetPosition();
			}
		}
		ret.SetPosition(pos);
		ret.SetWeight(max);
	}


//PEAK SUMMATION
	void OnsetDetector::PeakSummation( Array<TimeIndex>& in , TimeIndex &ret )
	{
		TData max=-1 , sum=0;
		int pos , j;

		for (j=0 ; j<in.Size() ; j++) {
			sum+=in[j].GetWeight();
			if (in[j].GetWeight()>max) {
				max=in[j].GetWeight();
				pos=in[j].GetPosition();
			}
		}

		ret.SetPosition(pos);
		ret.SetWeight(sum);
	}



/////////////////////
//CHECK FOR OFFSETS//
/////////////////////
	void OnsetDetector::CheckOffset( Segment &s , Array<TimeIndex>& finalOnsets)
	{

		int i , j;
		Array< PointTmpl<int,int> > offsets;
	
		////////////////////////////////
		//Extracts Amplitude Enveloppe//
		////////////////////////////////
		Array<double> amplitude;
		
		amplitude.Resize( mAudio.GetSize()/90 );
		amplitude.SetSize( mAudio.GetSize()/90 );
		DataArray& samples = mAudio.GetBuffer();
		TSize numSamples = mAudio.GetSize();

		for( i=0; i< numSamples ; i++)
			samples[i] = fabsf(samples[i]);

		//Decimation

		mDecimator.DecimateFrom22050To245(samples, amplitude);

		//Computes the smoothing filter coefficients
		TSize winSize = 0.05*mSampleRate;
		Array<TData> winCoef(winSize);
		winCoef.SetSize(winSize);
		for(i=0; i<winSize; i++)	winCoef[i]=1;

		//Rescaling factor
		TData sum=winCoef.Size();

		//convolution
		DataArray envelope(amplitude.Size());
		envelope.SetSize(amplitude.Size());
		DataArray revenvelope(amplitude.Size());
		revenvelope.SetSize(amplitude.Size());
		for(i=0; i<amplitude.Size();i++)
		{	
			TData temp=0;
			for(j=0; j<winSize; j++)
			{
				if(i-winSize+1+j>=0) 
				{
					temp+=amplitude[i-winSize+1+j]*winCoef[winSize-1-j];
				}
			}
			envelope[i]=temp/sum;
			revenvelope[amplitude.Size()-i-1]=envelope[i];
		}

		//reverse convolution for zero-phase distortion
		for(i=0; i<amplitude.Size();i++)
		{	
			TData temp=0;
			for(j=0; j<winSize; j++)
			{
				if(i-winSize+1+j>=0) 
				{
					temp+=revenvelope[i-winSize+1+j]*winCoef[winSize-1-j];
				}
			}
			envelope[amplitude.Size()-i-1]=temp/sum;
		}


		//////////////////////
		//Checks for Offsets//  checks backwards, from each offset, when the smoothed amplitude rises above the threshold after the minimum;
		//////////////////////
		PointTmpl<int,int> temp;
		TData min = 0;
		for (j=1 ; j<finalOnsets.Size() ; j++)
		{
			//onset position
			i=finalOnsets[j].GetPosition();
			temp.SetX(finalOnsets[j-1].GetPosition());
			//looks for the first min before the onset
			min = envelope[i];
			do
			{
				if(envelope[i]<min)
					min = envelope[i];
				i--;
			} while (i>0  && envelope[i]>envelope[i-1]);
			//finds the point (going backwards) where the amplitude rises above the threshold 
			if (min < mOffsetThreshold && i>0) 
			{
				while (envelope[i] < mOffsetThreshold && i>finalOnsets[j-1].GetPosition())
					i--;
				if(i>finalOnsets[j-1].GetPosition())
				{
					temp.SetY(i);
					offsets.AddElem(temp);
				}
			}
		}
		//Checking for last offset
		i=finalOnsets[finalOnsets.Size()-1].GetPosition();
		temp.SetX(finalOnsets[finalOnsets.Size()-1].GetPosition());
		bool found=false;
		do  
		{
			if (envelope[i]<mOffsetThreshold && envelope[i]>envelope[i+1] && envelope[i+1]>envelope[i+2] && envelope[i+2]>envelope[i+3])
			{
				temp.SetY(i);
				offsets.AddElem(temp);
				found=true;
			}
			i++;
		}while(i<envelope.Size()-3 && !found);


		/////////////////////////////
		//Storing Onset/Offset Info//	
		/////////////////////////////
		//A segment beggining is always an onset
		//A segment end is either an offset (if the amplitude goes under mOffsetThreshold) , or the next onset
		if(finalOnsets.Size()>0)
		{
			i=0;
			j=0;
			bool hasOffset = false;
			while( j<finalOnsets.Size()-1) 
			{
				Segment tmpSegment;
				int onset=finalOnsets[j].GetPosition();
			
				if (i<offsets.Size())
				{
					int offOnset=offsets[i].GetX();
					if(onset==offOnset)
						hasOffset=true;
				}
	
				if (hasOffset)
				{
					tmpSegment.SetBeginTime((TData)offsets[i].GetX()/(TData)mSampleRate);
					tmpSegment.SetEndTime((TData)offsets[i].GetY()/(TData)mSampleRate);
					tmpSegment.SetpParent(&s);
					s.GetChildren().AddElem(tmpSegment);
					i++;
					j++;
					hasOffset=false;
				}
				else
				{
					tmpSegment.SetBeginTime((TData)finalOnsets[j].GetPosition() / (TData)mSampleRate);
					tmpSegment.SetEndTime((TData)finalOnsets[j+1].GetPosition() / (TData)mSampleRate);
					tmpSegment.SetpParent(&s);
					s.GetChildren().AddElem(tmpSegment);
					j++;
				}
			}
			//Last boundaries
			Segment tmpSegment;
			int onset=finalOnsets[j].GetPosition();
			
			if (i<offsets.Size())
			{
				int offOnset=offsets[i].GetX();
				if(onset==offOnset)
					hasOffset=true;
			}

			if (hasOffset)
			{
				tmpSegment.SetBeginTime((TData)offsets[i].GetX()/(TData)mSampleRate);
				tmpSegment.SetEndTime((TData)offsets[i].GetY()/(TData)mSampleRate);
				tmpSegment.SetpParent(&s);
				s.GetChildren().AddElem(tmpSegment);
			}
			else
			{
				tmpSegment.SetBeginTime((TData)finalOnsets[finalOnsets.Size()-1].GetPosition() / (TData)mSampleRate);
				tmpSegment.SetEndTime(s.GetEndTime());
				tmpSegment.SetpParent(&s);
				s.GetChildren().AddElem(tmpSegment);
			}
		}
	}

}
