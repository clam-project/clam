#ifndef __SPECTRUMPRESENTATION__
#define __SPECTRUMPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv2.hxx"

namespace CLAMVM
{
		using SigSlot::Slotv2;
		using CLAM::DataArray;
		using CLAM::TData;
		using CLAM::TTime;
		
		class SpectrumPresentation : public Presentation
		{
		private:
				
		protected:
				virtual void Bind( Aspect& ) throw ( std::bad_cast );

				virtual void OnNewSpectrum( const DataArray&, TData ) = 0;
		public:
    
				SpectrumPresentation();

				~SpectrumPresentation();

				virtual void Show() = 0;

				Slotv2< const DataArray&, TData >       SetSpectrum;
				
		};
}

#endif // SpectrumPresentation.hxx
