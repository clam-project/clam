#include "StdioAudioPresentation.hxx"
#include "AudioViewAspect.hxx"
#include <algorithm>
#include <iostream>

namespace CLAMGUI
{
		StdioAudioPresentation::StdioAudioPresentation()
				: mMaximum( 0 ), mMinimum( 0 ), mAudioLen( 0 ),
				  mAudioStart( 0 ), mSampleRate( 0 )
		{
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
				AudioViewAspect& viewAspect = dynamic_cast<AudioViewAspect& >( a );
				
				viewAspect.AcquireSamples.Connect( this, &StdioAudioPresentation::HandleIncomingBuffer, mBufferSlot );
				viewAspect.AcquireDuration.Connect( this, &StdioAudioPresentation::HandleIncomingDuration, mLenSlot );
				viewAspect.AcquireStartTime.Connect( this, &StdioAudioPresentation::HandleIncomingStartTime, mStartSlot );
				viewAspect.AcquireSampleRate.Connect( this, &StdioAudioPresentation::HandleIncomingSampleRate, mRateSlot );
		}

		void StdioAudioPresentation::HandleIncomingBuffer( const DataArray& array )
		{
				mMaximum = *std::max_element( array.GetPtr(), array.GetPtr()+array.Size() );
				mMinimum = *std::min_element( array.GetPtr(), array.GetPtr()+array.Size() );
				
		}

		void StdioAudioPresentation::HandleIncomingDuration( TTime secs )
		{
				mAudioLen = secs;
		}

		void StdioAudioPresentation::HandleIncomingStartTime( TTime secs )
		{
				mAudioStart = secs;
		}

		void StdioAudioPresentation::HandleIncomingSampleRate( TData rate )
		{
				mSampleRate = rate;
		}
}
