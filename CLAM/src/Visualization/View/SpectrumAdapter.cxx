#include "SpectrumAdapter.hxx"
#include "Spectrum.hxx"

namespace CLAMGUI
{
		using CLAM::TSize;

		SpectrumAdapter::SpectrumAdapter()
				: mAspect( *this ), mObserved( NULL )
		{
		}

		SpectrumAdapter::~SpectrumAdapter()
		{
		}

		bool SpectrumAdapter::BindTo( const ProcessingData* specObj )
		{
				mObserved = dynamic_cast< const Spectrum* >( specObj );

				if ( !mObserved ) return false;

				return true;
		}

}
