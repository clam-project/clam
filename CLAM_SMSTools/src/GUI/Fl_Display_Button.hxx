#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>

#ifndef __FL_DISPLAY_BUTTON__
#define __FL_DISPLAY_BUTTON__

#define FL_CURSOR_EYE (enum Fl_Cursor)65

class Fl_Display_Button : public  Fl_Button {

public:
	Fl_Display_Button( int x, int y, int w, int h, char* label = "" ) : Fl_Button( x, y, w, h, label ) {
	}

	int handle( int event )
	{
		if( active( ) ) {
			if ( event == FL_ENTER  ) {
				fl_cursor( FL_CURSOR_EYE, FL_BLACK, FL_WHITE );			
			}
			else if ( event == FL_LEAVE ) {
				fl_cursor( FL_CURSOR_DEFAULT, FL_BLACK, FL_WHITE );			
			}
		}
		return Fl_Button::handle( event );
	}

};

#endif //__FL_BUTTON__
