#ifndef __TICKSEQUENCETRACKER__
#define __TICKSEQUENCETRACKER__

#include "ProcessingComposite.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"
#include "TickSequenceTrackerConfig.hxx"
#include "IOIHistPeakDetector.hxx"
#include "IOIHistogram.hxx"
#include "Pulse.hxx"
#include "RD_TimeDifference.hxx"
#include "RD_TimeSeriesFinder.hxx"
#include "RD_TimeSeriesSeed.hxx"
#include "AdjustTickWRTSwing.hxx"
#include "AdjustTickWRTOnsets.hxx"
#include "AdjustBeatWRTTick.hxx"
#include "AdjustBeatWRTOnsets.hxx"

namespace CLAM
{

	class Audio;


	namespace RhythmDescription
	{
		class GlobalPulseGenerator;
		class PulseGridGenerator;


		class TickSequenceTracker : public ProcessingComposite
		{
			TickSequenceTrackerConfig mConfig;
			const char *GetClassName() const {return "TickSequenceTracker";}
			bool ConcreteConfigure(const ProcessingConfig&);

		public:
			TickSequenceTracker();

			TickSequenceTracker(const TickSequenceTrackerConfig &c);

			~TickSequenceTracker() {}

			const ProcessingConfig &GetConfig() const { return mConfig;}

			bool Do(void);

			///Method to use when the input is a list of note onsets
			///(onsets already computed, or MIDI)
			bool Do(const Array<TimeIndex>& transients, Pulse& tickSequence,
				Pulse& beatSequence, IOIHistogram& interOnsetHist );




		protected:
			bool Compute(const Array<TimeIndex>& transients, 
				     IOIHistogram& interOnsetHist,
				     Array<TimeIndex>& ticks,
				     Array<TimeIndex>& beats,
				     TData& globalTick,
				     TData& globalTempo);

			TData CompGlobPulse( GlobalPulseGenerator& gpulse, 
					     const int pulseLimSup, 
					     const Array<TData> &forGlobalPulseCalc);

			void StorePulseIndexes( const int nLoops, 
						const Array<TimeIndex>& pulsesArray,
						Array<TimeIndex>& mPulses );

			void GeneratePulseGrid( const TData start, const TData gap, const TData end, 
						PulseGridGenerator& pulseGridGen, 
						Array<TimeIndex>& pulseArray );


			TData    ComputeTempo( Array<TimeIndex>& IOIHistPeaks );
				       

		private: // children Processing

			IOIHistPeakDetector     mPeakDetector;
			TimeDifference          mTemporalDiff;
			TimeSeriesFinder        mTimeSeriesFinder;
			TimeSeriesFinderConfig  mTSFConfig;
			AdjustTickWRTSwing      mTickSwingAdjuster;
			AdjustTickWRTOnsets     mTickOnsetsAdjuster;
			AdjustBeatWRTTick       mBeatTickAdjuster;
			AdjustBeatWRTOnsets     mBeatOnsetsAdjuster;

		private: // Internal PD's
		
			TimeSeriesSeed    mTickFirstGuess;
			TimeSeriesSeed    mGoodTick;
			TimeSeriesSeed    mGoodTempo;

		};

	} // RhythmDescription

} // namespace CLAM


#endif // TickSequenceTracker.hxx
