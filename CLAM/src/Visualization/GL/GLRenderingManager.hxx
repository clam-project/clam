#ifndef __GLRENDERINGMANAGER__
#define __GLRENDERINGMANAGER__

#include "BoundingBoxes.hxx"
#include <cstdlib>

namespace CLAMVM
{

	class DrawingSurface2D;

	class GLRenderingManager
	{
	public:

		GLRenderingManager();

		virtual void SetWorldBounds( const DataBoundBox&  ) = 0;
		virtual void SetPixelBounds( const ScreenBoundBox&  ) = 0;
		virtual void RenderData() = 0;
		
		virtual ~GLRenderingManager();

		
		void SetSurface( DrawingSurface2D* s )
		{
			mSurface = NULL;
		}


		DrawingSurface2D* GetSurface()
		{
			return mSurface;
		}

	protected:


	private:

		DrawingSurface2D*  mSurface;
	};


}
#endif // GLRenderingManager.hxx
