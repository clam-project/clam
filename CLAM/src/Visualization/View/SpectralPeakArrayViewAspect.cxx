#include "SpectralPeakArrayViewAspect.hxx"
#include "SpectralPeakArrayView.hxx"

namespace CLAMGUI
{
		SpectralPeakArrayViewAspect::SpectralPeakArrayViewAspect( SpectralPeakArrayView& view )
				: mView( view )
		{
		}

		SpectralPeakArrayViewAspect::~SpectralPeakArrayViewAspect()
		{
		}

		void SpectralPeakArrayViewAspect::ForceViewRefresh()
		{
				mView.Refresh();
		}
		
}
