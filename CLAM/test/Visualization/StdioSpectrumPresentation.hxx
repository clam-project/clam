#ifndef __STDIOSPECTRUMPRESENTATION__
#define __STDIOSPECTRUMPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv1.hxx"

namespace CLAMVM
{
		using SigSlot::Slotv1;
		using CLAM::DataArray;
		using CLAM::TData;
		
		class StdioSpectrumPresentation : public Presentation
		{
				// attributes
		private:
				DataArray        mBinsMagnitude;
				DataArray        mBinsPhase;
				TData            mSpectralRange;


				// Implementation details
		protected:
				virtual void Bind( Aspect& ) throw ( std::bad_cast );
				
				// callback methods to be called by the view
				virtual void OnNewMagBins( const DataArray& array );
				virtual void OnNewPhaseBins( const DataArray& array );
				virtual void OnNewSpecRng( TData specRange );

		public:
				// slots
				Slotv1<const DataArray&>         SetMagnitudeBins;
				Slotv1<const DataArray&>         SetPhaseBins;
				Slotv1<TData>                    SetSpectralRange;


				StdioSpectrumPresentation();

				virtual ~StdioSpectrumPresentation();

				void Show();
		};
		
}


#endif // StdioSpectrumPresentation.hxx
