#ifndef __PLOTEXAMPLESHELPER__
#define __PLOTEXAMPLESHELPER__

#include "DataTypes.hxx"

namespace CLAM
{
	template < typename T > class Array;
	class Spectrum;
}

extern void generateLowPassFilter( CLAM::Spectrum& spectrum );

extern void generateBandPassFilter( CLAM::Spectrum& spectrum );

extern void generateHighPassFilter( CLAM::Spectrum& spectrum );

extern void measureRandRandomness( CLAM::Array< CLAM::TData >& measure );

#endif // PlotExamplesHelper.hxx
