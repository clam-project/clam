#include "AudioAspect.hxx"
#include "AudioView.hxx"

namespace CLAMGUI
{
		AudioAspect::AudioAspect( AudioAdapter& view )
				: mAdapter( view )
		{
		}

		AudioAspect::~AudioAspect()
		{
				
		}

		void AudioAspect::ForceViewRefresh()
		{
				mAdapter.Publish();
		}

}
