#ifndef __SINETRACKSVIEWASPECT__
#define __SINETRACKSVIEWASPECT__

#include "Aspect.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"

namespace CLAMGUI
{
	
		using CLAM::Array;
		using CLAM::TTime;
		using CLAM::TData;

		class SinTracksView;

		class SinTracksViewAspect : public Aspect
		{
		public:
				
				friend class SinTracksView;

				SinTracksViewAspect( SinTracksView& );
				
				virtual ~SinTracksViewAspect();

				virtual void ForceViewRefresh();

				// Signals
				Signalv2< const Array<Partial>& , int > AcquirePartials;
				Signalv1< TData >                       AcquireSpectralRange;
				Signalv2< TTime, TTime >                AcquireDuration;

		private:
				SinTracksView&       mView;
		};

}

#endif // SinTracksViewAspect.hxx
