#ifndef __SPECTRALPEAKSPRESENTATION__
#define __SPECTRALPEAKSPRESENTATION__

#include "Presentation.hxx"
#include "Partial.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv2.hxx"
#include "Slotv1.hxx"

namespace CLAMVM
{
		using SigSlot::Slotv1;
		using SigSlot::Slotv2;
		using CLAM::Array;
		using CLAM::DataArray;
		using CLAM::TData;

		class SpectralPeaksModel;
		class SpectrumModel;

		class SpectrumPlusPeaksPresentation : public Presentation
		{
		private:
		protected:
				virtual void OnNewSpectrum( const DataArray&, TData ) = 0; 
				virtual void OnNewPeakArray( const Array<Partial>& ) = 0;

		public:

				SpectrumPlusPeaksPresentation();
				~SpectrumPlusPeaksPresentation();

				virtual void AttachTo( SpectrumModel& , SpectralPeaksModel& );
				virtual void Detach();

				virtual void Show() = 0;
				virtual void Hide() = 0;

				Slotv2< const DataArray&, TData >   SetSpectrum;
				Slotv1< const Array<Partial>& >     SetPartials;
		};
}

#endif // SpectrumPlusPeaksPresentation.hxx
