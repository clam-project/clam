#ifndef __SPECTRALPEAKARRAYASPECT__
#define __SPECTRALPEAKARRAYASPECT__

#include "Aspect.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Signalv1.hxx"

namespace CLAMGUI
{
	
	using CLAM::Array;

		class SpectralPeakArrayAdapter;

		class SpectralPeakArrayAspect : public Aspect
		{
		public:
			   
				SpectralPeakArrayAspect( SpectralPeakArrayAdapter& );
				
				virtual ~SpectralPeakArrayAspect();

				virtual void ForceViewRefresh();

				// Signals
				Signalv1< const Array<Partial>& >       AcquirePartials;

		private:
				SpectralPeakArrayAdapter&       mAdapter;
		};

}

#endif // SpectralPeakArrayAspect.hxx
