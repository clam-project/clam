/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __GLARRAYRENDERER__
#define __GLARRAYRENDERER__

#include "Array.hxx"
#include <valarray>
#include "CLAMGL.hxx"
#include "VertexFormats.hxx"
#include "ArrayRenderer.hxx"
using CLAM::DataArray;

namespace CLAMGUI
{

class GLArrayRenderer
	: public ArrayRenderer
{
	struct tCullingData
	{
		unsigned left, right, pixel_width;
	};
public:

	GLArrayRenderer( unsigned char red = 0, unsigned char gree = 255, unsigned char blu = 0 )
		: mLastMaxIndex(0), mLastMinIndex(0), mFirstMaxIndex(0), mFirstMinIndex(0),r( red ), g( gree ), b( blu ), mDataChanged( false ), 
		mCullingRequested(false), mMustGenerateIndexes( true ), mFirstIndex(0),mLastIndex(0)
	{
		ResizeArray( 1024 );
	}

	virtual ~GLArrayRenderer()
	{
	}

	virtual void CacheData( const DataArray& array );
	virtual void Draw();
	virtual void PerformCulling( float left, float right, unsigned pixel_width );
	virtual void DefineViewport( const DataArray& array, Viewport& view_specs );

protected:

	void ResizeArray( unsigned int new_size);
	virtual void InitArray( unsigned int nelems ); // 1024 is assumed
	virtual void DataTransform( const DataArray& array);
	virtual void XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer );
	virtual void YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer );
	virtual float GetXConversionFactor()
	{
		return 1;
	}
	virtual void GenerateElemIndexes();
	virtual void FindMaxMin();

protected:

	std::valarray< c3f_v3f > mIntertwined;
	std::valarray< GLuint >  mElemIdxBuffer;
	std::valarray< GLuint >  mMaxElemIdxBuffer; // element index buffer for local maximums
	std::valarray< GLuint >  mMinElemIdxBuffer; // element index buffer for local minimums
	unsigned       mLastMaxIndex,mLastMinIndex,mFirstMaxIndex,mFirstMinIndex,mnMax,mnMin;
	unsigned       mLastIndex,mFirstIndex,mnMaxMin;
	unsigned char r;
	unsigned char g;
	unsigned char b;
	bool          mDataChanged;
	bool          mCullingRequested;
	bool          mMustGenerateIndexes;
	tCullingData  mCullingData;
};


}

#endif // GLArrayRenderer.hxx









