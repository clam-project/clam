#include <FL/Fl_Group.H>
#include <FL/Fl_Window.H>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Pixmap.H>

#include "Fl_Smart_Tile.H"

static char *image_shade[] = {
"11 11 2 1",
".\tc #000000",
"+\tc #FFFFFF",
"...........",
"...........",
"...........",
"...........",
"...........",
"..+++++++..",
"..+++++++..",
"..+++++++..",
"..+++++++..",
"............",
"............",
};
Fl_Pixmap pixmapShade(image_shade);

static char *image_close[] = {
"11 11 2 1",
".\tc #000000",
"+\tc None",
"...+++++...",
"....+++....",
".....+.....",
"+.........+",
"++.......++",
"+++.....+++",
"++.......++",
"+.........+",
".....+.....",
"....+++....",
"...+++++..."
};
Fl_Pixmap pixmapClose(image_close);

Fl_Smart_Tile::Fl_Smart_Tile(int X,int Y,int W,int H,const char*l)
:Fl_Group(X,Y,W,H,l)
{
	moving_ = -1;	
	closing_ = -1;
	shading_ = -1;
	buttondown_ = 0;
	minsize_ = 50;
}

static int movingnewsize_ = 0; // TODO: this should be a member

int Fl_Smart_Tile::handle(int e)
{
	static int cursor = 0;
	static int prevy = 0;
	
	if (e==FL_MOVE)
	{
		int i;
		for (i=1;i<children();i++)
		{
			if (
				Fl::event_x()<x()+w()-42 &&
				Fl::event_y()>child(i)->y()-21 && Fl::event_y()<child(i)->y())
			{
				moving_ = i;
				closing_ = -1;
				shading_ = -1;
				if (!cursor) {
					window()->cursor(FL_CURSOR_NS);
					cursor = 1;
				}
				return 1;
			}
		}
		moving_ = -1;
		closing_ = -1;
		shading_ = -1;
		if (cursor) {
			window()->cursor(FL_CURSOR_DEFAULT);
			cursor = 0;
		}
		return 0;
	}
	if (e==FL_DRAG)
	{
		if (moving_!=-1)
		{
			movingnewsize_ = child(moving_)->h()-Fl::event_y()+prevy;
			recalc();
			prevy = Fl::event_y();
		}
		if (shading_!=-1)
		{
			if (
				Fl::event_y()>child(shading_)->y()-21 && Fl::event_y()<child(shading_)->y() &&
				Fl::event_x()>=x()+w()-42 && Fl::event_x()<x()+w()-21)
			{
				if (!buttondown_)
				{
					buttondown_ = 1;			
					damage(FL_DAMAGE_CHILD);
				}
			}else{
				if (buttondown_)
				{
					buttondown_ = 0;			
					damage(FL_DAMAGE_CHILD);
				}
			}
		}
		if (closing_!=-1)
		{
			if (
				Fl::event_y()>child(closing_)->y()-21 && Fl::event_y()<child(closing_)->y() &&
				Fl::event_x()>=x()+w()-21 && Fl::event_x()<x()+w())
			{
				if (!buttondown_)
				{
					buttondown_ = 1;			
					damage(FL_DAMAGE_CHILD);
				}
			}else{
				if (buttondown_)
				{
					buttondown_ = 0;			
					damage(FL_DAMAGE_CHILD);
				}
			}
		}
		return 1;
	}
	if (e==FL_RELEASE)
	{
		if (shading_!=-1)
		{
			if (
				Fl::event_y()>child(shading_)->y()-21 && Fl::event_y()<child(shading_)->y() &&
				Fl::event_x()>=x()+w()-42 && Fl::event_x()<x()+w()-21)
			{
				if (child(shading_)->visible())
					child(shading_)->hide();
				else
				{
					child(shading_)->show();
				}
				moving_ = shading_; 
				// to force reuse of the size the child had when it 
				// was hidden

				recalc();
				closing_ = -1;
				shading_ = -1;
			}
		}
		if (closing_!=-1)
		{
			if (
				Fl::event_y()>child(closing_)->y()-21 && Fl::event_y()<child(closing_)->y() &&
				Fl::event_x()>=x()+w()-21 && Fl::event_x()<x()+w())
			{
				Fl_Widget* w = child(closing_);
				remove(w);
				w->do_callback(); // SHOULD THIS CALL THE CALLBACK??
				delete w;	
				closing_ = -1;
				shading_ = -1;
				redraw();
			}
		}
		return 1;
	}
	if (e==FL_PUSH)
	{
		moving_ = -1;
		closing_ = -1;
		shading_ = -1;
		int i;
		for (i=0;i<children();i++)
		{
			if (Fl::event_y()>child(i)->y()-21 && Fl::event_y()<child(i)->y())
			{
				if (i>0 && Fl::event_x()<x()+w()-42)
				{
					moving_ = i;
					prevy = Fl::event_y();
				}
				if (Fl::event_x()>=x()+w()-42)
				{
					if (Fl::event_x()<x()+w()-21)
					{
						shading_ = i;
						buttondown_ = 1;
						damage(FL_DAMAGE_CHILD);
					}else{
						closing_ = i;
						buttondown_ = 1;
						damage(FL_DAMAGE_CHILD);
					}
				}
				return 1;
			}
		}
	
	}
	return Fl_Group::handle(e);
}

