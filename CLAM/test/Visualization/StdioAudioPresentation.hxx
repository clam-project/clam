#ifndef __STDIOAUDIOPRESENTATION__
#define __STDIOAUDIOPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slotv1.hxx"

namespace CLAMVM
{
	using SigSlot::Slotv1;
	using CLAM::DataArray;
	using CLAM::TData;
	using CLAM::TTime;

class StdioAudioPresentation : public Presentation
{
		// attributes
private:
		TData      mMaximum;
		TData      mMinimum;
		TTime      mAudioLen;
		TTime      mAudioStart;
		TData      mSampleRate;


		// Implementation details
protected:

		virtual void Bind( Aspect& ) throw (std::bad_cast);

		// callback methods to retrieving data from the vie
//public slots:
		virtual void OnNewSamples( const DataArray& array );
		virtual void OnNewDuration(  TTime secs );
		virtual void OnNewStartTime(  TTime secs );
		virtual void OnNewSampleRate(  TData rate );

		// Public class interface
public:
		
		StdioAudioPresentation();

		// slots
		Slotv1<const DataArray&>     SetSamples;
		Slotv1<TTime>                SetDuration;
		Slotv1<TTime>                SetStartTime;
		Slotv1<TData>                SetSampleRate;

		virtual ~StdioAudioPresentation();

		void Show();
};

}

#endif // StdioAudioPresentation.hxx
