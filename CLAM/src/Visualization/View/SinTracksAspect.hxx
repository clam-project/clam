#ifndef __SINETRACKSASPECT__
#define __SINETRACKSASPECT__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Signalv1.hxx"
#include "Signalv2.hxx"

namespace CLAMVM
{
		using SigSlot::Signalv1;
		using SigSlot::Signalv2;
		using CLAM::Array;
		using CLAM::TTime;
		using CLAM::TData;

		class SinTracksAdapter;

		class SinTracksAspect
		{
		public:
				
				SinTracksAspect( SinTracksAdapter& );
				
				virtual ~SinTracksAspect();

				virtual void ForceViewRefresh();

				// Signals
				Signalv2< const Array<Partial>& , int > AcquirePartials;
				Signalv1< TData >                       AcquireSpectralRange;
				Signalv2< TTime, TTime >                AcquireDuration;

		private:
				SinTracksAdapter&       mAdapter;
		};

}

#endif // SinTracksAspect.hxx
