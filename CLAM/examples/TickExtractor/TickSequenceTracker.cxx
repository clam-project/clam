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

#include "TickSequenceTracker.hxx"
#include "RD_GlobalPulse.hxx"
#include "RD_GridGen.hxx"
#include "Audio.hxx"
#include "CLAM_Math.hxx"
#include <list>

namespace CLAM
{

	TickSequenceTracker::TickSequenceTracker()
	{
		Configure(TickSequenceTrackerConfig());

	}

	TickSequenceTracker::TickSequenceTracker(const TickSequenceTrackerConfig &c)
	{
		Configure(c);
	}

	// Configure the Processing Object according to the Config object
	bool TickSequenceTracker::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig( mConfig, c );

		//For Hist peaks:
		RhythmDescription::IOIHistPeakDetectorConfig apdconf;
		apdconf.SetThreshold(mConfig.GetThreshold_IOIHistPeaks());

		mPeakDetector.Configure( apdconf );

		mPeakDetector.SetParent( this );

		RhythmDescription::TimeDifferenceConfig tconf;
		tconf.SetGaussianSize((TSize)(mConfig.GetSamplingRate()*mConfig.GetGaussianWindowSize()));

		mTemporalDiff.Configure( tconf );
		
		mTemporalDiff.SetParent( this );
		
		TemporalSeriesFinderConfig tsfConfig;

		mTimeSeriesFinder.Configure( tsfConfig );

		mTimeSeriesFinder.SetParent( this );

