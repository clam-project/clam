#ifndef __GLSTRAIGHTLINEARRAY__
#define __GLSTRAIGHTLINEARRAY__

#include "GLRenderer.hxx"
#include "DataTypes.hxx"

namespace CLAMGUI
{
		using CLAM::TData;

class GLStraightLineArray
		: public GLRenderer
{
public:

		GLStraightLineArray();

		virtual ~GLStraightLineArray();

		void SetLineColor( unsigned char r, unsigned char g, unsigned char b )
		{
				mColor[0] = r; 
				mColor[1] = g; 
				mColor[2] = b;
		}

		void SetStartAndHopSize( TData xstart, TData xhop )
		{
				mXStart = xstart;
				mHopSize = xhop;
		}

		void SetDataPtr( const TData* pValues, unsigned nelems )
		{
				mValues = pValues;
				mElemsToDraw = nelems;
		}

		virtual void ExecuteGLCommands();

protected:

		unsigned mElemsToDraw;
		const TData* mValues;
		TData mXStart;
		TData mXHopSize;
		unsigned char mLineColor[3];
};

}

#endif // GLStraightLineArray.hxx
