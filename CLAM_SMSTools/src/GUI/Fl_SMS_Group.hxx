#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>

#ifndef __FL_SMS_GROUP__
#define __FL_SMS_GROUP__

class Fl_SMS_Group : public  Fl_Group {

public:
	Fl_SMS_Group( int x, int y, int w, int h, char* label = "" ) : Fl_Group( x, y, w, h, label ) {
	}

	int handle( int event )
	{
		if ( event == FL_ENTER  ) {
			fl_cursor( FL_CURSOR_DEFAULT, FL_BLACK, FL_WHITE );			
		}
		return Fl_Group::handle( event );
	}

};

#endif //__FL_BUTTON__
