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
			Configure(MeterEstimatorConfig());
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
			AutocorrelationTDConfig myACFcfg;
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


		bool MeterEstimator::Do(Audio& audioIn, const Array<TimeIndex>& beats, 
					Meter& dataOut)
		{
			if ( beats.Size() == 0 )
			{
				dataOut.SetNumerator(0);
				dataOut.SetDenominator(0);
				
				return true;
			}

			TData globalTempo, globalTick;
			//Beat centering method 1
			//globalTempo = beats[1].GetPosition()-beats[0].GetPosition(); 
			//Beat centering method 3 (mean IBI)
			globalTempo = .0; 
			for (int i=0;i<beats.Size()-1;i++) 
				globalTempo += beats[i+1].GetPosition()-beats[i].GetPosition();
			globalTempo /= beats.Size()-1;
			
			TData sampleRate = audioIn.GetSampleRate();

			//-------Remove audio DC component------------------
			//TODO

			
			
			//-------Compute beat descriptors----
			Array<TData> segments; segments.Init();
			//Beat index recentering method 1 & 3
			for (int i=1;i<beats.Size();i++)   //NB: begins at 1
				segments.AddElem(beats[i].GetPosition()*sampleRate
						 -globalTempo*sampleRate/2);
			//Beat index recentering method 2
/*			for (int i=1;i<beats.Size();i++)   //NB: begins at 1
			segments.AddElem((beats[i].GetPosition()+
			beats[i-1].GetPosition())*sampleRate/2);
*/
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
			for (int i=0;i<segments.Size()-1;i++) {
				//temporal centroid mapped to values between 0 and 1
				seq.AddElem(segDList[i].GetAudioD().GetTemporalCentroid()
					    / (segments[i+1]-segments[i]));
			}
#if 0
			std::cout<<"temp cent between 0 and 1"<<std::endl;
			for(int i=0;i<segments.Size()-1;i++) 
				std::cout<<seq[i]<<"\n";
#endif
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
			for(int i=0;i<acf.Size()-1;i++) 
				std::cout<<	acf[i]<<"\n";
#endif
			//TData M = (acf[2]+acf[4]+acf[8])/3 - (acf[3]+acf[6]+acf[9])/3;
			TData M = (acf[2]+acf[4]+acf[8])/3 - (acf[3]+acf[9])/2;
			std::cout<<"Feature M = "<<M<<std::endl;
			
			//--------Final decision--------------
			dataOut.SetDenominator(4);
			if(M<-0.000665) {
				dataOut.SetNumerator(3);
				std::cout<<"Triple meter"<<std::endl;
			}
			else {
				dataOut.SetNumerator(4);
				std::cout<<"Duple meter"<<std::endl;
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
