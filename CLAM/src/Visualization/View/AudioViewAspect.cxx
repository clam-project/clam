#include "AudioViewAspect.hxx"
#include "AudioView.hxx"

namespace CLAMGUI
{
		AudioViewAspect::AudioViewAspect( AudioView& view )
				: mView( view )
		{
		}

		AudioViewAspect::~AudioViewAspect()
		{
				
		}

		void AudioViewAspect::ForceViewRefresh()
		{
				mView.Refresh();
		}

}
