#ifndef __ONSET_DETECTOR__
#define __ONSET_DETECTOR__

#include "ProcessingComposite.hxx"
#include "Segment.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"
#include "GT_FilterBank.hxx"
#include "Normalization.hxx"
#include "OnsetDetectorConfig.hxx"
#include "OD_AudioDecimation.hxx"
#include <list>
#include <map>

namespace CLAM
{

	/** OnsetDetector class **/
	class OnsetDetector : public ProcessingComposite 
	{
		typedef std::list<TimeIndex>::iterator LI;
		typedef std::map< TIndex, double>::iterator MI;
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
		GT_FilterBank	mFilterBank;
		OnsetDetection::AudioDecimator  mDecimator;

	
		// Internal convenience methods.
		void AttachChildren();
		bool ConfigureChildren();

		// Processing Object compliance methods.

		/* Configuration method */
		bool ConcreteConfigure(const ProcessingConfig&);

	
		bool CalcFilterBankOutput(  );
		void Algorithm(Segment& s, Array<TimeIndex>& out );
		void Smoothing(Array<double>& energy, Array<double> &smoothedEnergy );
		void DetectPosition(Array<double>& in, Array<double> &ret);
		void DetectCandidates(Array<double>& in, Array<double>& weight, TData threshold , Array<TimeIndex> &ret);
		void DetectCandidates( Array<double>& in, Array<double>& weight,
				       TData threshold, std::list<TimeIndex>& candidates );
		void DeleteWeakOnsets(Array<TimeIndex> &in , int type );

		void RemoveTooNearOnsetsFromCandidatesList( std::list<TimeIndex>& in );
		void PeakDeletion( LI first, LI last, TimeIndex& newPeak );

		void RemoveTooWeakOnsetsFromCandidatesList( std::list<TimeIndex>& in );
		void PeakSummation( LI first, LI last, TimeIndex& newPeak );

		void PeakDeletion( Array<TimeIndex>& in , TimeIndex &ret );
		void PeakSummation( Array<TimeIndex>& in , TimeIndex &ret );
		
		void CheckOffset( Segment &s , Array<TimeIndex>& finalOnsets);
		void CheckOffset( Segment& s, std::list<TimeIndex>& candidates );

		void DecimationForEnvelopeComputation( Array<double>& envelope );
		void ComputeSmoothingFilterCoeffs( Array<double>& coeffs );
		void ExtractAudioEnvelope( Array<double>& envelope );

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

		TData mSampleRate;						//Sample Rate after down sampling
		TData mOffsetThreshold;
		bool mComputeOffsets;

		Array<double> mWinCoef;						//Smoothing Filter Coefficients
		//Rescaling factor
		double        mWinCoefSummation;
		Array< Array<double> > mFilterBankOutputs;
		Array<TimeIndex> mOnsets;
		Array<double>    mRevSmoothedEnergy;
	};


	
}


#endif // OnsetDetector.hxx
