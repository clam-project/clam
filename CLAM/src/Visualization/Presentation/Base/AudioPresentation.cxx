#include "AudioPresentation.hxx"
#include "AudioAspect.hxx"

namespace CLAMVM
{
		AudioPresentation::AudioPresentation()
		{
				SetAudio.Wrap( this, &AudioPresentation::OnNewAudio );
		}

		AudioPresentation::~AudioPresentation()
		{
		}

		void AudioPresentation::Bind( Aspect& a ) throw ( std::bad_cast )
		{
			AudioAspect& asp = dynamic_cast<AudioAspect& >( a );

			asp.AcquireAudio.Connect( SetAudio );
		}

}
