#include "StdioAudioPresentation.hxx"
#include "AudioAspect.hxx"
#include <algorithm>
#include <iostream>

namespace CLAMVM
{
		StdioAudioPresentation::StdioAudioPresentation()
				: mMaximum( 0 ), mMinimum( 0 ), mAudioLen( 0 ),
				  mAudioStart( 0 ), mSampleRate( 0 )
		{
				SetSamples.Wrap( this, &StdioAudioPresentation::OnNewSamples );
				SetDuration.Wrap( this, &StdioAudioPresentation::OnNewDuration );
				SetStartTime.Wrap( this, &StdioAudioPresentation::OnNewStartTime );
				SetSampleRate.Wrap( this, &StdioAudioPresentation::OnNewSampleRate );
		}
		
		StdioAudioPresentation::~StdioAudioPresentation()
		{
		}

		void StdioAudioPresentation::Show()
		{
				std::cout << "DATA RETRIEVED:" << std::endl;
				std::cout << "Maximum sample value: " << mMaximum << std::endl;
				std::cout << "Minimum sample value: " << mMinimum << std::endl;
				std::cout << "Audio length: " << mAudioLen << std::endl;
				std::cout << "Audio start time: " << mAudioStart << std::endl;
				std::cout << "Sample rate: " << mSampleRate << std::endl;
				std::cout << "END OF DATA RETRIEVED SO FAR" << std::endl;
		}

		void StdioAudioPresentation::Bind( Aspect& a ) throw ( std::bad_cast )
		{
				// Here we go!
				AudioAspect& aspect = dynamic_cast<AudioAspect& >( a );
				
				aspect.AcquireSamples.Connect( SetSamples );
				aspect.AcquireDuration.Connect( SetDuration );
				aspect.AcquireStartTime.Connect( SetStartTime );
				aspect.AcquireSampleRate.Connect( SetSampleRate );
		}

		void StdioAudioPresentation::OnNewSamples( const DataArray& array )
		{
				mMaximum = *std::max_element( array.GetPtr(), array.GetPtr()+array.Size() );
				mMinimum = *std::min_element( array.GetPtr(), array.GetPtr()+array.Size() );
				
		}

		void StdioAudioPresentation::OnNewDuration( TTime secs )
		{
				mAudioLen = secs;
		}

		void StdioAudioPresentation::OnNewStartTime( TTime secs )
		{
				mAudioStart = secs;
		}

		void StdioAudioPresentation::OnNewSampleRate( TData rate )
		{
				mSampleRate = rate;
		}
}
