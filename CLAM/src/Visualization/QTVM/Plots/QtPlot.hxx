#ifndef __QTPLOT__
#define __QTPLOT__

#include <string>
#include <qwidget.h>
#include "VMColor.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtPlot : public QWidget
		{	
			public:
				QtPlot(QWidget* parent = 0);
				virtual ~QtPlot();
			
				virtual void Label(const std::string& label)=0;
				virtual void Geometry(int x,int y,int w,int h)=0;
				virtual void Show()=0;
				virtual void Hide()=0;

			protected:
				virtual void closeEvent(QCloseEvent *);
		};
	}
}

#endif
