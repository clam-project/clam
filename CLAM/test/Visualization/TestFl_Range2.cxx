#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "Fl_Range.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_GridLayout.hxx"
#include <exception>
#include <iostream>

using namespace CLAMGUI;

class TestWindow : public Fl_Window
{
public:

	TestWindow( int x, int y, int w, int h, const char* l )
		: Fl_Window( x, y, w, h, l )
	{
			mainLayout = new Fl_GridLayout( 10, 10 );
			Fl_Range* range = new Fl_Range( 0,0, 0, 0, FL_HORIZONTAL );
			range->SetRange( -13.0f, 13.0f );
			range->box( FL_EMBOSSED_BOX );
			range->SetFontType( FL_COURIER_BOLD );

			mainLayout->AddMultiCellWidget( range, 0, 8, 7, 1 );

			Fl_Range* range2 = new Fl_Range( 0, 0, 0, 0, FL_VERTICAL );
			range2->SetRange(  -100.0f, 0.0f );
			range2->SetFontType( FL_COURIER_BOLD );

			mainLayout->AddMultiCellWidget( range2, 8,0,1,7 );

			Fl_ZoomSlider* slide = new Fl_ZoomSlider(0,0,0,0, FL_HORIZONTAL );
			
			mainLayout->AddMultiCellWidget( slide, 0, 9, 7, 1 );

			Fl_ZoomSlider* slide2 = new Fl_ZoomSlider( 0, 0, 0, 0, FL_VERTICAL );

			mainLayout->AddMultiCellWidget( slide2, 9, 0, 1, 7 );


			range->SetSpanNotifier( slide->SpanChanged );
			range2->SetSpanNotifier( slide2->SpanChanged );

			mainLayout->end();

			add_resizable( *mainLayout );

			end();
	}

	void resize( int x, int y, int w, int h )
	{
		Fl_Widget::resize( x, y, w, h );

		mainLayout->resize( x, y, w, h );

		redraw();
	}
private:
	Fl_GridLayout* mainLayout;

};

int main( int argc, char** argv )
{

	 	try
		{
				TestWindow* win = new TestWindow( 100, 100, 300, 300, "Test Fl_Range + Fl_ZoomSlider" );
				win->show();

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
