#ifndef __PARTIAL__
#define __PARTIAL__

#include "DataTypes.hxx"

namespace CLAMGUI
{
		using CLAM::TData;

		struct Partial
		{
				TData    mMag;
				TData    mFreq;
				TData    mPhase;
		};
}

#endif // Partial.hxx
