#include "DrawingSurface2D.hxx"
#include <cmath>

namespace CLAMGUI
{
		DrawingSurface2D::DrawingSurface2D()
				: mxmax( 1.0f ), mxmin( -1.0f ), mymax( 1.0f ), mymin( -1.0f ),
				  mcxmax( 1.0f ), mcxmin( -1.0f ), mcymax( 1.0f ), mcymin( -1.0f ),
				  mxdist( 0 ), mydist(0)
		{
				mxdist = fabs( mxmax - mxmin );
				mydist = fabs( mymax - mymin );

				SetWorldSpace.Wrap( this, &DrawingSurface2D::_SetWorldSpace );
				AdjustXAxis.Wrap( this, &DrawingSurface2D::_AdjustXAxis );
				AdjustYAxis.Wrap( this, &DrawingSurface2D::_AdjustYAxis );
		}

		DrawingSurface2D::~DrawingSurface2D()
		{
		}

		void DrawingSurface2D::_SetWorldSpace( double xmax, double xmin, double ymax, double ymin )
		{
				mxmax = xmax;
				mxmin = xmin;
				mymax = ymax;
				mymin = ymin;
				
				mxdist = fabs( mxmax - mxmin );
				mydist = fabs( mymax - mymin );
				
				DamageProjection();
		}

		void DrawingSurface2D::_AdjustXAxis( double offset, double scale )
		{
				mcxmin = mxmin + mxdist*offset;
				mcxmax = mcxmin + mxdist*scale;

				DamageProjection();
		}

		void DrawingSurface2D::_AdjustYAxis( double offset, double scale )
		{
				mcymin = mymin + ydist*offset;
				mcymax = mcymin + ydist* scale;

				DamageProjection();
		}		
}
