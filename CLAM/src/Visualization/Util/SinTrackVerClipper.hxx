#ifndef __SINTRACKVERCLIPPER__
#define __SINTRACKVERCLIPPER__

#include "SineTracksDef.hxx"

namespace CLAMGUI
{

class SinTrackVerClipper
{
		typedef SineTrackSpanEnds::iterator span_iterator;
		typedef SinusoidalTrack::iterator   peak_iterator;

		typedef enum { Inside = 0x0, Outside = 0x01 } outcode; // 0x0 means Inside
public:
		SinTrackVerClipper( TData f = 30.0 );

		~SinTrackVerClipper();

		void Cull( TData f_lower, TData f_upper, SineTrackSpanEnds& pl_s, SineTrackSpanEnds& pl_e );

		void SetMinimumFreqRange( TData f )
		{
				mMinFreqRange = f;
		}

protected:
		
		void ClipSpans( TData f_lower, TData f_upper, peak_iterator& si, peak_iterator& ei, 
						SineTrackSpanEnds& pl_s, SineTrackSpanEnds& pl_e  );
private:
		inline outcode in_out_test( TData lower, TData upper, TData f )
		{

				if ( f > upper || f < lower ) return Outside;
				
				return Inside;
		}
		
		TData mLowFreq;
		TData mHiFreq;
		TData mMinFreqRange;
};

}

#endif // SinTrackVerClipper.hxx
