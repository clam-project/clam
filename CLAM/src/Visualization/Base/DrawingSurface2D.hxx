#ifndef __DRAWINGSURFACE2D__
#define __DRAWINGSURFACE2D__

#include "Slotv2.hxx"
#include "Slotv4.hxx"
#include "BoundingBoxes.hxx"

namespace CLAMGUI
{

class DrawingSurface2D
{
public:

		DrawingSurface2D();

		~DrawingSurface2D();


		virtual void EnableIndexedMode() = 0;
		virtual void DisableIndexedMode() = 0;
		virtual bool IsIndexedModeEnabled() const = 0;
		virtual void EnableDoubleBuffering() = 0;
		virtual void DisableDoubleBuffering() = 0;
		virtual bool IsDoubleBufferingEnabled() const = 0;

		bool MustReproject() const
		{
				return mMustReproject;
		}

		void QueryDataBoundBox( DataBoundBox& );

		Slotv4< double, double, double, double>  SetWorldSpace;
		Slotv2< double, double>                  AdjustXAxis;
		Slotv2< double, double>                  AdjustYAxis;

protected:
		void _SetWorldSpace( double xmax, double xmin, double ymax, double ymin );
		void _AdjustXAxis( double offset, double scale );
		void _AdjustYAxis( double offset, double scale );

		void DamageProjection()
		{
				mMustReproject = true;
		}

		virtual void WCSProject() = 0;
		virtual void WindowProject() = 0;
		virtual void DrawContents()=0;
		virtual void Refresh() = 0;
		
private:
		bool   mMustReproject;
		double mxdist, mydist;

protected:

		
		double mxmax, mxmin, mymax, mymin;
		double mcxmax, mcxmin, mcymax, mcymin;
};

}

#endif // DrawingSurface2D.hxx
