#ifndef __SPECTRUMVIEW__
#define __SPECTRUMVIEW__

#include "ProcessingDataView.hxx"
#include "SpectrumViewAspect.hxx"

namespace CLAM
{
		class Spectrum;
		class ProcessingData;
}

namespace CLAMGUI
{

		using CLAM::Spectrum;
		using CLAM::ProcessingData;

		class SpectrumView : public ProcessingDataView
		{
				// attributes
		protected:
				SpectrumViewAspect  mAspect;
				const Spectrum*     mObserved;

				// Implementation Details
		protected:

				/**
				 *  This pure virtual method allows to define
				 *  which is the scale used for acquiring the spectrum
				 *  supporting the possibility to make the necessary conversions
				 *  (i.e. the observed Spectrum comes as a Complex Array for expressing
				 *  both magnitude and phase, but you are providing Linear Magnitud and Phase
				 *  - Polar complex numbers in two buffers ). This should try to do the least
				 *  work possible, taking the easiest approach to achieve this effect.
				 *  Look at @see Spectrum class to get an idea of the reasons behind this.
				 */

				virtual void TranslateSpectralData() = 0;

				// Public class interfacs
		public:

				SpectrumView();
				
				virtual ~SpectrumView();
				
				virtual const char* GetClassName() const
				{
						return "SpectrumView";
				}

				virtual Aspect& GetAspect()
				{
						return mAspect;
				}

				virtual bool Refresh() = 0;

				virtual bool BindTo( const ProcessingData* procDataObj );
		};

}

#endif // SpectrumView.hxx
