#ifndef __ONSET_DETECTOR__
#define __ONSET_DETECTOR__

#include "ProcessingComposite.hxx"
#include "Segment.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"
#include "GTFilterbank.hxx"
#include "Normalization.hxx"
#include "OnsetDetectorConfig.hxx"

namespace CLAM
{

	/** OnsetDetector class **/
	class OnsetDetector : public ProcessingComposite 
	{
		
	public:
	
		OnsetDetector();
		OnsetDetector(OnsetDetectorConfig& c);
		//~OnsetDetector();

		const ProcessingConfig &GetConfig() const {return mConfig;}

		/** Supervised mode execution */
		bool Do(void){return false;}

		/** Unsupervised mode execution */
		bool Do(Segment& originalSegment); //inplace processing only?
		bool Do(Segment& originalSegment, Array<TimeIndex>& out);  //in order to also keep onset weights

		Array<TimeIndex>& GetOnsets() { return mOnsets;}

		const char *GetClassName() const {return "OnsetDetector";}

	private:
	
		// Configuration data
		OnsetDetectorConfig mConfig;
	
		Normalization	mNorm;
		GTFilterbank	mFilterBank;

	
		// Internal convenience methods.
		void AttachChildren();
		bool ConfigureChildren();

		// Processing Object compliance methods.

		/* Configuration method */
		bool ConcreteConfigure(const ProcessingConfig&);

	
		bool CalcFilterBankOutput( Audio &in );
		//void Algorithm(Segment& s, Array< Array<TData> >& values);
		void Algorithm(Segment& s, Array< Array<TData> >& values, Array<TimeIndex>& out, bool weightsOut);
		void Smoothing(DataArray energy, DataArray &smoothedEnergy );
		void DetectPosition(DataArray in, DataArray &ret);
		void DetectCandidates(DataArray in, DataArray weight, TData threshold , Array<TimeIndex> &ret);
		void DeleteWeakOnsets(Array<TimeIndex> &in , int type );
		void PeakDeletion( Array<TimeIndex> in , TimeIndex &ret );
		void PeakSummation( Array<TimeIndex> in , TimeIndex &ret );
		void CheckOffset( Segment &s , Array<TimeIndex> finalOnsets);

		//Member Data
		Audio mAudio;
		TSize mnSamples;						//Number of samples in the segment (after down sampling)	
		TSize mnBands;							//Number of bands in the filter bank
		TSize mLowFreq;							//Lowest central frequency in the filter bank
		TSize mHighFreq;						//Highest central frequency in the filter bank
		TData mNoiseThreshold;
		TSize mWinSize;							//Smoothing Filter Size
		TData mBandThreshold;					//Threshold for Band peak picking 
		TData mMinPeakDist;						//Minimum distance between picked peaks
		TData mGlobalThreshold;					
		DataArray mWinCoef;						//Smoothing Filter Coefficients
		TData mSampleRate;						//Sample Rate after down sampling
		TData mOffsetThreshold;
		bool mComputeOffsets;
		Array< Array<TData> > mFilterBankOutput;
		Array<TimeIndex> mOnsets;
		DataArray cf;
	};


	
}


#endif // OnsetDetector.hxx
