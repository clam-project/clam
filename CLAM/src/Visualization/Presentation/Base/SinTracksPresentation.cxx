#include "SinTracksPresentation.hxx"
#include "SinTracksAspect.hxx"

namespace CLAMVM
{
		SinTracksPresentation::SinTracksPresentation()
		{
				AddPartials.Wrap( this, &SinTracksPresentation::OnNewPartials );
				SetSpectralRange.Wrap( this, &SinTracksPresentation::OnNewRange );
				SetTimeInterval.Wrap( this, &SinTracksPresentation::OnNewDuration );
		}

		SinTracksPresentation::~SinTracksPresentation()
		{
		}

		void SinTracksPresentation::Bind( Aspect& givenAspect ) throw ( std::bad_cast )
		{
				SinTracksAspect& concrete = dynamic_cast< SinTracksAspect& >( givenAspect );

				concrete.AcquirePartials.Connect( AddPartials );
				concrete.AcquireSpectralRange.Connect( SetSpectralRange );
				concrete.AcquireDuration.Connect( SetTimeInterval );
		}
}
