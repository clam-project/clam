#ifndef __QTPLOT__
#define __QTPLOT__

#include <string>
#include <qwidget.h>
#include <qwt/qwt_scldiv.h>
#include "VMColor.hxx"

class QLayout;
class QBoxLayout;
class QScrollBar;
class QPushButton;
class QLabel;

class QwtScale;

namespace CLAM
{
	namespace VM
	{
		class PlotController;
		class DisplaySurface;
		class VZLabel;
		
		/*
		* This class implements a common presentation. 
		* A view with: 
		*	- surface 
		*   - scroll bars
		*   - rulers: horizontal/vertical 
		*   - zoom controls: in/out 
		*/ 
		class QtPlot : public QWidget
		{
			Q_OBJECT
			
			public:
				QtPlot(QWidget* parent = 0);
				~QtPlot();
			
				void SetBackgroundColor(Color c);
				void Label(const std::string& label);
				void Geometry(int x,int y,int w,int h);
				void Show();
				void Hide();

				void SwitchDisplayColors(bool b);

			private slots:
				void hZoomIn();
				void hZoomOut();
				void receivedHZoomRatio(int);
				void setMaxHScroll(int);
				void hScrollValue(int);
				void updateHScroll(int);

				void vZoomIn();
				void vZoomOut();
				void receivedVZoomRatio(int);
				void setMaxVScroll(int);
				void vScrollValue(int);
				void updateVScroll(int);

				void switchColors();

			protected slots:
				void setXRulerScaleDiv(QwtScaleDiv);
				void setYRulerScaleDiv(QwtScaleDiv);

			protected:
				PlotController* _controller;
				
				void SetController(PlotController* controller);

				void AddToMainLayout(QLayout* layout);

				void SetXRulerLabelFormat(char f,int prec,int fieldWidth);
				void SetYRulerLabelFormat(char f,int prec,int fieldWidth);

				virtual void closeEvent(QCloseEvent *);

				virtual void SetPlotController()=0;
				virtual void Connect()=0;

				virtual void DisplayBackgroundBlack()=0;
				virtual void DisplayBackgroundWhite()=0;

			private:
				QBoxLayout *_mainLayout,*_top,*_middle,*_bottom;
				QwtScale *_xRuler,*_yRuler;
			    DisplaySurface* _surf;

				QScrollBar* _hsb;
				QLabel* _hzLabel;
				QPushButton *_hzIn, *_hzOut;
				QPushButton* _btoggle_color;

				QScrollBar* _vsb;
				VZLabel* _vzLabel;
				QPushButton *_vzIn, *_vzOut;

				void Init();
		};
	}
}

#endif
