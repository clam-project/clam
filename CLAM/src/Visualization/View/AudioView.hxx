#ifndef __AUDIOVIEW__
#define __AUDIOVIEW__

#include "ProcessingDataAdapter.hxx"
#include "AudioAspect.hxx"

namespace CLAM
{
		class Audio;
		class ProcessingData;
}

namespace CLAMGUI
{
		using CLAM::Audio;
		using CLAM::ProcessingData;

		class AudioAdapter : public ProcessingDataAdapter
		{
				// attributes
		private:
				AudioAspect          mAspect;
				const Audio*         mObserved;

		protected:
				// methods
		public:
				AudioAdapter();

				virtual ~AudioAdapter();

				virtual const char* GetClassName() const
				{
						return "AudioAdapter";
				}

				virtual Aspect& GetAspect()
				{
						return mAspect;
				}

				virtual bool Publish();

				virtual bool BindTo( const ProcessingData* audioObj );

		};
}


#endif // AudioAdapter.hxx
