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

#include "GLArrayRenderer.hxx"
#include "Viewport.hxx"
#include <algorithm>
#include "Assert.hxx"
using std::max_element;
using std::min_element;
#include "CLAM_Math.hxx"
#include <iostream>
using namespace CLAMGUI;

void GLArrayRenderer::InitArray( unsigned int nelems )
{
	GLfloat color[3] = { 0.0f, 0.0f, 0.0f };
	color[0] = GLfloat(r)/255.0f; color[1] = GLfloat(g)/255.0f; color[2] = GLfloat(b)/255.0f;

	for (unsigned int k = 0; k < nelems; k++ )
	{
		mIntertwined[k].r = color[0]; mIntertwined[k].g = color[1]; mIntertwined[k].b = color[2];
		mIntertwined[k].x = GLfloat( k );     // We update here the x-coordinates
		mIntertwined[k].z = 0.0f;             // We make zero the z-values
	}

}

void GLArrayRenderer::ResizeArray( unsigned int new_size )
{
	mIntertwined.resize( new_size );
	InitArray( new_size );
	
	/** This resize should be done in the FindMaxMin method*/
	mMaxElemIdxBuffer.resize( new_size );
	mMinElemIdxBuffer.resize( new_size );
	mLastMaxIndex = 0;
	mLastMinIndex = 0;
	mFirstMaxIndex = 0;
	mFirstMinIndex = 0;
	mFirstIndex=0;
	mLastIndex=0;
}

void GLArrayRenderer::CacheData( const DataArray& array )
{

	unsigned int nbins = array.Size();

	if ( nbins != mIntertwined.size() )
		ResizeArray( nbins ); // Valarray resizing to accomodate the new CLAM Array

	DataTransform( array );
	mDataChanged = true;
	FindMaxMin();
}

void GLArrayRenderer::DataTransform( const DataArray& array )
{
	for ( int k = 0; k < array.Size(); k++ )
	{
		mIntertwined[k].y = GLfloat( array[k] ); 
	}

}

void GLArrayRenderer::Draw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	if ( mDataChanged )
		{
			// caches the data on the video card ( if possible, otherwise it remains in 
			// processor memory - bad luck boy)
			glInterleavedArrays (GL_C3F_V3F, 0, &mIntertwined[0]);
			mDataChanged = false;
			
		}
	//if ( mCullingRequested )
	if(mCullingData.right-mCullingData.left>50000)
		{
			if ( mMustGenerateIndexes )
				{
					GenerateElemIndexes();
					mMustGenerateIndexes = false;
				}
			
			glDrawElements( GL_LINE_STRIP, (mLastIndex-mFirstIndex), GL_UNSIGNED_INT, &mElemIdxBuffer[0] );
			//glDrawElements( GL_LINE_STRIP, mLastMaxIndex-mFirstMaxIndex, GL_UNSIGNED_INT, &mMaxElemIdxBuffer[mFirstMaxIndex] );
			//glDrawElements( GL_LINE_STRIP, mLastMinIndex-mFirstMinIndex, GL_UNSIGNED_INT, &mMinElemIdxBuffer[mFirstMinIndex] );
		}
	else
		{
			glDrawArrays( GL_LINE_STRIP, 0, mIntertwined.size() );
		}
	glFlush();
}

void GLArrayRenderer::DefineViewport( const DataArray& array, Viewport& view_specs )
{


	TData top ;
	TData bottom ;	
	
	// First of all we check if the caller knows something about the data to be rendered
	
	if ( view_specs.top_es ) // Estimation provided
		top = view_specs.top;
	else
		top    = *( max_element( array.GetPtr(), array.GetPtr()+array.Size() - 1 ) );
	
	if ( view_specs.bottom_es) // Estimation provided
		bottom = view_specs.bottom;
	else
		bottom = *( min_element( array.GetPtr(), array.GetPtr()+array.Size() - 1 ) );
	
	// MRJ:
	// we should avoid that top and bottom match, since it seems to cause an undetermined
	// 'memory straying' somewhere. The possible causes could range from a faulty OpenGL
	// ICD to a hidden bug in FLTK. However I haven't got a clue what happens when you
	// specify a projection with the same "top" and "bottom", I should revise the projective
	// math, since I fear that some numerical unstability problem manifestates 
	
	// :KLUDGE: let's pad the top and bottom when they are equal
	
	if ( fabs( top - bottom ) < 0.01 )
		{
			top += TData(1.0);
		}

	TData left   = 0;
	TData right  = (TData) array.Size();
	
	XaxisTransform( left, right, view_specs.left, view_specs.right, view_specs.isIntX );
	YaxisTransform( top,  bottom, view_specs.top, view_specs.bottom, view_specs.isIntY );
}

void GLArrayRenderer::XaxisTransform( TData left, TData right, TData& transleft, TData& transright, bool& integer )
{
	transleft = left;
	transright = right;
	integer = true;
}

