#include "Fl_Gl_2DSurface.hxx"
#include "CLAMGL.hxx"
#include "FLTKWrapper.hxx"
#include "Assert.hxx"

namespace CLAMGUI
{
	Fl_Gl_2DSurface::Fl_Gl_2DSurface( int X, int Y, int W, int H, const char* label )
				: Fl_Gl_Window( X, Y, W, H, label ), mTimerLaunched( false )
		{
		}

		Fl_Gl_2DSurface::~Fl_Gl_2DSurface()
		{
				FLTKWrapper& tk = dynamic_cast<FLTKWrapper&>(WidgetTKWrapper::GetWrapperFor("FLTK" ));
		
				tk.CancelAsynchronousRefresh( mRefreshSlot );				
		}

		void Fl_Gl_2DSurface::draw( )
		{
				if ( !valid() )
						ResizeGL();

				DrawContents();

				if ( !mTimerLaunched )
				{
						FLTKWrapper& tk = dynamic_cast<FLTKWrapper&>( WidgetTKWrapper::GetWrapperFor("FLTK") );
						mRefreshSlot = tk.RequestAsynchronousRefresh( this );

						mTimerLaunched = true;
				}
		}

		void Fl_Gl_2DSurface::Refresh()
		{
			redraw();
		}

		void Fl_Gl_2DSurface::resize( int X, int Y, int W, int H )
		{
				Fl_Gl_Window::resize(X,Y,W,H);
				invalidate();
				Fl_Gl_Window::damage();
		}

		void Fl_Gl_2DSurface::ResizeGL( )
		{
				glMatrixMode( GL_PROJECTION );
				glViewport( 0, 0, w(), h() );
				glMatrixMode( GL_MODELVIEW );

				DamageProjection();
		}

		void Fl_Gl_2DSurface::InitGL( )
		{		
				mode( FL_RGB | FL_SINGLE );				
		}

		void Fl_Gl_2DSurface::EnableIndexedMode()
		{
				CLAM_ASSERT( can_do( mode()|FL_INDEX), "OpenGL driver doesn't support Indexed Mode!");
				mode( mode()|FL_INDEX );
		}

		void Fl_Gl_2DSurface::DisableIndexedMode()
		{
				mode( mode()&(~FL_INDEX));
		}

		bool Fl_Gl_2DSurface::IsIndexedModeEnabled() const
		{
				return mode()&FL_INDEX;
		}

		void Fl_Gl_2DSurface::EnableDoubleBuffering()
		{
				CLAM_ASSERT( can_do( mode()|FL_DOUBLE), "OpenGL driver doesn't support Indexed Mode!");
				mode( mode()|FL_DOUBLE );
		}
		
		void Fl_Gl_2DSurface::DisableDoubleBuffering()
		{
				mode( mode()&(~FL_DOUBLE));
		}
		
		bool Fl_Gl_2DSurface::IsDoubleBufferingEnabled() const
		{
				return bool(mode()&FL_DOUBLE);
		}

		void Fl_Gl_2DSurface::WCSProject()
		{
				glMatrixMode( GL_PROJECTION );
				glLoadIdentity();
				glOrtho( mcxmin, mcxmax, mcymin, mcymax, -1, 1 );
				glMatrixMode( GL_MODELVIEW );
		}

		void Fl_Gl_2DSurface::WindowProject()
		{
				glMatrixMode( GL_PROJECTION );
				glLoadIdentity();
				glOrtho( 0, w(), 0, h(), -1, 1 );
				glMatrixMode( GL_MODELVIEW );
		}
}
