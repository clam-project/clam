#include "AudioAdapter.hxx"
#include "Audio.hxx"

namespace CLAMVM
{
		using CLAM::TSize;
		
		
		AudioAdapter::AudioAdapter()
				: mAspect( *this ), mObserved( NULL )
		{
		}

		AudioAdapter::~AudioAdapter()
		{
		}

		bool AudioAdapter::BindTo( const ProcessingData* audioObj ) 
		{
				mObserved = dynamic_cast< const Audio* >( audioObj );
			
				if ( !mObserved ) 
						return false;

				return true;
		}

		bool AudioAdapter::Publish()
		{
				if ( !mObserved )  // there is no object being observed
						return false;

				TData obsSR = 1.0f / mObserved->GetSampleRate();
				TSize nSamples = mObserved->GetBuffer().Size();

				mAspect.AcquireSamples.Emit( mObserved->GetBuffer() );
				mAspect.AcquireDuration.Emit( nSamples * obsSR );
				mAspect.AcquireStartTime.Emit( mObserved->GetBeginTime() );
				mAspect.AcquireSampleRate.Emit( mObserved->GetSampleRate() );
				mAspect.AcquireAudio.Emit( mObserved->GetBuffer(), mObserved->GetBeginTime(), nSamples * obsSR, mObserved->GetSampleRate()   ); 

				return true;
		}
}
