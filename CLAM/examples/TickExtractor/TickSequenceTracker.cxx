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
#include <algorithm>

namespace CLAM
{

	namespace RhythmDescription
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
			IOIHistPeakDetectorConfig apdconf;
			apdconf.SetThreshold( mConfig.GetThreshold_IOIHistPeaks() );
			apdconf.SetSampleRate( mConfig.GetSamplingRate() );

			mPeakDetector.Configure( apdconf );

			mPeakDetector.SetParent( this );

			TimeDifferenceConfig tconf;
			tconf.SetGaussianSize((TSize)(mConfig.GetSamplingRate()*mConfig.GetGaussianWindowSize()));

			mTemporalDiff.Configure( tconf );
		
			mTemporalDiff.SetParent( this );
		
			TimeSeriesFinderConfig tsfConfig;

			tsfConfig.SetDeviationPenalty(mConfig.GetDeviationPenalty());
			tsfConfig.SetOverSubdivisionPenalty(mConfig.GetOverSubdivisionPenalty());
				
			mTimeSeriesFinder.Configure( tsfConfig );

			mTimeSeriesFinder.SetParent( this );

			AdjustTickWRTSwingConfig swingAdjusterCfg;

			swingAdjusterCfg.SetSampleRate( mConfig.GetSamplingRate() );
			swingAdjusterCfg.SetTempoLimSup( mConfig.GetTempoLimSup() );
			swingAdjusterCfg.SetTempoLimInf( mConfig.GetTempoLimInf() );

			mTickSwingAdjuster.Configure( swingAdjusterCfg );

			mTickSwingAdjuster.SetParent( this );

			AdjustTickWRTOnsetsConfig onsetsAdjusterCfg;

			onsetsAdjusterCfg.SetSampleRate( mConfig.GetSamplingRate() );
			onsetsAdjusterCfg.SetDeviationPenalty( mConfig.GetDeviationPenalty() );
			onsetsAdjusterCfg.SetOverSubdivisionPenalty( mConfig.GetOverSubdivisionPenalty() );
			onsetsAdjusterCfg.SetScope( mConfig.GetScope() );
			onsetsAdjusterCfg.SetTickLimSup( mConfig.GetTickLimSup()*mConfig.GetSamplingRate() );

			mTickOnsetsAdjuster.Configure( onsetsAdjusterCfg );

			mTickOnsetsAdjuster.SetParent( this );

			AdjustBeatWRTTickConfig beatTickAdjusterCfg;
			beatTickAdjusterCfg.SetSampleRate( mConfig.GetSamplingRate() );
			beatTickAdjusterCfg.SetTempoLimSup( mConfig.GetTempoLimSup() );
			beatTickAdjusterCfg.SetTempoLimInf( mConfig.GetTempoLimInf() );

			mBeatTickAdjuster.Configure( beatTickAdjusterCfg );

			mBeatTickAdjuster.SetParent( this );


			AdjustBeatWRTOnsetsConfig beatOnsetsAdjusterCfg;

			beatOnsetsAdjusterCfg.SetSampleRate( mConfig.GetSamplingRate() );
			beatOnsetsAdjusterCfg.SetDeviationPenalty( mConfig.GetDeviationPenalty() );
			beatOnsetsAdjusterCfg.SetOverSubdivisionPenalty( mConfig.GetOverSubdivisionPenalty() );

			mBeatOnsetsAdjuster.Configure( beatOnsetsAdjusterCfg );

			mBeatOnsetsAdjuster.SetParent( this );

			BeatIntervalEstimatorConfig tempoEstCfg;

			tempoEstCfg.SetSampleRate( mConfig.GetSamplingRate() );
			tempoEstCfg.SetTempoLimSup( mConfig.GetTempoLimSup() );
			tempoEstCfg.SetTempoLimInf( mConfig.GetTempoLimInf() );

			mTempoEstimator.Configure( tempoEstCfg );

			mTempoEstimator.SetParent( this );

			GlobalPulseRateEstimatorConfig gpreCfg;

			gpreCfg.SetSampleRate( mConfig.GetSamplingRate() );
			gpreCfg.SetRateLowerBound( mConfig.GetTickLimInf() );
			gpreCfg.SetGaussianSize( mConfig.GetSamplingRate()*mConfig.GetGaussianWindowSize() );

			mGlobalPREstimator.Configure( gpreCfg );

