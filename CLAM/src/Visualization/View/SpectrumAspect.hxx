#ifndef __SPECTRUMASPECT__
#define __SPECTRUMASPECT__

#include "Aspect.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"

namespace CLAMGUI
{
		using CLAM::TData;
		using CLAM::DataArray;

		class SpectrumAdapter;

		class SpectrumAspect : public Aspect
		{
		public:

				SpectrumAspect( SpectrumAdapter& );
				
				virtual ~SpectrumAspect();

				virtual void ForceViewRefresh();

				// signals
				Signalv1< const DataArray& > AcquireMagnitude;
				Signalv1< const DataArray& > AcquirePhase;
				Signalv1< TData >            AcquireSpectralRange;
				Signalv2< const DataArray&, TData > AcquireSpectrum;

		private:
				SpectrumAdapter&           mAdapter;
		};

}

#endif // SpectrumAspect.hxx
