#include "SpectralPeaksPresentation.hxx"
#include "SpectrumAspect.hxx"
#include "SpectralPeakArrayAspect.hxx"

namespace CLAMVM
{

		SpectralPeaksPresentation::SpectralPeaksPresentation()
		{
				SetSpectrum.Wrap( this, &SpectralPeaksPresentation::OnNewSpectrum );
				SetPartials.Wrap( this, &SpectralPeaksPresentation::OnNewPeakArray );
		}

		SpectralPeaksPresentation::~SpectralPeaksPresentation()
		{
		}

		void SpectralPeaksPresentation::Bind( Aspect& givenAspect ) throw(  )
		{
				Aspect* a = &givenAspect;

				SpectrumAspect* sa = NULL;
				SpectralPeakArrayAspect* spa = NULL;

				sa = dynamic_cast< SpectrumAspect* >( a );
				if ( sa != NULL )
				{
						sa->AcquireSpectrum.Connect( SetSpectrum );
						return;
				}
				spa = dynamic_cast< SpectralPeakArrayAspect* >( a );
				if ( spa != NULL )
				{
						spa->AcquirePartials.Connect( SetPartials );
						return;
				}
				CLAM_ASSERT( false, "Unwanted aspect provided" );
		}

}



