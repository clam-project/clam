#include "AudioAspect.hxx"
#include "AudioAdapter.hxx"

namespace CLAMVM
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
