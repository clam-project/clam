#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "Fl_Range.hxx"
#include "Fl_ZoomSlider.hxx"
#include <exception>
#include <iostream>

using namespace CLAMGUI;

int main( int argc, char** argv )
{

	 	try
		{
				Fl_Window* win = new Fl_Window( 100, 100, 300, 20, "Horizontal Ruler" );
				Fl_Range* range = new Fl_Range( 0,0, 300, 20, FL_HORIZONTAL );
				range->SetRange( 0.0f, 1000.0f );
				range->SetSpan( 0.0f, 1000.0f );
				range->SetFontType( FL_COURIER_BOLD );
				win->resizable(range);
				win->show();
				
				Fl_Window* win2 = new Fl_Window( 450, 100, 20, 300, "Vertical Ruler" );
				Fl_Range* range2 = new Fl_Range( 0, 0, 20, 300, FL_VERTICAL );
				range2->SetRange(  0.0f, -100.0f );
				range2->SetSpan(  0.0f, -100.0f );
				win2->resizable( range2 );
				win2->show();

				Fl_Window* win3 = new Fl_Window( 100, 150, 300, 20, "Horizontal zoomslider" );
				Fl_ZoomSlider* slide = new Fl_ZoomSlider(0,0,300,20, FL_HORIZONTAL );
				win3->resizable( slide );
				win3->show();

				Fl_Window* win4 = new Fl_Window( 575, 100, 20, 300, "Vertical zoomslider" );
				Fl_ZoomSlider* slide2 = new Fl_ZoomSlider( 0, 0, 20, 300, FL_VERTICAL );
				win4->resizable( slide2 );
				win4->show();

				range->SetSpanNotifier( slide->SpanChanged );
				range2->SetSpanNotifier( slide2->SpanChanged );

				Fl::run();
		}
		catch ( std::exception& e )
		{
				std::cout << "Standard library exception caught" << std::endl;
				std::cout << "what?: " << e.what() << std::endl;
		}
		catch( ... )
		{
				std::cout << "Unexpected exception caught" << std::endl;
				abort();
		}

		return 0;

}
