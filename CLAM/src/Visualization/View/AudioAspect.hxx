#ifndef __AUDIOASPECT__
#define __AUDIOASPECT__

#include "Aspect.hxx"
#include "Signalv1.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"

namespace CLAMGUI
{
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
				Signalv1< const DataArray& >   AcquireSamples;
				Signalv1< TTime >              AcquireDuration;
				Signalv1< TTime >              AcquireStartTime;
				Signalv1< TData >              AcquireSampleRate;

		private:
				AudioAdapter&                  mAdapter;
		};

}


#endif // AudioAspect.hxx
