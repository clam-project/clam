#ifndef __GLSPECTRALBLOCK__
#define __GLSPECTRALBLOCK__

#include "ArrayRenderer.hxx"
#include "CLAMGL.hxx"
#include "TabFunct.hxx"
#include "CLAM_Math.hxx"
#include <list>
#include <utility>

namespace CLAMGUI
{

using CLAM::TData;

class GLSpectralBlock
	: public ArrayRenderer
{

	struct tIndexRange
	{
		int left;
		int right;
		int size;
	};
	typedef std::list<tIndexRange> tIndexList;

public:

	GLSpectralBlock( int divs );

	virtual ~GLSpectralBlock()
	{
	}

	void CacheData( const DataArray& );

	void CacheSampleRate( TData sampleRate );

	void DefineViewport( const DataArray&, Viewport& );

	void Draw();

protected:

	void CalculateBandRanges( int arraySize );

	void GenerateBlockHeights( const DataArray& array );

	void DataTransform( const DataArray& );

	void XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer )
	{
	}

	void YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer )
	{
	}

	void BuildDisplayList();

private:	
	inline float quantize_dB_values( float dB_value )
	{
		static float inv35db = 1.0f/35.0f;
		
		// value clamping
		
		if ( dB_value > 0.0f )
			return 1.0f;
		else if ( dB_value < -35.0f )
			return 0.0f;
		
		return (dB_value +35.0f )*inv35db;
		
	}


	struct Log10_20
	{
		inline TData operator()(const TData arg)
		{
			return 20.0f * log10( arg );
		}
	};

	int                       mDivisions;
	DataArray                 mBlockHeights;
	GLuint                    mDLid;
	bool                      mDLready;
	TData                     mSpectralRange;
	//	CLAM::TabFunct<Log10_20>  mLogFunc;
	float                     mOctaveStride;
	const float               mF0;
	const float               mFf;
	bool                      mMustCalculateIndexes;
	tIndexList                mRangesList;
};

}

#endif // GLSpectralBlock.hxx
