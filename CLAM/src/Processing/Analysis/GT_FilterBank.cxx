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

//Implemented from Slaney's Auditory Toolbox (http://rvl4.ecn.purdue.edu/~malcolm/interval/1998-010/)

#include "Complex.hxx"
#include "ERB_Space.hxx"
#include "GT_FilterBank.hxx"
#include "Audio.hxx"
#include "OSDefines.hxx"

namespace CLAM
{

	GT_FilterBank::GT_FilterBank()
	{
		Configure(GT_FilterBankConfig());
	}

	GT_FilterBank::GT_FilterBank(GT_FilterBankConfig& c)
	{
		Configure(c);
	}

	GT_FilterBank::~GT_FilterBank() 
	{
	}


	bool GT_FilterBank::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig( mConfig, c );

		mnChannels = mConfig.GetnChannels();
		mLowFreq = mConfig.GetLowFreq();
		mHighFreq = mConfig.GetHighFreq();
		mSampleRate = mConfig.GetSampleRate();

		MakeERBFilters();
		
		return true;
	}

	bool GT_FilterBank::Do(void) 
	{
		return false;
	}
	
	bool GT_FilterBank::Do(Audio &in, Array< DataArray >& filterBankOutputs )
	{
		CLAM_ASSERT( filterBankOutputs.Size() == mnChannels,
			     "GT_FilterBank::Do() : filterBankOutputs array size is smaller than the number of bands setup on the configuration" );


		const TSize audiosize = in.GetSize();
	
		DataArray audioArray;

		audioArray.Resize( audiosize );
		audioArray.SetSize( audiosize );


		const DataArray & inputBuffer = in.GetBuffer();

	

		double tempBuffer0[4];
		double tempBuffer1[4];
		double tempBuffer2[4];
		double tempBuffer3[4];
		double tempBuffer4[4];

		//filtering

		//Slaney's comment:
		//Note this implementation fixes a problem in the original code by
		//computing four separate second order filters.  This avoids a big
		//problem with round off errors in cases of very small mCentreFreqs (100Hz) and
		//large sample rates (44kHz).  The problem is caused by roundoff error
		//when a number of poles are combined, all very close to the unit
		//circle.  Small errors in the eigth order coefficient, are multiplied
		//when the eigth root is taken to give the pole location.  These small
		//errors lead to poles outside the unit circle and instability.

		//Is that also true here?
	
		DataArray & outputBuffer = audioArray;

		for ( int i = 0; i < mnChannels; i++ )
		{
		
			tempBuffer0[0] = inputBuffer[0];
			tempBuffer0[1] = inputBuffer[1];
			tempBuffer1[0] = 0;
			tempBuffer1[1] = 0;
			tempBuffer2[0] = 0;
			tempBuffer2[1] = 0;
			tempBuffer3[0] = 0;
			tempBuffer3[1] = 0;
			tempBuffer4[0] = 0;
			tempBuffer4[1] = 0;

			outputBuffer[0] = 0;
			outputBuffer[1] = 0;

			const double A0byGain=A0/gain[i];
			const double A11byGain=A11[i]/gain[i];
			const double A2byGain=A2/gain[i];
			const double B1i=B1[i];
			const double B2i=B2[i];
			const double A12i=A12[i];
			const double A13i=A13[i];
			const double A14i=A14[i];

			unsigned j0=1,j1=0,j2;

			for(int j=2; j<audiosize; j++)
			{
				// j0, j1 and j2 implement a circular buffer of 4 positions
				// Althougt we need only 3, 4 is cheaper on calculations.
				j2 = j1;  // j-2
				j1 = j0;  // j-1
				j0 = j&3; // j

				tempBuffer0[j0] = inputBuffer[j];
				tempBuffer1[j0] =
					+ A0byGain *tempBuffer0[j0]
					+ A11byGain*tempBuffer0[j1] 
					+ A2byGain *tempBuffer0[j2]
					- B1i *tempBuffer1[j1] 
					- B2i *tempBuffer1[j2];
				tempBuffer2[j0] =
					+ A0  *tempBuffer1[j0] 
					+ A12i*tempBuffer1[j1] 
					+ A2  *tempBuffer1[j2]
					- B1i *tempBuffer2[j1] 
					- B2i *tempBuffer2[j2];
				tempBuffer3[j0] =
					+ A0  *tempBuffer2[j0]
					+ A13i*tempBuffer2[j1]
					+ A2  *tempBuffer2[j2]
					- B1i *tempBuffer3[j1]
					- B2i *tempBuffer3[j2];
				tempBuffer4[j0] =
					+ A0  *tempBuffer3[j0]
					+ A14i*tempBuffer3[j1]
					+ A2  *tempBuffer3[j2]
					- B1i *tempBuffer4[j1] 
					- B2i *tempBuffer4[j2];
				outputBuffer[j] = tempBuffer4[j0];
			}

			for(int k=0 ; k < audiosize; k++)
				audioArray[k]=fabsf(audioArray[k]);
				
			//Decimation to 245 Hz
			
			mDecimator.DecimateFrom22050To245(audioArray, filterBankOutputs[i]);

			for(int k=0 ; k< filterBankOutputs[i].Size() ; k++)
				filterBankOutputs[i][k] = filterBankOutputs[i][k]*mCentreFreq[i];
			

		}

		return true;
	}



	void GT_FilterBank::MakeERBFilters()
	{
	
		mCentreFreq.Resize(mnChannels);
		mCentreFreq.SetSize(mnChannels);
	
		ERB_SpaceConfig ERBCfg;
		ERBCfg.SetNumFilter(mnChannels);
		ERBCfg.SetLowFreq(mLowFreq);
		ERBCfg.SetHighFreq(mHighFreq);

		ERB_Space ERBS(ERBCfg);

		ERBS.Do(mCentreFreq);

	
		//Glasberg and Moore Parameters
		TData EarQ = 9.26449;
		TData minBW = 24.7;
		int order = 1;	
	
		TData T=1/(float)mSampleRate;


		A0 = T;
		A2 = 0;
		B0 = 1;	

		DataArray ERB, B;


		ERB.Resize(mCentreFreq.Size());
		ERB.SetSize(mCentreFreq.Size());
		B.Resize(mCentreFreq.Size());
		B.SetSize(mCentreFreq.Size());	
		B1.Resize(mCentreFreq.Size());
		B1.SetSize(mCentreFreq.Size());
		B2.Resize(mCentreFreq.Size());
		B2.SetSize(mCentreFreq.Size());
		A11.Resize(mCentreFreq.Size());
		A11.SetSize(mCentreFreq.Size());
		A12.Resize(mCentreFreq.Size());
		A12.SetSize(mCentreFreq.Size());
		A13.Resize(mCentreFreq.Size());
		A13.SetSize(mCentreFreq.Size());
		A14.Resize(mCentreFreq.Size());
		A14.SetSize(mCentreFreq.Size());
		gain.Resize(mCentreFreq.Size());
		gain.SetSize(mCentreFreq.Size());
	

		Complex a, b, tempGain;

		for(int i=0; i<mnChannels; i++)
		{

			ERB[i] = pow( ( pow(mCentreFreq[i]/EarQ,order) + pow(minBW,order) ) , 1/(float)order);
			B[i] = ERB[i]*2*1.019*M_PI;

			B1[i] = -2*std::cos(2*mCentreFreq[i]*M_PI*T)/exp(B[i]*T);
			B2[i] = exp(-2*B[i]*T);

			A11[i] = -(2*T*std::cos(2*mCentreFreq[i]*M_PI*T)/exp(B[i]*T) + 2*sqrt(3+pow(2.0,1.5))*T*std::sin(2*mCentreFreq[i]*M_PI*T)/exp(B[i]*T))/2;
			A12[i] = -(2*T*std::cos(2*mCentreFreq[i]*M_PI*T)/exp(B[i]*T) - 2*sqrt(3+pow(2.0,1.5))*T*std::sin(2*mCentreFreq[i]*M_PI*T)/exp(B[i]*T))/2;
			A13[i] = -(2*T*std::cos(2*mCentreFreq[i]*M_PI*T)/exp(B[i]*T) + 2*sqrt(3-pow(2.0,1.5))*T*std::sin(2*mCentreFreq[i]*M_PI*T)/exp(B[i]*T))/2;
			A14[i] = -(2*T*std::cos(2*mCentreFreq[i]*M_PI*T)/exp(B[i]*T) - 2*sqrt(3-pow(2.0,1.5))*T*std::sin(2*mCentreFreq[i]*M_PI*T)/exp(B[i]*T))/2;

			a.SetReal(-2*std::cos(4*mCentreFreq[i]*M_PI*T));
			a.SetImag(-2*std::sin(4*mCentreFreq[i]*M_PI*T));

			b.SetReal(2*exp(-B[i]*T)*std::cos(2*mCentreFreq[i]*M_PI*T));
			b.SetImag(2*exp(-B[i]*T)*std::sin(2*mCentreFreq[i]*M_PI*T));

			tempGain =
				( a*T + b*T*(std::cos(2*mCentreFreq[i]*M_PI*T) - sqrt(3-pow(2.0,1.5))*std::sin(2*mCentreFreq[i]*M_PI*T)))*
				( a*T + b*T*(std::cos(2*mCentreFreq[i]*M_PI*T) + sqrt(3-pow(2.0,1.5))*std::sin(2*mCentreFreq[i]*M_PI*T)))*
				( a*T + b*T*(std::cos(2*mCentreFreq[i]*M_PI*T) - sqrt(3+pow(2.0,1.5))*std::sin(2*mCentreFreq[i]*M_PI*T)))*
				( a*T + b*T*(std::cos(2*mCentreFreq[i]*M_PI*T) + sqrt(3+pow(2.0,1.5))*std::sin(2*mCentreFreq[i]*M_PI*T)))/
				(( a-2/exp(2*B[i]*T)  - (a-2)/exp(B[i]*T))*( a-2/exp(2*B[i]*T)  - (a-2)/exp(B[i]*T))*
				 ( a-2/exp(2*B[i]*T)  - (a-2)/exp(B[i]*T))*( a-2/exp(2*B[i]*T)  - (a-2)/exp(B[i]*T)));

			gain[i] = tempGain.Mag();

		}


	}
	
} // namespace CLAM

