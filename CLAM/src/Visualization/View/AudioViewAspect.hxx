#ifndef __AUDIOVIEWASPECT__
#define __AUDIOVIEWASPECT__

#include "Aspect.hxx"
#include "Signalv1.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAMGUI
{
		using CLAM::TData;
		using CLAM::TTime;
		using CLAM::DataArray;
		class AudioView;

		class AudioViewAspect : public Aspect
		{
		public:
				friend class AudioView;
				
				AudioViewAspect( AudioView& );

				virtual ~AudioViewAspect();

				virtual void ForceViewRefresh();

				//signals
				Signalv1< const DataArray& >   AcquireSamples;
				Signalv1< TTime >        AcquireDuration;
				Signalv1< TTime >        AcquireStartTime;
				Signalv1< TData >        AcquireSampleRate;

		private:
				AudioView&               mView;
		};

}


#endif // AudioViewAspect.hxx
