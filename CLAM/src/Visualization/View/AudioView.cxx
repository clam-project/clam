#include "AudioView.hxx"
#include "Audio.hxx"

namespace CLAMGUI
{
		using CLAM::TSize;
		
		
		AudioView::AudioView()
				: mAspect( *this ), mObserved( NULL )
		{
		}

		AudioView::~AudioView()
		{
		}

		bool AudioView::BindTo( const ProcessingData* audioObj ) 
		{
				mObserved = dynamic_cast< const Audio* >( audioObj );
			
				if ( !mObserved ) 
						return false;

				return true;
		}

		bool AudioView::Refresh()
		{
				if ( !mObserved )  // there is no object being observed
						return false;

				TData obsSR = 1.0f / mObserved->GetSampleRate();
				TSize nSamples = mObserved->GetBuffer().Size();

				mAspect.AcquireSamples.Emit( mObserved->GetBuffer() );
				mAspect.AcquireDuration.Emit( nSamples * obsSR );
				mAspect.AcquireStartTime.Emit( mObserved->GetBeginTime() );
				mAspect.AcquireSampleRate.Emit( obsSR );

				return true;
		}
}
