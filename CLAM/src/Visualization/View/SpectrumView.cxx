#include "SpectrumView.hxx"
#include "Spectrum.hxx"

namespace CLAMGUI
{
		using CLAM::TSize;

		SpectrumView::SpectrumView()
				: mAspect( *this ), mObserved( NULL )
		{
		}

		SpectrumView::~SpectrumView()
		{
		}

		bool SpectrumView::BindTo( const ProcessingData* specObj )
		{
				mObserved = dynamic_cast< const Spectrum* >( specObj );

				if ( !mObserved ) return false;

				return true;
		}

}