void GLArrayRenderer::YaxisTransform( TData top, TData bottom, TData& transtop, TData& transbottom, bool& integer )
{
	transtop = top;
	transbottom = bottom;
	integer = true;
}

void GLArrayRenderer::PerformCulling( float left, float right, unsigned pixel_width )
{
	mCullingData.left = left*GetXConversionFactor();
	mCullingData.right = (right*GetXConversionFactor());
	mCullingData.pixel_width = pixel_width;
	mCullingRequested = true;
	mMustGenerateIndexes = true;	

}

void GLArrayRenderer::GenerateElemIndexes()
{
	GLuint start, end, range, step;
	start = mCullingData.left;
	end = mCullingData.right;
	CLAM_ASSERT( start<end, "Start and End indexes cannot be equal!!!!" );
//	CLAM_ASSERT( mMaxElemIdxBuffer.size() > end, "End index is out of bonds!!!!" );
//	CLAM_ASSERT( mMinElemIdxBuffer.size() > end, "End index is out of bonds!!!!" );


	unsigned k;
	
	bool found=false;

	if(mCullingData.left>mElemIdxBuffer[mFirstIndex])
	{
		for(k=mFirstIndex;k<mnMaxMin;k++)
		{
			if(mElemIdxBuffer[k]>mCullingData.left)
			{
				mFirstIndex=k;
				found=true;
				break;
			}
		}
		if(!found) mFirstIndex=mnMaxMin;
	}
	else if(mCullingData.left<mElemIdxBuffer[mFirstIndex])
	{
		for(k=mFirstIndex;k>-1;k--)
		{
			if(mElemIdxBuffer[k]<mCullingData.left)
			{
				mFirstIndex=k+1;
				found=true;
				break;
			}
		}
		if(!found) mFirstIndex=0;
	}

	found=false;

	if(mCullingData.right>mElemIdxBuffer[mLastIndex])
	{
		for(k=mLastIndex;k<mnMaxMin;k++)
		{
			if(mElemIdxBuffer[k]>mCullingData.right)
			{
				mLastIndex=k-1;
				found=true;
				break;
			}
		}
		if(!found) mLastIndex=mnMaxMin;
	}
	else if(mCullingData.right<mElemIdxBuffer[mLastIndex])
	{
		for(k=mLastIndex;k>-1;k--)
		{
			if(mElemIdxBuffer[k]<mCullingData.right)
			{
				mLastIndex=k;
				found=true;
				break;
			}
		}
		if(!found) mLastIndex=0;
	}


/*	if(mCullingData.left>mMaxElemIdxBuffer[mFirstMaxIndex])
	{
		for(k=mFirstMaxIndex;k<mnMax;k++)
		{
			if(mMaxElemIdxBuffer[k]>mCullingData.left)
			{
				mFirstMaxIndex=k;
				found=true;
				break;
			}
		}
		if(!found) mFirstMaxIndex=mnMax;
	}
	else if(mCullingData.left<mMaxElemIdxBuffer[mFirstMaxIndex])
	{
		for(k=mFirstMaxIndex;k>-1;k--)
		{
			if(mMaxElemIdxBuffer[k]<mCullingData.left)
			{
				mFirstMaxIndex=k+1;
				found=true;
				break;
			}
		}
		if(!found) mFirstMaxIndex=0;
	}

	CLAM_ASSERT( start<end, "Start and End indexes cannot be equal!!!!" );

	if ( end >= mElemIdxBuffer.size() )
		end = mElemIdxBuffer.size()-1;

	range = end - start;

	found=false;

	if(mCullingData.left>mMinElemIdxBuffer[mFirstMinIndex])
	{
		for(k=mFirstMinIndex;k<mnMin;k++)
		{
			if(mMinElemIdxBuffer[k]>mCullingData.left)
			{
				mFirstMinIndex=k;
				found=true;
				break;
			}
		}
		if(!found) mFirstMinIndex=mnMin;
	}
	else if(mCullingData.left<mMinElemIdxBuffer[mFirstMinIndex])
	{
		for(k=mFirstMinIndex;k>-1;k--)
		{
			if(mMinElemIdxBuffer[k]<mCullingData.left)
			{
				mFirstMinIndex=k+1;
				found=true;
				break;
			}
		}
		if(!found) mFirstMinIndex=0;
	}

	found=false;

	if(mCullingData.right>mMaxElemIdxBuffer[mLastMaxIndex])
	{
		for(k=mLastMaxIndex;k<mnMax;k++)
		{
			if(mMaxElemIdxBuffer[k]>mCullingData.right)
			{
				mLastMaxIndex=k-1;
				found=true;
				break;
			}
		}
		if(!found) mLastMaxIndex=mnMax;
	}
	else if(mCullingData.right<mMaxElemIdxBuffer[mLastMaxIndex])
	{
		for(k=mLastMaxIndex;k>-1;k--)
		{
			if(mMaxElemIdxBuffer[k]<mCullingData.right)
			{
				mLastMaxIndex=k;
				found=true;
				break;
			}
		}
		if(!found) mLastMaxIndex=0;
	}

	found=false;

	if(mCullingData.right>mMinElemIdxBuffer[mLastMinIndex])
	{
		for(k=mLastMinIndex;k<mnMin;k++)
		{
			if(mMinElemIdxBuffer[k]>mCullingData.right)
			{
				mLastMinIndex=k-1;
				found=true;
				break;
			}
		}
		if(!found) mLastMinIndex=mnMin;
	}
	else if(mCullingData.right<mMinElemIdxBuffer[mLastMinIndex])
	{
		for(k=mLastMinIndex;k>-1;k--)
		{
			if(mMinElemIdxBuffer[k]<mCullingData.right)
			{
				mLastMinIndex=k;
				found=true;
				break;
			}
		}
		if(!found) mLastMinIndex=0;
	}
		
*/		
	
}



