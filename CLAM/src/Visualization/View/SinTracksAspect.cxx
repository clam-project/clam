#include "SinTracksAspect.hxx"
#include "SinTracksAdapter.hxx"

namespace CLAMGUI
{
		SinTracksAspect::SinTracksAspect( SinTracksAdapter& view )
				: mAdapter( view )
		{
		}

		SinTracksAspect::~SinTracksAspect()
		{
		}

		void SinTracksAspect::ForceViewRefresh()
		{
				mAdapter.Publish();
		}
		
}
