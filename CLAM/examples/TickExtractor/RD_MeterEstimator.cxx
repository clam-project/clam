/*
 * Author: fabien gouyon 
 * http://www.iua.upf.es/~fgouyon
 * Description:
 *
 * Syntax: C++
 *
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
#include "RD_MeterEstimator.hxx"
#include "Audio.hxx"
#include "Meter.hxx"
#include <iostream>

namespace CLAM
{

	namespace RhythmDescription
	{

		void MeterEstimatorConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
			//default values
			SetTempoLimInf(50);
			SetTempoLimSup(200);
			SetACFUpperLimit(10);
			SetAutomaticIntegTime(true);
			SetACFIntegrationTime(10);
		}

		MeterEstimator::MeterEstimator()
		{
			AttachChildren();

		}

		MeterEstimator::~MeterEstimator()
		{
		}

		const ProcessingConfig& MeterEstimator::GetConfig() const
		{
			return mConfig;
		}

		const char* MeterEstimator::GetClassName() const
		{
			return "MeterEstimator";
		}

		// Configure the Processing Object according to the Config object
		bool MeterEstimator::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );

			ConfigureData();
			ConfigureChildren();

			return true;
		}

		bool MeterEstimator::ConfigureChildren()
		{
			AutoCorrelationTDConfig myACFcfg;
			myACFcfg.AddAll();
			myACFcfg.UpdateData();
			myACFcfg.SetUpperLimit(mConfig.GetACFUpperLimit());
			myACFcfg.SetIntegrationTime(mConfig.GetACFIntegrationTime());
			myACFcfg.SetAutomaticIntegTime(mConfig.GetAutomaticIntegTime());
			mACF.Configure(myACFcfg);

			return true;
		}

		bool MeterEstimator::Do(void)
		{
			CLAM_ASSERT(false, "MeterEstimator::Do(): Supervised mode not implemented");
			return false;
		}


		bool MeterEstimator::Do(Audio& audioIn, const Pulse& beatData, 
					Meter& dataOut)
		{
			const Array<TimeIndex>& beats = beatData.GetIndexes();
			
			if ( beats.Size() == 0 )
			{
				dataOut.SetNumerator(0);
				dataOut.SetDenominator(0);
				
				return true;
			}

			TData globalTempo;
			TData sampleRate = audioIn.GetSampleRate();

			//Beat centering method 3 (mean IBI)


			globalTempo = .0; 
			for (int i=0;i<beats.Size()-1;i++) 
				globalTempo += beats[i+1].GetPosition()-beats[i].GetPosition();
			globalTempo /= beats.Size()-1;
			globalTempo *= sampleRate;
			globalTempo /= 2;

			/*
			globalTempo = (60.0 * sampleRate)/beatData.GetRate();
			TData offset = globalTempo / 2.0;
			*/

			//-------Remove audio DC component------------------
			//TODO

			
			//-------Compute beat descriptors----
			Array<TData> segments; 
			//Beat index recentering method 1 & 3

			for (int i=1;i<beats.Size();i++)   //NB: begins at 1
				segments.AddElem(beats[i].GetPosition()*sampleRate
						 - globalTempo);

			//mSegment.SetAudio(audioIn);
			mSegment.SetHoldsData(true);
			//mSegmentD.SetpSegment(&mSegment);
			
			//List<Segment> & segList = mSegment.GetChildren();
			//List<SegmentDescriptors> & segDList = mSegmentD.GetChildrenD();
			List<Segment> segList;
			List<SegmentDescriptors> segDList;
			
			for(int i=0;i<segments.Size()-1;i++) 
			{
				segList.AddElem(mSegment);
				segDList.AddElem(mSegmentD);
				
				Segment & seg = segList[i];
				SegmentDescriptors & segD = segDList[i];
				
				segD.SetpSegment(&seg);
				
				Audio & audioTmp = seg.GetAudio();
				
				audioIn.GetAudioChunk((int)(segments[i]),
						      (int)(segments[i+1]),audioTmp);
				AudioDescriptors & audioD = segD.GetAudioD();
				mAudioDescGen.Do(audioD);
			}
			
			//---Compute feature Z-values-----
			//TODO
			
			//-------Compute ACFs------------
			Array<TData> seq,acf;
			for (int i=0;i<segments.Size()-1;i++) 
			{
				//temporal centroid mapped to values between 0 and 1
				//MRJ: TemporalCentroid is in seconds!!!!!
				//seq.AddElem(segDList[i].GetAudioD().GetTemporalCentroid()
				// / (segments[i+1]-segments[i]));
				TData centroidTime = segDList[i].GetAudioD().GetTemporalCentroid();
				centroidTime*=sampleRate;
				std::cerr << "Centroid: " << centroidTime << " Segment("<< i+1 <<"): ";
				std::cerr << segments[i+1] << " Segment("<<i<<"): " << segments[i];
				std::cerr << " Width: " << segments[i+1] - segments[i];
				std::cerr << " Factor: " << centroidTime / ( segments[i+1] - segments[i] ) << std::endl; 
				seq.AddElem( centroidTime / ( segments[i+1] - segments[i] ) );
			}

			mACF.Do(seq,acf);
			
			//-------Compute feature M------------
			TSize acfSize = acf.Size();
			if (acfSize<10) 
			{
				std::cout<<"upper limit is too small, unreliable computations..."<< std::endl;
				acf.Resize(10);
				acf.SetSize(10);
				//newly created elements are set to 0 automatically
			}