int Fl_Smart_Tile::recalc(void)
{
	int ty = y();
	int n = children();
	int rh = h()-n*21;
	int mh = 0;
	int i;
	int m = 0;
	int th = 0;
	int changed = 0;
	int minsize = minsize_;
	
	int *sizes = new int[n];
	
  for (i = 0; i < n; i++)
	{
		sizes[i] = i==moving_ ? movingnewsize_ : child(i)->h();
		if (child(i)->visible())
		{
			th += sizes[i];
			m++;
		}
	}
	
	if (m*minsize>rh)
	{
		minsize = rh/m;
	}
	
	if (th!=rh)
	{	
		int d = rh-th;
		if (moving_==-1)
		{
			while (d&&m)
			{
				int dh = d/m;
				m = 0;
				if (dh==0) dh = d<0 ? -1 : 1;
				for (i=0;i<n && d;i++)
				{
					if (child(i)->visible())
					{
						int ch = sizes[i];
						int nh = ch+dh;
						if (nh<minsize) nh = minsize;
						if (nh!=minsize) m++;
						
						sizes[i] = nh;						
						
						d -= (nh-ch);
					}
				}
			}
		}
		else
		{
			int i = moving_;
			if (sizes[i]<minsize)
			{
				int d2 = sizes[i]-minsize;
				sizes[i]=(minsize);
				i++;
				while (d2!=0 && i!=n)
				{
					if (child(i)->visible())
					{
						int ch = sizes[i];
						int nh = ch+d2;
						if (nh<minsize) nh = minsize;
						sizes[i]=nh;
						d2 -= (nh-ch);
					}
					i++;
				}
				d += d2;
			}
			
			i = moving_;
			
			while (d!=0 && i)
			{
				i--;
				if (child(i)->visible())
				{
					int ch = sizes[i];
					int nh = ch+d;
					if (nh<minsize) nh = minsize;
					sizes[i]=nh;
					d -= (nh-ch);
				}
			}
			i = moving_+1;
			while (d!=0 && i!=n)
			{
				if (child(i)->visible())
				{
					int ch = sizes[i];
					int nh = ch+d;
					if (nh<minsize) nh = minsize;
					sizes[i]=nh;
					d -= (nh-ch);
				}
				i++;
			}
			if (d)
			{
				sizes[moving_] = sizes[moving_]+d;
			}
		}
	}

	ty = y();	

	n = children();

	for (i=0;i<n;i++)
	{
		ty += 21;
		if (ty!=child(i)->y() || sizes[i]!=child(i)->h())
		{
			changed = 1; child(i)->redraw();
		}
		child(i)->resize(x(),ty,w(),sizes[i]);
		if (child(i)->visible())
		{
			ty += sizes[i];
		}
	}
	
	if (changed) damage(FL_DAMAGE_CHILD);

	delete sizes;
	
	return changed;
}

void Fl_Smart_Tile::draw(void)
{
	int i;
	int n = children();
	
	int r = recalc();
	int ty = y();
	
	for (i=0;i<n;i++)
	{
		fl_draw_box(FL_THIN_UP_BOX,x(),ty,w()-42,21,color());
		fl_color(labelcolor());
		fl_font(labelfont(),labelsize());
		fl_draw(child(i)->label(),x()+5,ty,w()-35,21,
			Fl_Align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE));
		fl_draw_box(shading_==i && buttondown_ ? FL_THIN_DOWN_BOX : FL_THIN_UP_BOX,
			x()+w()-42,ty,21,21,color());
		pixmapShade.draw(x()+w()-42+5,ty+5);
		fl_draw_box(closing_==i && buttondown_ ? FL_THIN_DOWN_BOX : FL_THIN_UP_BOX,
			x()+w()-21,ty,21,21,color());
		pixmapClose.draw(x()+w()-21+5,ty+5);
		ty += 21;
		if (child(i)->visible()) ty += child(i)->h();
	}
	
	fl_color(color());
	fl_rectf(x(),ty,w(),h()-(ty-y()));
	Fl_Group::draw();
}
		
