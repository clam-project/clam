#include "StdioSpectrumPresentation.hxx"
#include "SpectrumViewAspect.hxx"
#include <algorithm>
#include <iostream>

namespace CLAMGUI
{
		StdioSpectrumPresentation::StdioSpectrumPresentation()
				: mSpectralRange( 22050 )
		{
				
		}

		StdioSpectrumPresentation::~StdioSpectrumPresentation()
		{
		}

		void StdioSpectrumPresentation::Show()
		{
				std::cout << "DATA RETRIEVED:" << std::endl;
				
				std::cout << "Highest magnitude value (dB): " << *std::max_element(mBinsMagnitude.GetPtr(),mBinsMagnitude.GetPtr()+mBinsMagnitude.Size() ) << std::endl;
				std::cout << "Lowest magnitude value (dB): " << *std::min_element(mBinsMagnitude.GetPtr(),mBinsMagnitude.GetPtr()+mBinsMagnitude.Size() ) << std::endl;

				std::cout << "Highest phase angle ( radians ):" << *std::max_element( mBinsPhase.GetPtr(),mBinsPhase.GetPtr()+mBinsPhase.Size() ) << std::endl;				
				std::cout << "Lowest phase angle ( radians ):"  << *std::max_element( mBinsPhase.GetPtr(),mBinsPhase.GetPtr()+mBinsPhase.Size() ) << std::endl;

				std::cout << "Spectral range :" << mSpectralRange << std::endl;
 		}

		void StdioSpectrumPresentation::Bind( Aspect& a ) throw ( std::bad_cast )
		{
				SpectrumViewAspect& viewAspect = dynamic_cast< SpectrumViewAspect& >( a );
				
				viewAspect.AcquireMagnitude.Connect( this, &StdioSpectrumPresentation::HandleIncomingMagBins, mBinsMagSlot );
				viewAspect.AcquirePhase.Connect( this, &StdioSpectrumPresentation::HandleIncomingPhaseBins, mBinsPhaseSlot );
				viewAspect.AcquireSpectralRange.Connect( this, &StdioSpectrumPresentation::HandleIncomingSpecRng, mBinsSpecRngSlot );
		}

		void StdioSpectrumPresentation::HandleIncomingMagBins( const DataArray& array )
		{
				mBinsMagnitude.Resize( array.Size() );
				mBinsMagnitude.SetSize( array.Size() );

				std::copy( array.GetPtr(), array.GetPtr() + array.Size(), mBinsMagnitude.GetPtr() );
		}

		void StdioSpectrumPresentation::HandleIncomingPhaseBins( const DataArray& array )
		{
				mBinsPhase.Resize( array.Size() );
				mBinsPhase.SetSize( array.Size() );

				std::copy( array.GetPtr(), array.GetPtr() + array.Size(), mBinsPhase.GetPtr() );
				
		}
		
		void StdioSpectrumPresentation::HandleIncomingSpecRng( TData specRange )
		{
				mSpectralRange = specRange;
		}

}