void GLArrayRenderer::FindMaxMin()
{
	//Minimum value that will be acknowledged as maximum or minimum
	float minY=0.05;
	
	unsigned int nElems=mIntertwined.size();
	
	float leftY,middleY,rightY;
	// detection loop 
	int i;
	
	//We always add a start point at zero
	mMinElemIdxBuffer[0]=0;
	mMaxElemIdxBuffer[0]=0;
	mIntertwined[0].y=0;

	mnMax=1;
	mnMin=1;

	for (i=2;i<nElems-2;i++) 
	{
		
		leftY 	= mIntertwined[i].y;
		middleY	= mIntertwined[i+1].y;
		rightY 	= mIntertwined[i+2].y;

		// local constant detected 
		if (middleY == leftY && leftY == rightY) 
		{
			//do nothing
			continue;
		}
	
		// local Minimum detected 
		if ((middleY <= leftY) && (middleY<= rightY)) 
		{
			if(middleY > -minY) mIntertwined[i+1].y=0;
			
			float interpolatedPosition;
			
			int pointPosition = i+1; 	// middleY has index i+1
			// if we get to the end of a constant area then ... 
			if ((middleY == leftY) && (middleY < rightY) && (mnMin > 0))
			{
		
				// update last PointPosition, it will be located in the middle of the constant area 
				interpolatedPosition = (double) mMinElemIdxBuffer[mnMin-1] + (double) (i+1-mMinElemIdxBuffer[mnMin-1])/2; // center BinPos 
				mMinElemIdxBuffer[mnMin-1]= interpolatedPosition; // interpolated BinPos is stored	
			}

			else 
			{ 
				// quadratic interpolation
				float diffFromMin =  TData(0.5) * ((leftY-rightY) / (leftY- 2*middleY + rightY));
				interpolatedPosition = pointPosition+diffFromMin;
				mMinElemIdxBuffer[mnMin]=(interpolatedPosition);
								
				mnMin++;
			}
		}

			
		// local maximum Detected ! 
		if ((middleY >= leftY) && (middleY >= rightY)) 
		{	
			if(middleY < minY) mIntertwined[i+1].y=0;
			float diffFromMax;
			float interpolatedPosition;
			int pointPosition = i+1; 	// middleY has index i+1
			// if we get to the end of a constant area then ... 
			if ((middleY == leftY) && (middleY > rightY) && (mnMax > 0)){
		
				// update last PointPosition, it will be located in the middle of the constant area 
				interpolatedPosition = (double) mMaxElemIdxBuffer[mnMax-1] + (double) (i+1-mMaxElemIdxBuffer[mnMax-1])/2; // center BinPos 
				mMaxElemIdxBuffer[mnMax-1]= interpolatedPosition; // interpolated BinPos is stored	
			}

			else { 
		
				// quadratic interpolation
				diffFromMax =  TData(0.5) * ((leftY-rightY) / (leftY- 2*middleY + rightY));
				interpolatedPosition = pointPosition+diffFromMax;
				mMaxElemIdxBuffer[mnMax]=(interpolatedPosition);
								
				mnMax++;
			}
		}
	}
	mMinElemIdxBuffer.resize(mnMin);
	mMaxElemIdxBuffer.resize(mnMax);
	
	//now we mix maximums and minimums and sort them
	
	mnMaxMin=mnMin+mnMax;
	
	mElemIdxBuffer.resize(mnMaxMin);
	for(i=0;i<mnMax;i++)
	{
		mElemIdxBuffer[i]=mMaxElemIdxBuffer[i];
	}
	for(i=0;i<mnMin;i++)
	{
		mElemIdxBuffer[i+mnMax]=mMinElemIdxBuffer[i];
	}
	std::sort(&mElemIdxBuffer[0],&mElemIdxBuffer[mnMaxMin-1]);
}








