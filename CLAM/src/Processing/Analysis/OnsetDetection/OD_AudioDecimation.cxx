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

//Resampling using iir filter (coef from Matlab)
//Filtered in both ways in order to avoid phase distortion

#include "OD_AudioDecimation.hxx"

namespace CLAM
{

	namespace OnsetDetection
	{

		AudioDecimator::AudioDecimator()
		{
			Configure(AudioDecimatorConfig());
		}

		AudioDecimator::AudioDecimator(AudioDecimatorConfig& c)
		{
			Configure(c);
		}

		AudioDecimator::~AudioDecimator() {}


		bool AudioDecimator::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );
			
			return true;
		}

		bool AudioDecimator::Do(void) 
		{
			return false;
		}

		void AudioDecimator::DecimateFrom44100To22050( const DataArray& signal,
							       DataArray& decimatedSignal )
		{
			TSize size = signal.Size();

			if ( mOutput.Size() < signal.Size() )
			{
				mOutput.Resize( signal.Size() );
			}					
			mOutput.SetSize( signal.Size() );			

			double a0, a1, a2,a3, a4, a5, a6, a7, a8, b0, b1, b2, b3, b4, b5, b6, b7, b8;
			//coefs from Matlab for cheby1 filter order=8, R=0.5, wn=0.4.
			a0 = 0.00034708996409;
			a1 = 0.00277671971273;
			a2 = 0.00971851899455;
			a3 = 0.01943703798909;
			a4 = 0.02429629748636;
			a5 = 0.01943703798909;
			a6 = 0.00971851899455;
			a7 = 0.00277671971273;
			a8 = 0.00034708996409;
			
			b1 = -3.86557532033009;
			b2 = 8.26246590570273;
			b3 = -11.69390652917087;
			b4 = 11.77561171603404;
			b5 = -8.54420813616590;
			b6 = 4.35590129644984;
			b7 = -1.43430940799601;
			b8 = 0.23814049785960;			
			
			int i;

			// The "difficult" part of the filtering loop has been stripmined
			
			mOutput[0] = a0*signal[0];

			mOutput[1] = a0*signal[1]+ a1*signal[0]
				- b1*mOutput[0];

			mOutput[2] = a0*signal[2]+ a1*signal[1] + a2*signal[0] 
				- b1*mOutput[1] - b2*mOutput[0];

			mOutput[3] = a0*signal[3]+ a1*signal[2] + a2*signal[1] + a3*signal[0] 
				- b1*mOutput[2] - b2*mOutput[1] - b3*mOutput[0];

			mOutput[4] = a0*signal[4]+ a1*signal[3] + a2*signal[2] + a3*signal[1]  + a4*signal[0]
				- b1*mOutput[3] - b2*mOutput[2] - b3*mOutput[1] - b4*mOutput[0];

			mOutput[5] = a0*signal[5]+ a1*signal[4] + a2*signal[3] + a3*signal[2]  + a4*signal[1]+ a5*signal[0]
				- b1*mOutput[4] - b2*mOutput[3] - b3*mOutput[2] - b4*mOutput[1]- b5*mOutput[0];

			mOutput[6] = a0*signal[6]+ a1*signal[5] + a2*signal[4] + a3*signal[3]  + a4*signal[2]+ a5*signal[1]+ a6*signal[0]
				- b1*mOutput[5] - b2*mOutput[4] - b3*mOutput[3] - b4*mOutput[2]- b5*mOutput[1]- b6*mOutput[0];

			mOutput[7] = a0*signal[7]+ a1*signal[6] + a2*signal[5] + a3*signal[4]  + a4*signal[3]+ a5*signal[2]
				+ a6*signal[1]+ a7*signal[0]
				- b1*mOutput[6] - b2*mOutput[5] - b3*mOutput[4] - b4*mOutput[3]- b5*mOutput[2]
				- b6*mOutput[1]- b7*mOutput[0];
			
			//filtering
			for(i=8; i<size; i++)
			{

				mOutput[i] = a0*signal[i] + a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3] + a4*signal[i-4] + a5*signal[i-5]
					+ a6*signal[i-6] + a7*signal[i-7] + a8*signal[i-8]
					- b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4] - b5*mOutput[i-5] 
					- b6*mOutput[i-6] - b7*mOutput[i-7] - b8*mOutput[i-8];
				
			}
									
			TSize decimatedSize = signal.Size() / 2;
			decimatedSignal.Resize(decimatedSize);
			decimatedSignal.SetSize(decimatedSize);
			
			i=0;
			
			//decimation
			while(i<decimatedSize)
			{
				decimatedSignal[i] = mOutput[i*2];
				i++;
			}
			

		}

		void AudioDecimator::DecimateFrom22050To245( const DataArray& signal,
							     DataArray& decimatedSignal )
		{
			TSize size = signal.Size();
			int i;

			double a0, a1, a2,a3, a4, b1, b2, b3, b4;

			if ( mOutput.Size() < signal.Size() )
			{
				mOutput.Resize( signal.Size() );
			}
			mOutput.SetSize( signal.Size() );

			if ( mOutputRev.Size() < signal.Size() )
			{
				mOutputRev.Resize( signal.Size() );
			}
			mOutputRev.SetSize( signal.Size() );						
						
			a0 = 0.13376242754340e-007;
			a1 = 0.53504971017360e-007;
			a2 = 0.80257456526040e-007;
			a3 = 0.53504971017360e-007;
			a4 = 0.13376242754340e-007;			
			
			b1 = -3.96578818443280;
			b2 = 5.89871439914956;
			b3 = -3.90004183080475;
			b4 = 0.96711584278936;
				
						
			//filtering
			for(i=0; i<size; i++)
			{
				if(i==0) mOutput[i] = a0*signal[i];
				else if(i==1) mOutput[i] = a0*signal[i]+ a1*signal[i-1]- b1*mOutput[i-1];
				else if(i==2) mOutput[i] = a0*signal[i]+ a1*signal[i-1] + a2*signal[i-2] - b1*mOutput[i-1] - b2*mOutput[i-2];
				else if(i==3) mOutput[i] = a0*signal[i]+ a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3] - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3];
				else mOutput[i] = a0*signal[i] + a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3] + a4*signal[i-4] 
					     - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4];
				
				mOutputRev[size-1-i] = mOutput[i];
			}

			//filtering the reverse signal for 0 phase distortion
			for(i=0; i<size; i++)
			{
				if(i==0) mOutput[i] = a0*mOutputRev[i];
				else if(i==1) mOutput[i] = a0*mOutputRev[i]+ a1*mOutputRev[i-1]- b1*mOutput[i-1];
				else if(i==2) mOutput[i] = a0*mOutputRev[i]+ a1*mOutputRev[i-1] + a2*mOutputRev[i-2] - b1*mOutput[i-1] - b2*mOutput[i-2];
				else if(i==3) mOutput[i] = a0*mOutputRev[i]+ a1*mOutputRev[i-1] + a2*mOutputRev[i-2] + a3*mOutputRev[i-3] - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3];
				else mOutput[i] = a0*mOutputRev[i] + a1*mOutputRev[i-1] + a2*mOutputRev[i-2] + a3*mOutputRev[i-3] + a4*mOutputRev[i-4] 
					     - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4];
				
			
			}

			for(i=0;i<size;i++)
				mOutputRev[i]=mOutput[size-1-i];
			
			TSize decimatedSize = size/90;
			
			decimatedSignal.Resize(decimatedSize);
			decimatedSignal.SetSize(decimatedSize);
			
			i=0;
			
			//decimation
			while(i<decimatedSize)
			{
				decimatedSignal[i] = mOutputRev[i*90];
				i++;
			}
			

		}

		void AudioDecimator::Convolute22050SignalWithLowPassEllipticFilter( const DataArray& signal,
								    DataArray& filteredSignal )
		{
			TSize size = signal.Size();
			int i;

			double a0, a1, a2,a3, a4, a5, a6, a7, a8, b0, b1, b2, b3, b4, b5, b6, b7, b8;

			if ( mOutput.Size() < signal.Size() )
			{
				mOutput.Resize( signal.Size() );
			}								

			mOutput.SetSize( signal.Size() );

			a0 = 0.00219187924814;
			a1 = -0.00924853933802;
			a2 = 0.02076893250455;
			a3 = -0.03100961182809;
			a4 = 0.03516942453134;
			a5 = -0.03100961182809;
			a6 = 0.02076893250455;
			a7 = -0.00924853933802;
			a8 = 0.00219187924814;
			
			b1 = -6.73202563634183;
			b2 = 20.52589875623715;
			b3 = -36.90124484257926;
			b4 = 42.70393376784493;
			b5 = -32.53984062505747;
			b6 = 15.93618579254304;
			b7 = -4.58665365495816;
			b8 = 0.59439131759859;	
			
						
			//filtering
			for(i=0; i<size; i++)
			{
				if(i==0) mOutput[i] = a0*signal[i];
				else if(i==1) mOutput[i] = a0*signal[i]+ a1*signal[i-1]- b1*mOutput[i-1];
				else if(i==2) mOutput[i] = a0*signal[i]+ a1*signal[i-1] + a2*signal[i-2] - b1*mOutput[i-1] - b2*mOutput[i-2];
				else if(i==3) mOutput[i] = a0*signal[i]+ a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3] - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3];
				else if(i==4) mOutput[i] = a0*signal[i]+ a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3]  + a4*signal[i-4]
						      - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4];
				else if(i==5) mOutput[i] = a0*signal[i]+ a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3]  + a4*signal[i-4]+ a5*signal[i-5]
						      - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4]- b5*mOutput[i-5];
				else if(i==6) mOutput[i] = a0*signal[i]+ a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3]  + a4*signal[i-4]+ a5*signal[i-5]+ a6*signal[i-6]
						      - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4]- b5*mOutput[i-5]- b6*mOutput[i-6];
				else if(i==7) mOutput[i] = a0*signal[i]+ a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3]  + a4*signal[i-4]+ a5*signal[i-5]+ a6*signal[i-6]+ a7*signal[i-7]
						      - b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4]- b5*mOutput[i-5]- b6*mOutput[i-6]- b7*mOutput[i-7];
				else mOutput[i] = a0*signal[i] + a1*signal[i-1] + a2*signal[i-2] + a3*signal[i-3] + a4*signal[i-4] + a5*signal[i-5]
					     + a6*signal[i-6] + a7*signal[i-7] + a8*signal[i-8]	- b1*mOutput[i-1] - b2*mOutput[i-2] - b3*mOutput[i-3] - b4*mOutput[i-4] - b5*mOutput[i-5] - b6*mOutput[i-6] - b7*mOutput[i-7] - b8*mOutput[i-8];
			}
			

			// :TODO: This seemed to be a decimation, but
			// for some unknown reason it is no longer ?????
			// Ask Fabien and Xavi about this - pretty strange.
			/*
			  TSize decimatedSize= size/5;
			  decimatedSignal.Resize(decimatedSize);
			  decimatedSignal.SetSize(decimatedSize);
			  
			  i=0;
			  
			  //decimation
			  while(i<decimatedSize)
			  {
			  decimatedSignal[i] = mOutput[i*5];
			  i++;
			  }
			*/	
			
			i=0;
			
			while(i<size)
			{
				filteredSignal[i] = mOutput[i];
				i++;
			}

		}
	
		bool AudioDecimator::Do(DataArray &input, int sampleRate, int type)
		{
			TSize size = input.Size();
			int i;

			double a0, a1, a2,a3, a4, a5, a6, a7, a8, b0, b1, b2, b3, b4, b5, b6, b7, b8;
	
			switch(type){
		
			case 1:
				//44100 to 22050
				CLAM_ASSERT( sampleRate == 44100, "Sample rate should be 44100Hz!" );
				
				DecimateFrom44100To22050( input, input );
				break;
		
			case 2:
				//22050 to 245
				CLAM_ASSERT( sampleRate == 22050, "Sample rate should be 22050Hz!" );

				DecimateFrom22050To245( input, input );

				break;

			case 3:
				CLAM_ASSERT( sampleRate == 22050, "Sample rate should be 22050Hz!" );
			
				Convolute22050SignalWithLowPassEllipticFilter( input, input );

				break;
		
			default:
				CLAM_ASSERT( false, "This conversion is not implemented...");
			}

			return true;

		}

	}

} // namespace CLAM

