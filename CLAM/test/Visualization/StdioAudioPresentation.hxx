#ifndef __STDIOAUDIOPRESENTATION__
#define __STDIOAUDIOPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Slot.hxx"

namespace CLAMGUI
{
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

		// slots
		Slot       mBufferSlot;
		Slot       mLenSlot;
		Slot       mStartSlot;
		Slot       mRateSlot;

		// Implementation details
protected:

		virtual void Bind( Aspect& ) throw (std::bad_cast);

		// callback methods to retrieving data from the view
		virtual void HandleIncomingBuffer( const DataArray& array );
		virtual void HandleIncomingDuration(  TTime secs );
		virtual void HandleIncomingStartTime(  TTime secs );
		virtual void HandleIncomingSampleRate(  TData rate );

		// Public class interface
public:
		
		StdioAudioPresentation();

		virtual ~StdioAudioPresentation();

		void Show();
};

}

#endif // StdioAudioPresentation.hxx
