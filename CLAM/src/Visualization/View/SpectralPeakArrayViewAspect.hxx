#ifndef __SPECTRALPEAKARRAYVIEWASPECT__
#define __SPECTRALPEAKARRAYVIEWASPECT__

#include "Aspect.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Signalv1.hxx"

namespace CLAMGUI
{
	
	using CLAM::Array;

		class SpectralPeakArrayView;

		class SpectralPeakArrayViewAspect : public Aspect
		{
		public:
				
				friend class SpectralPeakArrayView;

				SpectralPeakArrayViewAspect( SpectralPeakArrayView& );
				
				virtual ~SpectralPeakArrayViewAspect();

				virtual void ForceViewRefresh();

				// Signals
				Signalv1< const Array<Partial>& > AcquirePartials;

		private:
				SpectralPeakArrayView&       mView;
		};

}

#endif // SpectralPeakArrayViewAspect.hxx