		return true;
	}

	bool  TickSequenceTracker::Do(void)
	{
		CLAM_ASSERT(false, "TickSequenceTracker::Do(): Supervised mode not implemented");
		return false;
	}


	bool TickSequenceTracker::Do(const Array<TimeIndex>& transients,
				     Pulse& tickSequence,
				     Pulse& beatSequence,
				     RhythmDescription::IOIHistogram& IOIHist)
	{

		TData globalTick = -1 , globalTempo = -1;

		mTickFirstGuess.SetOffset(0); 
		mTickFirstGuess.SetInterval(1);
		
		mGoodTick.SetOffset(0); 
		mGoodTick.SetInterval(1);
		
		mGoodTempo.SetOffset(0); 
		mGoodTempo.SetInterval(1);

		
		Compute( transients, 
			 IOIHist, 
			 tickSequence.GetIndexes(), 
			 beatSequence.GetIndexes(), 
			 globalTick, 
			 globalTempo);
		
		if (globalTick==-1) 
		{
			tickSequence.SetRate(0.0);
			beatSequence.SetRate(0.0);
		}
		else 
		{
			tickSequence.SetRate(60.0/globalTick);
			beatSequence.SetRate(60.0/globalTempo);
		}
		
		return true;
	}



	bool TickSequenceTracker::Compute(const Array<TimeIndex>& transients, 
					  RhythmDescription::IOIHistogram& IOIHist, 
					  Array<TimeIndex>& ticks,Array<TimeIndex>& beats,
					  TData& globalTick, 
					  TData& globalTempo)
	{
		if ( transients.Size() < 5 ) 
		{
			return false;
		}

		bool computeBeats = mConfig.GetComputeBeats();
		int stop = 0;
		int numbTrans = mConfig.GetNTrans();
		//If one gives a large value for numbTrans, the computation is done
		// in a single loop
		if (numbTrans>transients.Size())
		{
			numbTrans = transients.Size();
			stop = 1;
		}

		int transHop = mConfig.GetTransHop();
		int indTrans1 = 0;
		int indTrans2 = numbTrans-1;
		int posTrans1;
		int posTrans2;

		TData windowSize;

		Array<TimeIndex>  transientsForHist(numbTrans);
		transientsForHist.SetSize(numbTrans);
		//Don't use weights for the building of the histogram:
		transientsForHist[0].SetWeight(0); //because the 1st transient is added manually			
		transientsForHist[0].SetPosition(0);


		Array<TimeIndex>  IOIHistPeaks;

		TData tempo;

		const TData tempoLimInf = mConfig.GetTempoLimInf(); //BPM
		const TData tempoLimSup = mConfig.GetTempoLimSup();
		const TData tickLimInfS = mConfig.GetTickLimInf(); //seconds
		const TData tickLimSupS = mConfig.GetTickLimSup();
		const int tickLimInf = tickLimInfS*mConfig.GetSamplingRate(); //samples
		const int tickLimSup = tickLimSupS*mConfig.GetSamplingRate();

		Array<TimeIndex> tickArray, tempoArray;

		Array<TData> forGlobalTempoCalc;

		Array<TData> forGlobalTickCalc;
		
		//put a maximum on the IOIHist length
		const TData IOIHistLim = 10.0*mConfig.GetSamplingRate();

		int nLoops = 0;

		while (indTrans2<transients.Size() && stop<2)
		{
			posTrans1 = transients[indTrans1].GetPosition();
			posTrans2 = transients[indTrans2].GetPosition();
			windowSize = posTrans2-posTrans1;
			
			TSize IOIhistSize = CLAM::CLAM_min(windowSize,IOIHistLim);
			IOIHist.GetBins().Resize( IOIhistSize );
			IOIHist.GetBins().SetSize( IOIhistSize );

			/// Compute the IOIHistogram
			
			for (int i=1;i<transientsForHist.Size();i++)
			{
				transientsForHist[i].SetPosition
					(transients[indTrans1+i].GetPosition()-posTrans1);
				transientsForHist[i].SetWeight(1); //All weights to 1
			}


			mTemporalDiff.Do(transientsForHist,IOIHist);


			///IOI histogram Peak Detection

			mPeakDetector.Do(IOIHist,IOIHistPeaks);


			///Compute Tempo (optional)
			if (computeBeats)
			{
				tempo = ComputeTempo( IOIHistPeaks );
			}

			///Tick Estimation

			//Here the offset is set to 0 as the computation is done
			// over the histogram peaks
			//Thus, OffsetStep=tickLimInf ==> no offset seeking
			mTSFConfig.SetOffsetMin(0);
			mTSFConfig.SetOffsetStep(tickLimInf);
			mTSFConfig.SetIntervalMin(tickLimSup);
			mTSFConfig.SetIntervalMax(tickLimInf);
			mTSFConfig.SetIntervalStep(10);
			//Use of both errors:
			mTSFConfig.SetDeviationPenalty(mConfig.GetDeviationPenalty());
			mTSFConfig.SetOverSubdivisionPenalty(mConfig.GetOverSubdivisionPenalty());
			// default value: 2
			// bigger --> favor large ticks
			mTimeSeriesFinder.Stop();
			mTimeSeriesFinder.Configure(mTSFConfig);
			mTimeSeriesFinder.Start();
			//Use of histogram peak weights
			mTimeSeriesFinder.Do(IOIHistPeaks,mTickFirstGuess);

			unsigned int tickFirstGuessInterval = mTickFirstGuess.GetInterval();


			if ( mConfig.GetTickAdjustForSwing() ) 
			{				
				tickFirstGuessInterval = AdjustTickIntervalForSwing( IOIHist, 
										     tickFirstGuessInterval );
			}



			unsigned int goodTickInterval,goodTickOffset;
			RhythmDescription::GridGen pulseGridGen;
			
			if (mConfig.GetAdjustWithOnsets()) 
			{
				///Adjust pulses and generate arrays of pulses
				///Tick adjustment

				unsigned int scope = CLAM::CLAM_min(TData(mConfig.GetScope()*mConfig.GetSamplingRate()),
								    TData(tickFirstGuessInterval*0.5));
				mTSFConfig.SetOffsetMin(0);
				mTSFConfig.SetOffsetStep(50);
				mTSFConfig.SetIntervalMin(
					CLAM::CLAM_max(TData(tickFirstGuessInterval-scope*0.5),TData(tickLimSup)));
				mTSFConfig.SetIntervalMax(
					tickFirstGuessInterval+scope/2);
				mTSFConfig.SetIntervalStep(10);
				//Use of a single error:
				mTSFConfig.SetOverSubdivisionPenalty(0);

				mTimeSeriesFinder.Stop();
				mTimeSeriesFinder.Configure(mTSFConfig);
				mTimeSeriesFinder.Start();
				//Use of transientsForHist or transients???
				// i.e. use of weights or not???
				//myTemporalSeriesFinder.Do(transientsForHist,mGoodTick);
				mTimeSeriesFinder.Do(transients,mGoodTick);		

				goodTickInterval = mGoodTick.GetInterval();
				goodTickOffset = mGoodTick.GetOffset();
				///Generate tick indexes array
				GeneratePulseGrid((posTrans1+goodTickOffset)/mConfig.GetSamplingRate(),
						  goodTickInterval/mConfig.GetSamplingRate(), posTrans2/mConfig.GetSamplingRate(),
						  pulseGridGen,tickArray);			
			}
			else
				goodTickInterval = tickFirstGuessInterval;

			forGlobalTickCalc.AddElem(goodTickInterval);

			if (computeBeats) 
			{
				///Tempo adjustment (optional)
				unsigned int goodTempoInterval, goodTempoOffset;
				//set the tempo to the closest exact multiple of the tick
				tempo = ((int)(tempo+goodTickInterval/2)/goodTickInterval)
					*goodTickInterval;
				while (tempo<mConfig.GetSamplingRate()*60.0/tempoLimSup)
					tempo += goodTickInterval;
				while (tempo>mConfig.GetSamplingRate()*60.0/tempoLimInf)
					tempo -= goodTickInterval;

				if(tempo==0) 
					tempo = goodTickInterval;
				
				if (mConfig.GetAdjustWithOnsets()) 
				{
					//get the best phase
					// Computing best beat phase
					mTSFConfig.SetOffsetMin(goodTickOffset);			
					mTSFConfig.SetOffsetStep(goodTickInterval);
					mTSFConfig.SetIntervalMin(tempo);
					mTSFConfig.SetIntervalMax(tempo+1);
					mTSFConfig.SetIntervalStep(2);
					mTimeSeriesFinder.Stop();
					mTimeSeriesFinder.Configure(mTSFConfig);
					mTimeSeriesFinder.Start();

					//NB: Use of transients instead of transientsForHist
					// i.e. making use of transient weights
					mTimeSeriesFinder.Do(transients,mGoodTempo);

					goodTempoInterval = mGoodTempo.GetInterval();
					goodTempoOffset = mGoodTempo.GetOffset();
					///Generate beat indexes array
					GeneratePulseGrid((posTrans1+goodTempoOffset)/mConfig.GetSamplingRate(),
							  goodTempoInterval/mConfig.GetSamplingRate(), posTrans2/mConfig.GetSamplingRate(),
							  pulseGridGen,tempoArray);
				}
				else 
					goodTempoInterval = tempo;

				forGlobalTempoCalc.AddElem(goodTempoInterval);
			}


			nLoops +=1;
			indTrans1 = nLoops*transHop;
			indTrans2 = nLoops*transHop+numbTrans-1;
			if (indTrans2>=transients.Size())
			{
				indTrans2 = transients.Size()-1;
				indTrans1 = indTrans2-numbTrans+1;
				stop += 1;
			}

			///Store Tick indexes and (optionally) Beat indexes
			if (computeBeats)
				StorePulseIndexes(nLoops, tempoArray, beats);
			
			StorePulseIndexes(nLoops, tickArray, ticks);

		
		} //end of while loop

		std::cerr << "Number of loops: " << nLoops;
		///Compute Global tempo
		RhythmDescription::GlobalPulseConfig gpconf;
		gpconf.SetGaussianSize((TSize)(mConfig.GetSamplingRate()*mConfig.GetGaussianWindowSize()));
		//This is bad, There should be a global attribute specifying this size
		RhythmDescription::GlobalPulse gpulse(gpconf);
		if (computeBeats)
			globalTempo = CompGlobPulse(gpulse,
						    (mConfig.GetSamplingRate()*60.0)/tempoLimInf, forGlobalTempoCalc) / mConfig.GetSamplingRate();
		///Compute Global tick
		globalTick = CompGlobPulse(gpulse, tickLimInf, 
					   forGlobalTickCalc) / mConfig.GetSamplingRate();


		return true;

	}

	unsigned TickSequenceTracker::AdjustTickIntervalForSwing( RhythmDescription::IOIHistogram& ioiHist,
								  unsigned prevTickInterval )
	{
		//quarter-note is either = tick, 2 ticks, 3 ticks or 4 ticks
		TData quarternote = prevTickInterval;
		TData max = 0.0;
		
		std::list<TData> candidates;
		typedef std::list<TData>::iterator LI;
		
		TData* arr = ioiHist.GetBins().GetPtr();		
		TData tmpCand= prevTickInterval;
		
		TData upperBound = mConfig.GetSamplingRate()*60.0/ mConfig.GetTempoLimSup();
		TData lowerBound = mConfig.GetSamplingRate()*60.0/ mConfig.GetTempoLimInf();

		for(int i=0;i<3;i++) 
		{
			if( (tmpCand>upperBound) 
			    && (tmpCand<lowerBound) ) 
			{
				candidates.push_back(tmpCand);
			}
			
			tmpCand += prevTickInterval;
		}
		
		for( LI i=candidates.begin();
		     i != candidates.end(); i++) 
		{
			if (arr[(int)(*i)] > max) 
			{
				max = arr[(int)*i];
				quarternote = *i;	
			} 
		}
		
		return (unsigned)quarternote;
	}

	void TickSequenceTracker::StorePulseIndexes(const int nLoops,
					       const Array<TimeIndex>& pulsesArray, Array<TimeIndex>& mPulses)
	{
		int i = 0;
		if (nLoops == 1)
			while (i<pulsesArray.Size())
			{
				mPulses.AddElem(pulsesArray[i]);
				i+=1;
			}
		else
		{
			while ((pulsesArray[i].GetPosition() <
				mPulses[mPulses.Size()-1].GetPosition()) &&
			       (i<pulsesArray.Size()))
			{
				i+=1;
			}
			while (i<pulsesArray.Size())
			{
				mPulses.AddElem(pulsesArray[i]);
				i+=1;
			}
		}
	}

	TData TickSequenceTracker::CompGlobPulse(RhythmDescription::GlobalPulse& gpulse,
						 const int pulseLimSup, 
						 const Array<TData> &forGlobalPulseCalc)
	{
		RhythmDescription::IOIHistogram pulseHist;
		pulseHist.SetBinRate( mConfig.GetSamplingRate() );
		pulseHist.GetBins().Resize((int) (pulseLimSup +10000));//just for security
		pulseHist.GetBins().SetSize((int) (pulseLimSup +10000));//just for security
		
		gpulse.Start();
		gpulse.Do(forGlobalPulseCalc, pulseHist);
		gpulse.Stop();

		Array<TimeIndex>  pulseHistPeaks;


		RhythmDescription::IOIHistPeakDetectorConfig apdconf;
		apdconf.SetThreshold(0.0);
		mPeakDetector.Stop();
		mPeakDetector.Configure(apdconf);
		mPeakDetector.Start();

		mPeakDetector.Do(pulseHist,pulseHistPeaks);


		int max = 0;
		int index = 0;
		for (int i=1;i < pulseHistPeaks.Size() ;i++) //starts at 1 because there is a peak at 0 (with 0 weight)
			if (pulseHistPeaks[i].GetWeight() > max)
			{
				max =  pulseHistPeaks[i].GetWeight();
				index = i;
			}
			
		return pulseHistPeaks[index].GetPosition();
	}

	TData TickSequenceTracker::ComputeTempo( Array<TimeIndex>& IOIHistPeaks )
	{
		TData tempo;
		TData maxForTempo=0.0;
		int indexForTempo=0;
		
		for (int i=1;i < IOIHistPeaks.Size() ;i++) //starts at 1 because there is a peak at 0 (with 0 weight)
		{
			//Tempo is between tempoLimInf and tempoLimSup BPM
			if ((IOIHistPeaks[i].GetPosition() > mConfig.GetSamplingRate()*60.0/mConfig.GetTempoLimSup())
			    && (IOIHistPeaks[i].GetPosition() < mConfig.GetSamplingRate()*60.0/mConfig.GetTempoLimInf())
			    && (IOIHistPeaks[i].GetWeight() > maxForTempo))
			{
				maxForTempo = IOIHistPeaks[i].GetWeight();
				indexForTempo = i;
			}
		}
		if (indexForTempo==0)			
			tempo = 60.0*mConfig.GetSamplingRate()/((mConfig.GetTempoLimSup()+mConfig.GetTempoLimInf())/2);
		else
			tempo=IOIHistPeaks[indexForTempo].GetPosition();

		return tempo;
	}


	void TickSequenceTracker::GeneratePulseGrid(const TData start, const TData gap, 
					       const TData end, RhythmDescription::GridGen& pulseGridGen, 
					       Array<TimeIndex>& pulseArray)
	{
		RhythmDescription::GridGenConfig pulseGridConf=dynamic_cast<const RhythmDescription::GridGenConfig&>(pulseGridGen.GetConfig());
		pulseGridConf.SetStart(start);
		pulseGridConf.SetGap(gap);
		pulseGridConf.SetEnd(end);
		pulseGridGen.Configure(pulseGridConf);
		pulseGridGen.Start();
		pulseGridGen.Do(pulseArray);
		pulseGridGen.Stop();
	}

} // namespace CLAM
