#include "StdioSpectralPeakArrayPresentation.hxx"
#include "SpectralPeakArrayAspect.hxx"
#include <algorithm>
#include <iostream>


namespace CLAMVM
{
		StdioSpectralPeakArrayPresentation::StdioSpectralPeakArrayPresentation()
		{
				SetPartials.Wrap( this, &StdioSpectralPeakArrayPresentation::OnNewPartials );
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
				SpectralPeakArrayAspect& viewAspect = dynamic_cast< SpectralPeakArrayAspect& >( a );

				viewAspect.AcquirePartials.Connect( SetPartials );
		}

		void StdioSpectralPeakArrayPresentation::OnNewPartials( const Array<Partial>& array )
		{
				mPartialsToDraw.Resize( array.Size() );
				mPartialsToDraw.SetSize( array.Size() );

				std::copy( array.GetPtr(), array.GetPtr()+array.Size(), mPartialsToDraw.GetPtr() );
		}
}
