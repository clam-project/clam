#ifndef __SPECTRUMVIEWASPECT__
#define __SPECTRUMVIEWASPECT__

#include "Aspect.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "Signalv1.hxx"

namespace CLAMGUI
{
		using CLAM::TData;
		using CLAM::DataArray;

		class SpectrumView;

		class SpectrumViewAspect : public Aspect
		{
		public:

				friend class SpectrumView;

				SpectrumViewAspect( SpectrumView& );
				
				virtual ~SpectrumViewAspect();

				virtual void ForceViewRefresh();

				// signals
				Signalv1< const DataArray& > AcquireMagnitude;
				Signalv1< const DataArray& > AcquirePhase;
				Signalv1< TData >            AcquireSpectralRange;
				

		private:
				SpectrumView&           mView;
		};

}

#endif // SpectrumViewAspect.hxx
