#include "SpectrumViewAspect.hxx"
#include "SpectrumView.hxx"

namespace CLAMGUI
{
		SpectrumViewAspect::SpectrumViewAspect( SpectrumView& view )
				: mView( view )
		{
		}

		SpectrumViewAspect::~SpectrumViewAspect()
		{
		}

		void SpectrumViewAspect::ForceViewRefresh()
		{
				mView.Refresh();
		}

}
