#include "PeaksRenderingManager.hxx"
#include "Assert.hxx"
#include <algorithm>

namespace CLAMVM
{
	PeaksRenderingManager::PeaksRenderingManager()
		: mMustProcessData( false ), mNoPeaksToShow( false ), mSpecRange( 22050 ), mNumBins( 513 )
	{
		mPeakRenderer.SetLineColor( 0, 0, 255 );
	}

	PeaksRenderingManager::~PeaksRenderingManager()
	{
	}

	void PeaksRenderingManager::SetWorldBounds( const DataBoundBox& dataBBox )
	{
		mDataBBox = dataBBox;
		mMustProcessData = true;
	}

	void PeaksRenderingManager::SetPixelBounds( const ScreenBoundBox& scrbbox )
	{
		mScreenBBox = scrbbox;
		mMustProcessData = true;
	}

	void PeaksRenderingManager::SetSpectralRange( TData srange )
	{
		mSpecRange = srange;
		mMustProcessData = true;
	}

	void PeaksRenderingManager::SetBinNumber( TSize binNumber )
	{
		mNumBins = binNumber;
		mMustProcessData = true;
	}

	void PeaksRenderingManager::CacheData( const Array<Partial>& array )
	{
		if ( mDataCached.Size() < array.Size() )
			mDataCached.Resize( array.Size() );
		mDataCached.SetSize( array.Size() );

		std::copy( array.GetPtr(), array.GetPtr()+array.Size(), mDataCached.GetPtr() );

		if ( mProcessedX.Size() < array.Size() )
			mProcessedX.Resize( array.Size() );
		if ( mProcessedY.Size() < array.Size() )
			mProcessedY.Resize( array.Size() );

		mProcessedX.SetSize( array.Size()  );
		mProcessedY.SetSize( array.Size() );

		ConvertPartialsToArrays();

		mMustProcessData = true;
	}


	void PeaksRenderingManager::ProcessData()
	{
				
		DetermineVisibleSamples();

		TSize processedLen = (mRightMostPeak - mLeftMostPeak) + 1;				


		mPeakRenderer.SetXArray( mProcessedX.GetPtr()+mLeftMostPeak, processedLen );
		mPeakRenderer.SetYArray( mProcessedY.GetPtr()+mLeftMostPeak, processedLen );
		mPeakRenderer.SetYMinimum( mDataBBox.mBottom );

		mMustProcessData = false;
				
	}

	void PeaksRenderingManager::ConvertPartialsToArrays()
	{
		TIndex peakIndex = mLeftMostPeak;

		for ( peakIndex = 0; peakIndex < mProcessedX.Size(); peakIndex++ )
		{
			mProcessedX[peakIndex]= mDataCached[peakIndex].mFreq * ( mNumBins / mSpecRange );
			mProcessedY[peakIndex]= mDataCached[peakIndex].mMag;
		}
				
	}

	void PeaksRenderingManager::DetermineVisibleSamples()
	{
		TData minFreq = (mDataBBox.mLeft / mNumBins)*mSpecRange;
		TData maxFreq = (mDataBBox.mRight / mNumBins)*mSpecRange;

		TIndex i = 0;

		if ( mDataCached[0].mFreq > maxFreq )
		{
			mNoPeaksToShow = true;
			return;
		}
		mNoPeaksToShow = false;

		while ( i < mDataCached.Size() )
		{
			if ( mDataCached[i].mFreq >= minFreq )
				break;
			i++;
		}
		mLeftMostPeak = i;

		while ( i <= mDataCached.Size()-1 )
		{
			if ( mDataCached[i].mFreq > maxFreq )
				break;
			i++;
		}
		mRightMostPeak = i;

				
	}
		
	void PeaksRenderingManager::RenderData()
	{
		if ( !IsEnabled() )
			return;

		if ( mMustProcessData )
			ProcessData();
				
		mPeakRenderer.ExecuteGLCommands();
	}


}
