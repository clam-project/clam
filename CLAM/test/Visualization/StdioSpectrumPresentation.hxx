#ifndef __STDIOSPECTRUMPRESENTATION__
#define __STDIOSPECTRUMPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slot.hxx"

namespace CLAMGUI
{
		using CLAM::DataArray;
		using CLAM::TData;
		
		class StdioSpectrumPresentation : public Presentation
		{
				// attributes
		private:
				DataArray        mBinsMagnitude;
				DataArray        mBinsPhase;
				TData            mSpectralRange;

				// slots
				Slot             mBinsMagSlot;
				Slot             mBinsPhaseSlot;
				Slot             mBinsSpecRngSlot;

				// Implementation details
		protected:
				virtual void Bind( Aspect& ) throw ( std::bad_cast );
				
				// callback methods to be called by the view
				virtual void HandleIncomingMagBins( const DataArray& array );
				virtual void HandleIncomingPhaseBins( const DataArray& array );
				virtual void HandleIncomingSpecRng( TData specRange );

		public:

				StdioSpectrumPresentation();

				virtual ~StdioSpectrumPresentation();

				void Show();
		};
		
}


#endif // StdioSpectrumPresentation.hxx