#if 1
			std::cout<<"ACF"<<std::endl;
			for(int i=0;i<acf.Size();i++) 
				std::cout << "acf("<<i<<"):="<<acf[i]<<"\n";
#endif
			// MRJ: Almost the one on the paper ( coefficient #9 is used instead of #6 ).
			// Fabien changed this so we don't mix ternary and binary hypotheses: 3 and
			// 9 do not have as factor two, while six does. 

			//TData M = (acf[2]+acf[4]+acf[8])/3 - (acf[3]+acf[9])/2;
			TData M = (acf[1]+acf[3]+acf[7])/3.0 - (acf[2]+acf[8])/2.0;
			std::cout<<"Feature M = "<<M<<std::endl;
			
			//--------Final decision--------------
			dataOut.SetDenominator(4);


			//if ( M < -0.108046 )
			//if ( (acf[3]+acf[9])/2 > (acf[2]+acf[4]+acf[8])/3 )
			//
			TData dupleLikelihood = 0.0;
			int twoMults = 0, threeMults = 0;
			TData tripleLikelihood = 0.0;


			for ( int i = 1; i < acf.Size(); i++ )
			{
				if ( fabs(acf[i]) > 1e-7 )
				{

					if ( (i+1)%2 == 0)
					{
						dupleLikelihood+=acf[i];
						twoMults++;
					}
					
					if ( (i+1)%3 == 0)
					{
						tripleLikelihood+=acf[i];
						threeMults++;
					}
				}
			}

			tripleLikelihood *= 1.0/TData(threeMults);
			dupleLikelihood *= 1.0/TData(twoMults);
			
			std::cout << "twoMults= " << twoMults << std::endl;
			std::cout << "threeMults= " << threeMults << std::endl;
			std::cout << "dupleLikelihood = " << dupleLikelihood << std::endl;
			std::cout << "tripleLikelihood = " << tripleLikelihood << std::endl;
			std::cout << "M2 = " << dupleLikelihood - tripleLikelihood << std::endl;

			//if ( (acf[2]+acf[8])/2.0 > (acf[1]+acf[3]+acf[7])/3.0 )

			//if ( tripleLikelihood > dupleLikelihood )
			TData M2 = dupleLikelihood - tripleLikelihood;
			if ( M2 < -0.000665 ) 
			{
				dataOut.SetNumerator(3);
				std::cout<<"Triple (3/4) meter"<<std::endl;
			}
			else 
			{
				dataOut.SetNumerator(4);
				std::cout<<"Duple (4/4) meter"<<std::endl;
			}
					       

			return true;
		}


		
		
		
		void MeterEstimator::AttachChildren()
		{
			mAudioDescGen.SetParent(this);
			mACF.SetParent(this);
		}
		
		void MeterEstimator::ConfigureData()
		{
			mSegment.AddAudio();
			mSegment.AddBeginTime();
			mSegment.AddEndTime();
			//mSegment.AddChildren();
			mSegment.UpdateData();
			
			mSegmentD.RemoveAll();
			mSegmentD.AddAudioD();
			//mSegmentD.AddChildrenD();
			mSegmentD.UpdateData();
			
			AudioDescriptors & audioD = mSegmentD.GetAudioD();
			audioD.RemoveAll();
			audioD.AddTemporalCentroid();
			audioD.UpdateData();
		}
		
		
		
	} // namespace RhythmDescription
	
} // namespace CLAM
