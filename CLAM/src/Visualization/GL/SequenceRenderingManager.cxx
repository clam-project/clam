#include "SequenceRenderingManager.hxx"
#include "Assert.hxx"
#include "CLAMGL.hxx"
#include <algorithm>

namespace CLAMVM
{
	SequenceRenderingManager::SequenceRenderingManager()
		: mDrawAsLines( true ), mDrawAsDetail( false ), mDetailThreshold( 100 ), mMustProcessData( false ), mOffset(0), mLen(0),
		  mLineColor( 0, 255, 0 ), mVisible_dx( 0.01 ), mStart_x( 0.0), mCulledAway( false )
		
	{
		mDataBounds.mLeft = 0.0;
		mDataBounds.mRight = 1.0;
		mDataBounds.mBottom = -1.0;
		mDataBounds.mTop = 1.0;
	}

	SequenceRenderingManager::~SequenceRenderingManager()
	{
	}
		
	void SequenceRenderingManager::SetWorldBounds( const DataBoundBox& dataBBox )
	{
		mDataBBox = dataBBox;
		mMustProcessData = true;
	}

	void SequenceRenderingManager::SetPixelBounds( const ScreenBoundBox& scrBBox )
	{
		mScreenBBox = scrBBox;

		mMustProcessData = true;
	}

	void SequenceRenderingManager::CacheData( const DataArray& data )
	{
		if ( mDataCached.Size() < data.Size() )
			mDataCached.Resize( data.Size() );
		mDataCached.SetSize( data.Size() );
				
		std::copy( data.GetPtr(), data.GetPtr()+data.Size(), mDataCached.GetPtr() );
		
		mOffset = 0;
		mLen = data.Size();
		mMustProcessData = true;				
	}

	void SequenceRenderingManager::ActivateStraightRenderer()
	{
		mDrawAsLines = true;
		mDrawAsDetail = false;
	}
		
	void SequenceRenderingManager::ActivateDetailRenderer()
	{
		mDrawAsDetail = true;
		mDrawAsLines = false;
	}

	void SequenceRenderingManager::ProcessData()
	{
		DetermineVisibleSamples();
				
		if ( mProcessedData.Size() <= mDetailThreshold )
		{
			ActivateDetailRenderer();
			mDetailRenderer.SetStartAndHopSize( mStart_x, mVisible_dx );
			mDetailRenderer.SetDataPtr( mProcessedData.GetPtr(), mProcessedData.Size() );
		}
		else
		{
			ActivateStraightRenderer();
			mLineRenderer.SetStartAndHopSize( mStart_x, mVisible_dx );
			mLineRenderer.SetDataPtr( mProcessedData.GetPtr(), mProcessedData.Size() );
						
		}
				
		mMustProcessData = false;
	}

	void SequenceRenderingManager::RenderData()
	{
		if ( mDataCached.Size() <= 0 ) // has data
				return;
		if ( mMustProcessData )
			ProcessData();

		if ( mCulledAway )
			return;

		if ( mDrawAsLines )
		{
			mLineRenderer.SetLineColor( mLineColor.r, mLineColor.g, mLineColor.b );
			mLineRenderer.ExecuteGLCommands();
		}
		else
		{
			mDetailRenderer.SetLineColor( mLineColor.r, mLineColor.g, mLineColor.b );
			mDetailRenderer.ExecuteGLCommands();
		}
	}

	void SequenceRenderingManager::SetDataYRange( double ymin, double ymax )
	{
		mDataBounds.mBottom = ymin;
		mDataBounds.mTop = ymax;
	}

	void SequenceRenderingManager::SetDataXRange( double xmin, double xmax )
	{
		mDataBounds.mLeft = xmin;
		mDataBounds.mRight = xmax;
	}

	void SequenceRenderingManager::DetermineVisibleSamples()
	{
		TData dataSpan = fabs( mDataBounds.mRight - mDataBounds.mLeft );
		TData visibleSpan = fabs(mDataBBox.mRight - mDataBBox.mLeft);

		mCulledAway = false;
		
		if ( mDataBBox.mLeft < mDataBounds.mLeft )
		{
			mOffset = 0;
			mStart_x = mDataBounds.mLeft;			
		}
		else if ( mDataBBox.mLeft > mDataBounds.mRight ) // Totally culled away
		{
			mCulledAway = true;
			return;
		}
		else
		{
			mOffset = ( fabs( mDataBounds.mLeft - mDataBBox.mLeft)/dataSpan )* mDataCached.Size();
			mStart_x = mDataBBox.mLeft;
		}

		if ( mDataBBox.mRight < mDataBounds.mLeft ) // totally culled away
		{
			mCulledAway = true;
			return;
		}
		else if ( mDataBBox.mRight > mDataBounds.mRight )
		{
			mLen = mDataCached.Size() - mOffset;
		}
		else
		{
			mLen = (fabs(mDataBBox.mRight - mDataBounds.mLeft) / dataSpan)*mDataCached.Size();
			mLen -= mOffset;
			
		}

		if ( mProcessedData.Size() < mLen )
			mProcessedData.Resize( mLen );

		mProcessedData.SetSize( mLen );

		std::copy( mDataCached.GetPtr()+mOffset, mDataCached.GetPtr()+mOffset+mLen, mProcessedData.GetPtr() );
		
		mVisible_dx = dataSpan / TData(mProcessedData.Size()-1);
				
	}
}
