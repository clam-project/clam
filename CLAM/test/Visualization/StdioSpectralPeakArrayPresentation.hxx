#ifndef __STDIOSPECTRALPEAKARRAYPRESENTATION__
#define __STDIOSPECTRALPEAKARRAYPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "DataTypes.hxx"
#include "Slotv1.hxx"


namespace CLAMVM
{
		using SigSlot::Slotv1;
		using CLAM::Array;
		
		class StdioSpectralPeakArrayPresentation : public Presentation
		{
		private:
				Array<Partial>         mPartialsToDraw;


		protected:

				virtual void Bind( Aspect& ) throw ( std::bad_cast );

				// callback methods to be called by the view

				virtual void OnNewPartials( const Array<Partial>& array );
		public:

				Slotv1<const Array<Partial>& >            SetPartials;

				StdioSpectralPeakArrayPresentation();
				virtual ~StdioSpectralPeakArrayPresentation();
				
				void Show();
		};	
}

#endif // StdioSpectralPeakArrayPresentation.hxx
