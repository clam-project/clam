/*
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

#ifndef _SMSBase_
#define _SMSBase_

#include <string>
#include <vector>

#include "SerializationController.hxx"

#include "SMSSynthesisConfig.hxx"
#include "SMSAnalysisConfig.hxx"
#include "SMSAnalysisSynthesisConfig.hxx"
#include "Melody.hxx"
#include "SMSTransformationChain.hxx"
#include "SegmentDescriptors.hxx"
#include "Audio.hxx"
#include "Progress.hxx"
#include "WaitMessage.hxx"

namespace CLAM
{
	class DataState{
		std::vector<bool> mStateArray;
		enum{ mNumStates = 9 };
	public:
		enum States {
			/** Indicates whether there is a valid analysis-synthesis configuration */
			HaveConfig = 0,
			/** Indicates whether an analysis has been performed */
			HaveAnalysis,
			/** Indicates whether there is a valid input audio */
			HaveAudioIn,
			/** Indicates whether there is a valid audio to morph*/
			HaveAudioMorph,
			/** Indicates whether there is a valid output audio */
			HaveAudioOut,
			/** Indicates whether there is a valid analyzed melody */
			HaveMelody,
			/** Indicates whether there is a valid transformation score */
			HaveTransformationScore,
			/** Indicates whether there is a valid spectrum, needed for melody anlysis */
			HaveSpectrum,
			/** Indicates whether there a transformation has been performed */
			HaveTransformation
		};

		DataState( ) : mStateArray( mNumStates, false ) { }
		virtual ~DataState( ) { }
		inline bool Query( enum States query ) { return mStateArray[query]; }
		inline void Reached( enum States query ) { mStateArray[query] = true; }
		inline void Abandoned( enum States query ) { mStateArray[query] = false; }
	};


	/** This is the base class for the Analysis Synthesis example. It implements
	* all the necessary processing but it cannot be instantiated. To instantiate
	* a particular analysis synthesis application you need to work directly with 
	* the GUI or stdio versions
	* @see SMSTools.hxx
	* @see SMS.hxx 
	* This class performs an SMS analysis on the input sound. Necessary inputs are
	* a configuration xml file and, in case you want to perform a transformation,
	* a transformation xml score */	
	class SMSBase
	{
	public:
		SMSBase(void);	
		virtual ~SMSBase(void);
		void Run(void);
		inline void SetHaveConfig(bool hasConfig) {
			hasConfig ?
				mDataState.Reached( DataState::HaveConfig )
				: mDataState.Abandoned( DataState::HaveConfig );
		}
		
	protected:
		
	
		/** Cleans up segment from pre-existing data*/ 
		void Flush(Segment& seg);
		/** Copies all content in src segment except Audio */
		void CopySegmentExceptAudio(const Segment& src, Segment& dest);
		/** Initialize analysis and synthesis configuration from loaded
		* global configuration */
		void InitConfigs(void);
		/** Load global configuration */
		void LoadConfig(const std::string& inputFileName);
		/** Store global configuration */
		void StoreConfig(const std::string& inputFileName);
		/** Load transformation score */
		void LoadTransformationScore(const std::string& inputFileName);

		bool LoadAnalysis(const char* filename);
		bool LoadAnalysis(const std::string& filename){return LoadAnalysis(filename.c_str());}

		/** Store data resulting from analysis. Some unnecessary data
		* is removed from memory */
		void StoreAnalysis(const char* filename);
		void StoreAnalysis(const std::string& filename) {StoreAnalysis(filename.c_str());}

		/** Store data resulting from transformation. Some unnecessary data
		* is removed from memory */
		void StoreTransformation(const char* filename);
		void StoreTransformation(const std::string& filename) {StoreTransformation(filename.c_str());}

		/** Store synthesized sound */
		void StoreOutputSound( const char* fileName );
		/** Store synthesized sound, only residual component */
		void StoreOutputSoundResidual( const char* fileName );
		/** Store synthesized sound, only sinusoidal component */
		void StoreOutputSoundSinusoidal( const char* fileName );
		/** Load input sound */
		bool LoadInputSound( );
		bool LoadInputSound( const char* fileName );
		/** Load sound to morph*/
		bool LoadMorphSound(void);

		/** This method should be overridden on subclasses to provide
		further control on how the concrete process is performed */

		virtual void DoAnalysis();
		virtual void DoMorphAnalysis();

		void AnalysisProcessing();
		void MorphAnalysisProcessing();
		void SetSMSMorphFileName();

		/** This method should be overridden on subclasses to provide
		further control on how the concrete process is performed */

		virtual void DoTracksCleanup();
		virtual void DoMorphTracksCleanup();

		void TracksCleanupProcessing();
		void MorphTracksCleanupProcessing();

		/** Perform analysis. Requires a valid configuration file to be loaded */
		void Analyze(void);
		/** Perform synthesis. Requires a valid configuration file to be loaded 
		* and the analysis to be performed. */
		void SynthesisProcessing();

		/** This method should be overridden on subclasses to provide
		further control on how the concrete process is performed */

		virtual void DoSynthesis();

		void Synthesize(void);
		/** Perform transformation according to previously set transformation 
		* (PitchScale by default). Requires a valid transformation score to be loaded */
		virtual void DoTransformation();
		void Transform(void);
		void TransformProcessing();

		/** Analyze and extract melody. This feature only works on some sort of instruments
		* for monophonic phrases */
		void AnalyzeMelody(void);

		void ComputeLowLevelDescriptors();

		/** Stores previously analyzed melody into xml format */
		void StoreMelody( const char* fileName );

		/** This member is in charge of all the serialization movements in the application */
		SerializationController mSerialization;

		/** This member knows the application's data state*/
		DataState mDataState;

		/** Output audio */
		Audio mAudioOut;
		/** Output audio, only sinusoidal component */
		Audio mAudioOutSin;
		/** Output audio, only residual component */
		Audio mAudioOutRes;
		
		/** Internal structure used for analysis and synthesis, contains all.
		* @see Segment.hxx */
		Segment mOriginalSegment;
		Segment mTransformedSegment;

		Segment mMorphSegment;

		SegmentDescriptors mOriginalSegmentDescriptors;
		SegmentDescriptors mTransformedSegmentDescriptors;
		
		/** Analyzed melody */
		Melody mMelody;

		/** Analysis configuration */
		SMSAnalysisConfig mAnalConfig;
		/** Synthesis configuration */
		SMSSynthesisConfig mSynthConfig;
		/** Global configuration, loaded from an xml file and used to initialize
		* analysis and synthesis configurations 
		* @see InitConfigs */
		SMSAnalysisSynthesisConfig mGlobalConfig;
		/** Transformation score loaded from an xml file */
		SMSTransformationChainConfig mTransformationScore;

		/** Actual transformation to be used*/
		SMSTransformationChain mTransformation;

		CLAMGUI::Progress* mCurrentProgressIndicator;
		CLAMGUI::WaitMessage* mCurrentWaitMessage;

		/** Creates progress bar. Implemented both in GUI and stdio versions */
		virtual CLAMGUI::Progress* CreateProgress(const char* title,float from,float to) = 0;
		/** Creates a wait message. Implemented both in GUI and stdio versions */
		virtual CLAMGUI::WaitMessage* CreateWaitMessage(const char* title) = 0;

		void DestroyProgressIndicator();

		void DestroyWaitMessage();

		inline bool HasToDoTracksCleaning() const
		{
			return mGlobalConfig.GetDoCleanTracks();
		}

		/** sets and synchronizes sampling rate of internal objects and configurations*/
		void SetSamplingRate(TSize samplingRate);

		/** sampling rate for the whole application */
		TSize mSamplingRate;


	};

};

#endif
