#include "SpectrumPresentation.hxx"
#include "SpectrumAspect.hxx"

namespace CLAMGUI
{
		SpectrumPresentation::SpectrumPresentation()
		{
				SetSpectrum.Wrap( this, &SpectrumPresentation::OnNewSpectrum );
		}

		SpectrumPresentation::~SpectrumPresentation()
		{
		}

		void SpectrumPresentation::Bind( Aspect& a ) throw( std::bad_cast )
		{
				SpectrumAspect& asp = dynamic_cast<SpectrumAspect& >( a );
				
				asp.AcquireSpectrum.Connect( SetSpectrum );
		}
}
