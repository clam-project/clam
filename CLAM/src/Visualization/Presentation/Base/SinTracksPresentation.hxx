#ifndef __SINTRACKSPRESENTATION__
#define __SINTRACKSPRESENTATION__

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
		using CLAM::TData;
		using CLAM::TTime;
		using CLAM::TSize;

		class SinTracksPresentation : public Presentation
		{
		protected:

				virtual void Bind( Aspect& ) throw ( std::bad_cast );
				
				virtual void OnNewPartials( const Array<Partial>&, TIndex ) = 0; 
				
				virtual void OnNewRange( TData ) = 0;

				virtual void OnNewDuration( TTime begin, TTime end ) = 0;
		public:

				SinTracksPresentation();
				~SinTracksPresentation();

				virtual void Show() = 0;

				Slotv2<const Array<Partial>&, TIndex > AddPartials;
				Slotv1<TData>                     SetSpectralRange;
				Slotv2<TTime, TTime>              SetTimeInterval;
		};
}

#endif // SinTracksPresentation.hxx
