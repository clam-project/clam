#ifndef __LOGMAGSPECTRUMVIEW__
#define __LOGMAGSPECTRUMVIEW__

#include "SpectrumView.hxx"
#include "Array.hxx"

namespace CLAMGUI
{

		class LogMagSpectrumView : public SpectrumView
		{
				// attributes
		private:
				DataArray    mMagBuffer;
				DataArray    mPhaseBuffer;

		protected:
				void TrivialCaseConversion();
				void TranslateSpectralData();
		public:
				
				virtual ~LogMagSpectrumView();

				virtual Aspect& GetAspect()
				{
					return mAspect;
				}

				virtual const char* GetClassName() const
				{
						return "LogMagSpectrumView";
				}

				virtual bool Refresh();
		};

}

#endif // LogMagSpectrumView.hxx
