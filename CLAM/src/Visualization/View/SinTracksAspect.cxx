#include "SinTracksAspect.hxx"
#include "SinTracksAdapter.hxx"

namespace CLAMVM
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
