#include "SinTracksViewAspect.hxx"
#include "SinTracksView.hxx"

namespace CLAMGUI
{
		SinTracksViewAspect::SinTracksViewAspect( SinTracksView& view )
				: mView( view )
		{
		}

		SinTracksViewAspect::~SinTracksViewAspect()
		{
		}

		void SinTracksViewAspect::ForceViewRefresh()
		{
				mView.Refresh();
		}
		
}