			mGlobalPREstimator.SetParent( this );

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
					     IOIHistogram& IOIHist)
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
						  IOIHistogram& IOIHist, 
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

				TimeSeriesSeed initialBeatParams;

				///Compute Tempo (optional)
				if (computeBeats)
				{
					mTempoEstimator.Do( IOIHistPeaks, initialBeatParams );
					tempo = initialBeatParams.GetInterval();
				}

				///Tick Estimation

				//Here the offset is set to 0 as the computation is done
				// over the histogram peaks
				//Thus, OffsetStep=tickLimInf ==> no offset seeking
				
				mTimeSeriesFinder.GetInControl("OffsetMin").DoControl( 0 );
				mTimeSeriesFinder.GetInControl("OffsetStep").DoControl( tickLimInf );
				mTimeSeriesFinder.GetInControl("IntervalMin").DoControl( tickLimSup );
				mTimeSeriesFinder.GetInControl("IntervalMax").DoControl( tickLimInf );
				mTimeSeriesFinder.GetInControl("IntervalStep").DoControl( 10 );
				
				//Use of both errors:
				// default value: 2				
				//Use of histogram peak weights
				mTimeSeriesFinder.Do(IOIHistPeaks,mTickFirstGuess);

				unsigned int tickFirstGuessInterval = mTickFirstGuess.GetInterval();


				if ( mConfig.GetTickAdjustForSwing() ) 
					mTickSwingAdjuster.Do( IOIHist, tickFirstGuessInterval, tickFirstGuessInterval );


				unsigned int goodTickInterval,goodTickOffset;
			
				if (mConfig.GetAdjustWithOnsets()) 
				{
					///Adjust pulses and generate arrays of pulses
					///Tick adjustment
					mTickOnsetsAdjuster.GetInControl("FirstTransientPosition").DoControl( posTrans1 );
					mTickOnsetsAdjuster.GetInControl("LastTransientPosition").DoControl( posTrans2 );

					//Use of transientsForHist or transients???
					// i.e. use of weights or not???
					//myTemporalSeriesFinder.Do(transientsForHist,mGoodTick);
					
					mTickOnsetsAdjuster.Do(transients,mTickFirstGuess,tickArray,mGoodTick);		

					goodTickInterval = mGoodTick.GetInterval();
					goodTickOffset = mGoodTick.GetOffset();
				}
				else
					goodTickInterval = tickFirstGuessInterval;

				forGlobalTickCalc.AddElem(goodTickInterval);

				if (computeBeats) 
				{
					unsigned int goodTempoInterval, goodTempoOffset;

					TimeSeriesSeed tickAdjustedBeatParams;
					mBeatTickAdjuster.Do( mGoodTick, initialBeatParams, tickAdjustedBeatParams );

					tempo = tickAdjustedBeatParams.GetInterval();

					if (mConfig.GetAdjustWithOnsets()) 
					{
						//get the best phase
						// Computing best beat phase

						mBeatOnsetsAdjuster.GetInControl("FirstTransientPosition").DoControl( posTrans1 );
						mBeatOnsetsAdjuster.GetInControl("LastTransientPosition").DoControl( posTrans2 );

						mBeatOnsetsAdjuster.Do( transients, mGoodTick, tickAdjustedBeatParams,
									tempoArray, mGoodTempo );


						//NB: Use of transients instead of transientsForHist
						// i.e. making use of transient weights

						
						goodTempoInterval = mGoodTempo.GetInterval();
						goodTempoOffset = mGoodTempo.GetOffset();
						///Generate beat indexes array
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
			if (computeBeats)
			{
				TData rateLowerBound = (mConfig.GetSamplingRate()*60.0)/tempoLimInf;
				mGlobalPREstimator.GetInControl( "RateLowerBound" ).DoControl( rateLowerBound );
				mGlobalPREstimator.Do( forGlobalTempoCalc, globalTempo );
			}

			///Compute Global tick
			mGlobalPREstimator.GetInControl( "RateLowerBound" ).DoControl( tickLimInf );
			mGlobalPREstimator.Do( forGlobalTickCalc, globalTick );


			return true;

		}

		void TickSequenceTracker::StorePulseIndexes(const int nLoops,
							    const Array<TimeIndex>& pulsesArray, Array<TimeIndex>& mPulses)
		{
			int i = 0;
			if (nLoops == 1)
			{

				mPulses.Resize( pulsesArray.Size() );
				mPulses.SetSize( pulsesArray.Size() );

				std::copy( pulsesArray.GetPtr(), pulsesArray.GetPtr() + pulsesArray.Size(),
					   mPulses.GetPtr() );

			}
			else
			{
				// MRJ: Concatenates previously found pulses with the new ones

				int lastPosition = mPulses[mPulses.Size()-1].GetPosition();


				while ((pulsesArray[i].GetPosition() <
					lastPosition) &&
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



	} // namespace RhythmDescription

} // namespace CLAM
