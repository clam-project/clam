#include <FL/Fl_Double_Window.H>

class PresentationWindow:public Fl_Double_Window
{
protected:
	friend class Presentation;
	
	Presentation* mPresentation;

	PresentationWindow(Presentation* p,int x,int y,int w,int h,char* l)
	:Fl_Double_Window(x,y,w,h)
	{
		mPresentation = p;
	}
public:
	~PresentationWindow()
	{
	}
public:
	Presentation* GetPresentation(void)
	{
		return mPresentation;
	}
};
