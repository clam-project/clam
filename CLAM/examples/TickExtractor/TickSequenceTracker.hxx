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
#include "TemporalSeriesFinder.hxx"
#include "TemporalSeriesSeed.hxx"

namespace CLAM
{

	class Audio;


	namespace RhythmDescription
	{
		class GlobalPulseGenerator;
		class PulseGridGenerator;
	}

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
			Pulse& beatSequence, RhythmDescription::IOIHistogram& interOnsetHist );




	protected:
		bool Compute(const Array<TimeIndex>& transients, 
			     RhythmDescription::IOIHistogram& interOnsetHist,
			     Array<TimeIndex>& ticks,
			     Array<TimeIndex>& beats,
			     TData& globalTick,
			     TData& globalTempo);

		TData CompGlobPulse( RhythmDescription::GlobalPulseGenerator& gpulse, 
				     const int pulseLimSup, 
				     const Array<TData> &forGlobalPulseCalc);

		void StorePulseIndexes( const int nLoops, 
					const Array<TimeIndex>& pulsesArray,
				        Array<TimeIndex>& mPulses );

		void GeneratePulseGrid( const TData start, const TData gap, const TData end, 
					RhythmDescription::PulseGridGenerator& pulseGridGen, 
					Array<TimeIndex>& pulseArray );


		unsigned AdjustTickIntervalForSwing( RhythmDescription::IOIHistogram& ioiHistogram, 
						     unsigned previousTickInterval );

		TData    ComputeTempo( Array<TimeIndex>& IOIHistPeaks );
				       

	private: // children Processing

		RhythmDescription::IOIHistPeakDetector   mPeakDetector;
		RhythmDescription::TimeDifference        mTemporalDiff;
		TemporalSeriesFinder                     mTimeSeriesFinder;
		TemporalSeriesFinderConfig               mTSFConfig;

	private: // Internal PD's
		
		TemporalSeriesSeed    mTickFirstGuess;
		TemporalSeriesSeed    mGoodTick;
		TemporalSeriesSeed    mGoodTempo;

	};

} // namespace CLAM


#endif // TickSequenceTracker.hxx
