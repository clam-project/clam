#ifndef __AUDIOASPECT__
#define __AUDIOASPECT__

#include "Aspect.hxx"
#include "Signalv1.hxx"
#include "Signalv4.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAMVM
{
		using SigSlot::Signalv1;
		using SigSlot::Signalv4;
		using CLAM::TData;
		using CLAM::TTime;
		using CLAM::DataArray;
		class AudioAdapter;

		class AudioAspect : public Aspect
		{
		public:				
				AudioAspect( AudioAdapter& );

				virtual ~AudioAspect();

				virtual void ForceViewRefresh();

				//signals
				Signalv1< const DataArray& >                          AcquireSamples;
				Signalv1< TTime >                                     AcquireDuration;
				Signalv1< TTime >                                     AcquireStartTime;
				Signalv1< TData >                                     AcquireSampleRate;
				Signalv4< const DataArray&, TTime, TTime, TData >     AcquireAudio;

		private:
				AudioAdapter&                  mAdapter;
		};

}


#endif // AudioAspect.hxx
