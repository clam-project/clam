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
#include "AudioPeakDetect.hxx"
#include "GlobalPulse.hxx"
#include "TimeDifference.hxx"
#include "GridGen.hxx"
#include "TemporalSeriesSeed.hxx"
#include "TemporalSeriesFinder.hxx"
#include "Audio.hxx"
#include "CLAM_Math.hxx"

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
		AudioPeakDetectConfig apdconf;
		apdconf.SetThreshold(mConfig.GetThreshold_IOIHistPeaks());

		mAudioPeakDetector.Configure( apdconf );

		mAudioPeakDetector.SetParent( this );

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
				     Audio& IOIHist)
	{

		TData globalTick = -1 , globalTempo = -1;
		
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
					  Audio& IOIHist, 
					  Array<TimeIndex>& ticks,Array<TimeIndex>& beats,
					  TData& globalTick, 
					  TData& globalTempo)
	{
		if ( transients.Size() < 5 ) 
		{
			return false;
		}

		beats.Init();
		ticks.Init();
		globalTempo = -1;
		globalTick = -1;

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

		Audio readAudioBuf;
		TData* tdataBuf;

		TimeDifferenceConfig tconf;
		tconf.SetGaussianSize((TSize)(mConfig.GetSamplingRate()*mConfig.GetGaussianWindowSize()));
		TimeDifference tdiff(tconf);

		Array<TimeIndex>  transientsForHist(numbTrans);
		transientsForHist.SetSize(numbTrans);


		Array<TimeIndex>  IOIHistPeaks;

		TData tempo;

		TemporalSeriesSeed tickFirstGuess, goodTick, goodTempo;
		tickFirstGuess.SetOffset(0); tickFirstGuess.SetInterval(1);
		goodTick.SetOffset(0); goodTick.SetInterval(1);
		goodTempo.SetOffset(0); goodTempo.SetInterval(1);
		TemporalSeriesFinderConfig myTemporalSeriesFinderConfig;
		TemporalSeriesFinder myTemporalSeriesFinder(myTemporalSeriesFinderConfig);

		const TData tempoLimInf = mConfig.GetTempoLimInf(); //BPM
		const TData tempoLimSup = mConfig.GetTempoLimSup();
		const TData tickLimInfS = mConfig.GetTickLimInf(); //seconds
		const TData tickLimSupS = mConfig.GetTickLimSup();
		const int tickLimInf = tickLimInfS*mConfig.GetSamplingRate(); //samples
		const int tickLimSup = tickLimSupS*mConfig.GetSamplingRate();

		Array<TimeIndex> tickArray, tempoArray;

		Array<TData> forGlobalTempoCalc;
		forGlobalTempoCalc.Init();
		Array<TData> forGlobalTickCalc;
		forGlobalTickCalc.Init();

		int nLoops = 0;
		while (indTrans2<transients.Size() && stop<2)
		{
			posTrans1 = transients[indTrans1].GetPosition();
			posTrans2 = transients[indTrans2].GetPosition();
			windowSize = posTrans2-posTrans1;
			//put a maximum on the IOIHist length
			TData IOIHistLim = 10.0*mConfig.GetSamplingRate();
			IOIHist.SetSize(CLAM::CLAM_min(windowSize,IOIHistLim));

			/// Compute the IOIHistogram
			//Don't use weights for the building of the histogram:
			transientsForHist[0].SetWeight(0); //because the 1st transient is added manually
			//transientsForHist[0].SetPosition(posTrans1);//Original
			transientsForHist[0].SetPosition(0);
			for (int i=1;i<transientsForHist.Size();i++)
			{
				transientsForHist[i].SetPosition
					(transients[indTrans1+i].GetPosition()-posTrans1);
				transientsForHist[i].SetWeight(1); //All weights to 1
			}

			tdiff.Start();
			tdiff.Do(transientsForHist,IOIHist);
			tdiff.Stop();

			///IOI histogram Peak Detection

			mAudioPeakDetector.Do(IOIHist,IOIHistPeaks);


			///Compute Tempo (optional)
			if (computeBeats)
			{

				TData maxForTempo=0.0;
				int indexForTempo=0;
				for (int i=1;i < IOIHistPeaks.Size() ;i++) //starts at 1 because there is a peak at 0 (with 0 weight)
				{
					//Tempo is between tempoLimInf and tempoLimSup BPM
					if ((IOIHistPeaks[i].GetPosition() > mConfig.GetSamplingRate()*60.0/tempoLimSup)
					    && (IOIHistPeaks[i].GetPosition() < mConfig.GetSamplingRate()*60.0/tempoLimInf)
					    && (IOIHistPeaks[i].GetWeight() > maxForTempo))
					{
						maxForTempo = IOIHistPeaks[i].GetWeight();
						indexForTempo = i;
					}
				}
				if (indexForTempo==0) {

					tempo = 60.0*mConfig.GetSamplingRate()/((tempoLimSup+tempoLimInf)/2);
				}
				else
					tempo=IOIHistPeaks[indexForTempo].GetPosition();
			}

			///Tick Estimation

			//Here the offset is set to 0 as the computation is done
			// over the histogram peaks
			//Thus, OffsetStep=tickLimInf ==> no offset seeking
			myTemporalSeriesFinderConfig.SetOffsetMin(0);
			myTemporalSeriesFinderConfig.SetOffsetStep(tickLimInf);
			myTemporalSeriesFinderConfig.SetIntervalMin(tickLimSup);
			myTemporalSeriesFinderConfig.SetIntervalMax(tickLimInf);
			myTemporalSeriesFinderConfig.SetIntervalStep(10);
			//Use of both errors:
			myTemporalSeriesFinderConfig.SetDeviationPenalty(mConfig.GetDeviationPenalty());
			myTemporalSeriesFinderConfig.SetOverSubdivisionPenalty(mConfig.GetOverSubdivisionPenalty());
			// default value: 2
			// bigger --> favor large ticks
			myTemporalSeriesFinder.Configure(myTemporalSeriesFinderConfig);
			myTemporalSeriesFinder.Start();
			//Use of histogram peak weights
			myTemporalSeriesFinder.Do(IOIHistPeaks,tickFirstGuess);
			myTemporalSeriesFinder.Stop();
			unsigned int tickFirstGuessInterval = tickFirstGuess.GetInterval();


			if (mConfig.GetTickAdjustForSwing()) {

				//quarter-note is either = tick, 2 ticks, 3 ticks or 4 ticks
				TData quarternote = tickFirstGuessInterval;
				TData max = 0.0;
				Array<TData> candidates;
				TData* arr = IOIHist.GetBuffer().GetPtr();
				TData tmpCand=tickFirstGuessInterval;
				for(int i=0;i<3;i++) {
					if((tmpCand>mConfig.GetSamplingRate()*60.0/tempoLimSup) 
					   && (tmpCand<mConfig.GetSamplingRate()*60.0/tempoLimInf)) {
						candidates.AddElem(tmpCand);
					}
					tmpCand+=tickFirstGuessInterval;
				}
				for(int i=0;i<candidates.Size();i++) {
					if (arr[(int)(candidates[i])]>max) {
						max = arr[(int)(candidates[i])];
						quarternote = candidates[i];	
					} 
				}
				tickFirstGuessInterval = quarternote;

			}



			unsigned int goodTickInterval,goodTickOffset;
			GridGen pulseGridGen;
			if (mConfig.GetAdjustWithOnsets()) {
				///Adjust pulses and generate arrays of pulses
				///Tick adjustment

				unsigned int scope = CLAM::CLAM_min(TData(mConfig.GetScope()*mConfig.GetSamplingRate()),
								    TData(tickFirstGuessInterval*0.5));
				myTemporalSeriesFinderConfig.SetOffsetMin(0);
				myTemporalSeriesFinderConfig.SetOffsetStep(50);
				myTemporalSeriesFinderConfig.SetIntervalMin(
					CLAM::CLAM_max(TData(tickFirstGuessInterval-scope*0.5),TData(tickLimSup)));
				myTemporalSeriesFinderConfig.SetIntervalMax(
					tickFirstGuessInterval+scope/2);
				myTemporalSeriesFinderConfig.SetIntervalStep(10);
				//Use of a single error:
				myTemporalSeriesFinderConfig.SetOverSubdivisionPenalty(0);
				myTemporalSeriesFinder.Configure(myTemporalSeriesFinderConfig);
				myTemporalSeriesFinder.Start();
				//Use of transientsForHist or transients???
				// i.e. use of weights or not???
				//myTemporalSeriesFinder.Do(transientsForHist,goodTick);
				myTemporalSeriesFinder.Do(transients,goodTick);		
				myTemporalSeriesFinder.Stop();
				goodTickInterval = goodTick.GetInterval();
				goodTickOffset = goodTick.GetOffset();
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
				if(tempo==0) tempo = goodTickInterval;
				if (mConfig.GetAdjustWithOnsets()) 
				{
					//get the best phase
					// Computing best beat phase
					myTemporalSeriesFinderConfig.SetOffsetMin(goodTickOffset);			
					myTemporalSeriesFinderConfig.SetOffsetStep(goodTickInterval);
					myTemporalSeriesFinderConfig.SetIntervalMin(tempo);
					myTemporalSeriesFinderConfig.SetIntervalMax(tempo+1);
					myTemporalSeriesFinderConfig.SetIntervalStep(2);
					myTemporalSeriesFinder.Configure(myTemporalSeriesFinderConfig);
					myTemporalSeriesFinder.Start();
					//NB: Use of transients instead of transientsForHist
					// i.e. making use of transient weights
					myTemporalSeriesFinder.Do(transients,goodTempo);
					myTemporalSeriesFinder.Stop();
					goodTempoInterval = goodTempo.GetInterval();
					goodTempoOffset = goodTempo.GetOffset();
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

		///Compute Global tempo
		GlobalPulseConfig gpconf;
		gpconf.SetGaussianSize((TSize)(mConfig.GetSamplingRate()*mConfig.GetGaussianWindowSize()));
		//This is bad, There should be a global attribute specifying this size
		GlobalPulse gpulse(gpconf);
		if (computeBeats)
			globalTempo = CompGlobPulse(gpulse,
						    (mConfig.GetSamplingRate()*60.0)/tempoLimInf, forGlobalTempoCalc) / mConfig.GetSamplingRate();
		///Compute Global tick
		globalTick = CompGlobPulse(gpulse, tickLimInf, 
					   forGlobalTickCalc) / mConfig.GetSamplingRate();


		return true;

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

	TData TickSequenceTracker::CompGlobPulse(GlobalPulse& gpulse,
						 const int pulseLimSup, 
						 const Array<TData> &forGlobalPulseCalc)
	{
		Audio pulseHist;
		pulseHist.SetSize((int) (pulseLimSup +10000));//just for security
		gpulse.Start();
		gpulse.Do(forGlobalPulseCalc,pulseHist);
		gpulse.Stop();
		Array<TimeIndex>  pulseHistPeaks;
		pulseHistPeaks.Init();

		AudioPeakDetectConfig apdconf;
		apdconf.SetThreshold(0.0);
		mAudioPeakDetector.Stop();
		mAudioPeakDetector.Configure(apdconf);
		mAudioPeakDetector.Start();

		mAudioPeakDetector.Do(pulseHist,pulseHistPeaks);


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


	void TickSequenceTracker::GeneratePulseGrid(const TData start, const TData gap, 
					       const TData end, GridGen& pulseGridGen, 
					       Array<TimeIndex>& pulseArray)
	{
		pulseArray.Init();
		GridGenConfig pulseGridConf=dynamic_cast<const GridGenConfig&>(pulseGridGen.GetConfig());
		pulseGridConf.SetStart(start);
		pulseGridConf.SetGap(gap);
		pulseGridConf.SetEnd(end);
		pulseGridGen.Configure(pulseGridConf);
		pulseGridGen.Start();
		pulseGridGen.Do(pulseArray);
		pulseGridGen.Stop();
	}

} // namespace CLAM
