#ifndef __TICKSEQUENCETRACKER__
#define __TICKSEQUENCETRACKER__

#include "ProcessingComposite.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"
#include "TickSequenceTrackerConfig.hxx"
#include "AudioPeakDetect.hxx"
#include "Pulse.hxx"
#include "TimeDifference.hxx"
#include "TemporalSeriesFinder.hxx"

namespace CLAM
{

	class Audio;
	class GlobalPulse;
	class GridGen;

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
		bool Do(const Array<TimeIndex>& transients, 
			Pulse& tickSequence,
			Pulse& beatSequence,
			Audio& IOIHist );


		void SetVisualizationAudio(Audio& audio);

		TData CompGlobPulse(GlobalPulse& gpulse, const int pulseLimSup, 
				    const Array<TData> &forGlobalPulseCalc);

		void StorePulseIndexes(const int nLoops, const Array<TimeIndex>& pulsesArray,
				       Array<TimeIndex>& mPulses);

		void GeneratePulseGrid(const TData start, const TData gap, const TData end, 
				       GridGen& pulseGridGen, Array<TimeIndex>& pulseArray);

		void VisualizeGrid(Audio& IOIHist, TIndex index, Array<TimeIndex>& pulseArray,
				   TData samplingRate);

		void ComputeOnsets(Audio& readAudio, Array<TimeIndex>& transients, 
				   TData samplingRate);


	protected:
		bool Compute(const Array<TimeIndex>& transients, Audio& IOIHist,
			     Array<TimeIndex>& ticks,Array<TimeIndex>& beats,TData& globalTick,
			     TData& globalTempo);

	private: // children Processing

		AudioPeakDetect       mAudioPeakDetector;
		TimeDifference        mTemporalDiff;
		TemporalSeriesFinder  mTimeSeriesFinder;
		TemporalSeriesFinderConfig mTSFConfig;

	};

} // namespace CLAM


#endif // TickSequenceTracker.hxx
