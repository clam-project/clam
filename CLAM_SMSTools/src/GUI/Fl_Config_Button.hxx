#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>

#ifndef __FL_CONFIG_BUTTON__
#define __FL_CONFIG_BUTTON__

#define FL_CURSOR_PENCIL (enum Fl_Cursor)44

class Fl_Config_Button : public  Fl_Button {

public:
	Fl_Config_Button( int x, int y, int w, int h, char* label = "" ) : Fl_Button( x, y, w, h, label ) {
		box( FL_NO_BOX );
	}

	int handle( int event )
	{
		if( active( ) ) {
			if ( event == FL_ENTER  ) {
				fl_cursor( FL_CURSOR_PENCIL, FL_BLACK, FL_WHITE );			
			}
			else if ( event == FL_LEAVE  ) {
				fl_cursor( FL_CURSOR_DEFAULT, FL_BLACK, FL_WHITE );			
			}
		}
		return Fl_Button::handle( event );
	}

};

#endif //__FL_BUTTON__
