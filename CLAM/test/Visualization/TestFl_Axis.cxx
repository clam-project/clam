#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "Fl_X_Axis.hxx"
#include "Fl_Y_Axis.hxx"
#include "Fl_ZoomSlider.hxx"
#include "Fl_GridLayout.hxx"

#include <exception>
#include <iostream>

using namespace CLAMVM;

class TestWindow : public Fl_Window
{
public:

	TestWindow( int x, int y, int w, int h, const char* l )
		: Fl_Window( x, y, w, h, l )
	{
			mainLayout = new Fl_GridLayout( 14, 13 );
			Fl_X_Axis* range = new Fl_X_Axis( 0,0,0,0, "Frequency [Hz]" );
			range->align( FL_ALIGN_BOTTOM );
			range->scale( CA_LIN );
			range->minimum( 1 );
			range->maximum( 11025.0f );
			range->label_format( "%g" );
			range->label_step(10);
			range->label_size( 9 );
			range->axis_color(FL_BLACK);
			range->axis_align(CA_BOTTOM|CA_LINE);

			mainLayout->AddMultiCellWidget( range, 1, 12, 9, 1 );
			mainLayout->MakeHeightFixed();

			Fl_Y_Axis* range2 = new Fl_Y_Axis( 0,0,0,0, "Amplitude [dB]" );
			range2->align( FL_ALIGN_LEFT);
			range2->scale( CA_LIN );
			range2->minimum( -1.0f );
			range2->maximum( 1.0f );
			range2->label_format( "%g" );
			range2->label_step(10);
			range2->label_size( 9 );
			range2->axis_color(FL_BLACK);
			range2->axis_align(CA_RIGHT|CA_LINE);

			mainLayout->AddMultiCellWidget( range2, 11 ,1,1,10 );
			mainLayout->MakeWidthFixed();

			Fl_ZoomSlider* slide = new Fl_ZoomSlider(0,0,0,0, FL_HORIZONTAL );
			
			mainLayout->AddMultiCellWidget( slide, 1, 13, 9, 1 );
			mainLayout->MakeHeightFixed();

			Fl_ZoomSlider* slide2 = new Fl_ZoomSlider( 0, 0, 0, 0, FL_VERTICAL );

			mainLayout->AddMultiCellWidget( slide2, 12, 1, 1, 10 );
			mainLayout->MakeWidthFixed();

			slide->SpanChanged.Connect( range->AdjustRange );
			slide2->SpanChanged.Connect( range2->AdjustRange );

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
				TestWindow* win = new TestWindow( 100, 100, 640, 600, "Test Fl_Range + Fl_ZoomSlider" );
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
