#include "StdioSpectralPeakArrayPresentation.hxx"
#include "SpectralPeakArrayViewAspect.hxx"
#include <algorithm>
#include <iostream>


namespace CLAMGUI
{
		StdioSpectralPeakArrayPresentation::StdioSpectralPeakArrayPresentation()
		{
		}
		
		StdioSpectralPeakArrayPresentation::~StdioSpectralPeakArrayPresentation()
		{
		}

		void StdioSpectralPeakArrayPresentation::Show()
		{
				std::cout << "DATA RETRIEVED: " << std::endl;
				
				for ( int i = 0; i < mPartialsToDraw.Size(); i++ )
				{
						std::cout << "Peak #" << i+1 << std::endl;
						std::cout << "Magnitude (dB): " << mPartialsToDraw[i].mMag << std::endl;
						std::cout << "Frequency (Hz): " << mPartialsToDraw[i].mFreq << std::endl;
						std::cout << "Phase    (rad):" << mPartialsToDraw[i].mPhase << std::endl;
				}
		}

		void StdioSpectralPeakArrayPresentation::Bind( Aspect& a) throw( std::bad_cast )
		{
				SpectralPeakArrayViewAspect& viewAspect = dynamic_cast< SpectralPeakArrayViewAspect& >( a );

				viewAspect.AcquirePartials.Connect( this, &StdioSpectralPeakArrayPresentation::HandleIncomingPartials, mPartialsSlot );
		}

		void StdioSpectralPeakArrayPresentation::HandleIncomingPartials( const Array<Partial>& array )
		{
				mPartialsToDraw.Resize( array.Size() );
				mPartialsToDraw.SetSize( array.Size() );

				std::copy( array.GetPtr(), array.GetPtr()+array.Size(), mPartialsToDraw.GetPtr() );
		}
}
