#include "SpectralPeakArrayView.hxx"
#include "SpectralPeakArray.hxx"
#include "Assert.hxx"
#include "GlobalEnums.hxx"
#include <cmath>

namespace CLAMGUI
{
		using CLAM::EScale;
		using CLAM::DataArray;
		using CLAM::TSize;

		SpectralPeakArrayView::SpectralPeakArrayView()
				: mAspect( *this ), mObserved( NULL )
		{
		}

		SpectralPeakArrayView::~SpectralPeakArrayView()
		{
		}

		bool SpectralPeakArrayView::BindTo( const ProcessingData* obj )
		{
				mObserved = dynamic_cast< const SpectralPeakArray* >( obj );

				if ( !mObserved ) return false;

				return true;
		}

		bool SpectralPeakArrayView::Refresh()
		{
				if ( !mObserved )
						return false;
				
				TranscribePeakArray();

				mAspect.AcquirePartials.Emit( mPartialBuffer );

				return true;
		}

		void SpectralPeakArrayView::TranscribePeakArray()
		{
				TSize nPeaks = mObserved->GetMagBuffer().Size();
				
				CLAM_DEBUG_ASSERT( nPeaks==mObserved->GetFreqBuffer().Size() &&
								   nPeaks==mObserved->GetPhaseBuffer().Size(),
								   "Inconsistency detected at SpectralPeakArray: buffers differ in size" );

				const DataArray& magBuff = mObserved->GetMagBuffer();
				const DataArray& freqBuff = mObserved->GetFreqBuffer();
				const DataArray& phaseBuff = mObserved->GetPhaseBuffer();

				mPartialBuffer.Resize( nPeaks );
				mPartialBuffer.SetSize( nPeaks );

				for ( int i = 0; i < nPeaks; i++ )
				{
						mPartialBuffer[i].mMag = magBuff[i];
						mPartialBuffer[i].mFreq = freqBuff[i];
						mPartialBuffer[i].mPhase = phaseBuff[i];
				}
				
				if ( mObserved->GetScale() == EScale::eLinear )
				{
						for ( int i=0; i < nPeaks; i++ )
								mPartialBuffer[i].mMag = 20.0f*log10( mPartialBuffer[i].mMag );
				}
		}
}
