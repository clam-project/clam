#ifndef __AUDIOVIEW__
#define __AUDIOVIEW__

#include "View.hxx"
#include "AudioViewAspect.hxx"

namespace CLAM
{
		class Audio;
		class ProcessingData;
}

namespace CLAMGUI
{
		using CLAM::Audio;
		using CLAM::ProcessingData;

		class AudioView : public View
		{
				// attributes
		private:
				AudioViewAspect      mAspect;
				const Audio*         mObserved;

		protected:
				// methods
		public:
				AudioView();

				virtual ~AudioView();

				virtual const char* GetClassName() const
				{
						return "AudioView";
				}

				virtual Aspect& GetAspect()
				{
						return mAspect;
				}

				virtual bool Refresh();

				virtual bool BindTo( const ProcessingData* audioObj );

		};
}


#endif // AudioView.hxx
