#ifndef __AUDIOPRESENTATION__
#define __AUDIOPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv4.hxx"

namespace CLAMGUI
{

		using CLAM::DataArray;
		using CLAM::TData;
		using CLAM::TTime;

		class AudioPresentation : public Presentation
		{
		private:
				TData mMaximum;
				TData mMinimum;
				
		protected:

				virtual void Bind( Aspect& ) throw( std::bad_cast );

//slots functions:
				virtual void OnNewAudio( const DataArray&, TTime, TTime, TData ) = 0;
// Accessors to received data
				
		public:

				AudioPresentation();

				~AudioPresentation();

				virtual void Show() = 0;

				Slotv4<const DataArray&, TTime, TTime, TData> SetAudio;
		};

}

#endif // AudioPresentation.hxx
