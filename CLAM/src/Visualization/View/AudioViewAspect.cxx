#include "AudioViewAspect.hxx"
#include "AudioView.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"

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
