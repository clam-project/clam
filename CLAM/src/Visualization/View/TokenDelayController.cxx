#include "TokenDelayController.hxx"
#include "Spectrum.hxx"
#include "TokenDelay.hxx"

namespace CLAMVM
{
		SpectralTokenDelayController::SpectralTokenDelayController()
		{
		}

		SpectralTokenDelayController::~SpectralTokenDelayController()
		{
		}

		bool SpectralTokenDelayController::BindTo( Processing& proc )
		{
				ProcessingController::Publish();

				mObserved = dynamic_cast<TokenDelay<Spectrum>* >( &proc );

				CLAM_ASSERT( mObserved!=NULL, "Error: Trying to bind to a Processing that is not a TokenDelay<Spectrum>" );

				return true;
		}

		bool SpectralTokenDelayController::Publish()
		{
				ProcessingController::Publish();

				ControlAdapter* adap = static_cast<ControlAdapter*> (mChildren["Delay Control"]);
				
				const TokenDelayConfig& cfg = static_cast< const TokenDelayConfig& >( mObserved->GetConfig() );

				adap->SetValueRange( 0, max );
				
				return true;
		}
}
