#include <qlayout.h>
#include <qpushbutton.h>
#include <qscrollbar.h>
#include <qframe.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qwt/qwt_scale.h>
#include "IconData.hxx"
#include "PlotController.hxx"
#include "DisplaySurface.hxx"
#include "VZLabel.hxx"
#include "QtPlotter.hxx"
#include "QtPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtPlot::QtPlot(QWidget* parent) : QWidget(parent)
		{
			Init();
		}
		
		QtPlot::~QtPlot()
		{
		}

		void QtPlot::SetBackgroundColor(Color c)
		{
			double r = double(c.r)/255.0;
			double g = double(c.g)/255.0;
			double b = double(c.b)/255.0;

			_surf->SetBackgroundColor(r,g,b);
		}

		void QtPlot::SetController(PlotController* controller)
		{
			
			_controller = controller;
			_surf->SetController(_controller);
			
			// vertical scroll and zoom ratio management 
			connect(_controller,SIGNAL(vZoomRatio(int)),this,SLOT(receivedVZoomRatio(int)));
			connect(_controller,SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));
			connect(_controller,SIGNAL(vScrollValue(int)),this,SLOT(updateVScroll(int)));

			// horizontal scroll and zoom ratio management
			connect(_controller,SIGNAL(hZoomRatio(int)),this,SLOT(receivedHZoomRatio(int)));
			connect(_controller,SIGNAL(hScrollMaxValue(int)),this,SLOT(setMaxHScroll(int)));
			connect(_controller,SIGNAL(hScrollValue(int)),this,SLOT(updateHScroll(int)));

			// toggle button to change display colors automatically
			connect(_btoggle_color,SIGNAL(clicked()),this,SLOT(switchColors()));
		}

		void QtPlot::Init()
		{
			_controller = NULL;

			QFont f0("Sans",7);
			QFont f1("Sans",8);
			f0.setBold(true);
			f1.setBold(true);

			_mainLayout = new QVBoxLayout(this);
			_mainLayout->setMargin(2);
			_mainLayout->setSpacing(0);

			// top area (x ruler)
			_top = new QHBoxLayout(_mainLayout);
			QFrame* topLeftHole = new QFrame(this);
			topLeftHole->setFixedSize(50,40);
			_btoggle_color = new QPushButton(topLeftHole);
			_btoggle_color->setFixedSize(25,25);
			_btoggle_color->setGeometry(12,8,25,25);
			_btoggle_color->setToggleButton(true);
			_btoggle_color->setText("CM");
			QToolTip::add(_btoggle_color,"switch display colors");
			SwitchDisplayColors(false);
			_top->addWidget(topLeftHole,0);
			_xRuler = new QwtScale(QwtScale::Top,this,"top ruler");
		    _xRuler->setFont(f0);
			_xRuler->setFixedHeight(40);
			QwtScaleDiv div;
			div.rebuild(0.0,10,10,10,false,10.0);
			_xRuler->setScaleDiv(div);
			_top->addWidget(_xRuler);
			QFrame* topRightHole = new QFrame(this);
			topRightHole->setFixedSize(20,40);
			_top->addWidget(topRightHole,0);
			
			// middle area (y ruler, display area, vertical scroll and zoom group)
			_middle = new QHBoxLayout(_mainLayout);
			_yRuler = new QwtScale(QwtScale::Left,this,"left ruler");
			_yRuler->setMinimumWidth(50);
			_yRuler->setFont(f0);
			div.rebuild(-1.0,1.0,1,0,false,1.0);
			_yRuler->setScaleDiv(div);
			_middle->addWidget(_yRuler);
			
			_surf = new DisplaySurface(this);
			_surf->setMinimumSize(200,20);
			_middle->addWidget(_surf);
			
			QPixmap icon_zoomin((const char**)icon_zoomin);
			QPixmap icon_zoomout((const char**)icon_zoomout);

			QVBoxLayout* right = new QVBoxLayout(_middle);

			_vzOut = new QPushButton(this);
			_vzOut->setAutoRepeat(true);
			_vzOut->setFixedSize(20,20);
			_vzOut->setPixmap(icon_zoomout);
			QToolTip::add(_vzOut,"Zoom Out");
			right->addWidget(_vzOut,0);

			_vzIn = new QPushButton(this);
			_vzIn->setAutoRepeat(true);
			_vzIn->setFixedSize(20,20);
			_vzIn->setPixmap(icon_zoomin);
			QToolTip::add(_vzIn,"Zoom In");
			right->addWidget(_vzIn,0);

			_vzLabel = new VZLabel(this);
			right->addWidget(_vzLabel);

			_vsb = new QScrollBar(QScrollBar::Vertical,this);
			_vsb->setFixedWidth(_vsb->sizeHint().width());
			_vsb->setRange(0,0);
			_vsb->setSteps(20,100);
			right->addWidget(_vsb,1);

			// bottom area (horizontal scroll and zoom group)
			_bottom = new QHBoxLayout(_mainLayout);
			QFrame* bottomLeftHole = new QFrame(this);
			bottomLeftHole->setFixedSize(50,20);
			_bottom->addWidget(bottomLeftHole,0);

			_hsb = new QScrollBar(QScrollBar::Horizontal,this);
			_hsb->setFixedHeight(_hsb->sizeHint().height());
			_hsb->setRange(0,0);
			_hsb->setSteps(20,100);
			_bottom->addWidget(_hsb,1);

			_hzLabel = new QLabel(this);
			_hzLabel->setMinimumSize(30,20);
			_hzLabel->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
			_hzLabel->setFont(f1);
			_hzLabel->setAlignment(Qt::AlignCenter);
			QToolTip::add(_hzLabel,"Zoom Ratio");
			_bottom->addWidget(_hzLabel);

			_hzIn = new QPushButton(this);
			_hzIn->setAutoRepeat(true);
			_hzIn->setFixedSize(20,20);
			_hzIn->setPixmap(icon_zoomin);
			QToolTip::add(_hzIn,"Zoom In");
			_bottom->addWidget(_hzIn,0);

			_hzOut = new QPushButton(this);
			_hzOut->setAutoRepeat(true);
			_hzOut->setFixedSize(20,20);
			_hzOut->setPixmap(icon_zoomout);
			QToolTip::add(_hzOut,"Zoom Out");
			_bottom->addWidget(_hzOut,0);

			QFrame* bottomRightHole = new QFrame(this);
			bottomRightHole->setFixedSize(20,20);
			_bottom->addWidget(bottomRightHole,0);

			// Connections vertical scroll and zoom
			connect(_vzIn,SIGNAL(clicked()),this,SLOT(vZoomIn()));
		    connect(_vzOut,SIGNAL(clicked()),this,SLOT(vZoomOut()));
			connect(_vsb,SIGNAL(valueChanged(int)),this,SLOT(vScrollValue(int)));

			// Connections horizontal scroll and zoom group
			connect(_hzIn,SIGNAL(clicked()),this,SLOT(hZoomIn()));
		    connect(_hzOut,SIGNAL(clicked()),this,SLOT(hZoomOut()));
			connect(_hsb,SIGNAL(valueChanged(int)),this,SLOT(hScrollValue(int)));
		}

		void QtPlot::hZoomIn()
		{
			if(_controller) _controller->HZoomIn();
		}

		void QtPlot::hZoomOut()
		{
			if(_controller) _controller->HZoomOut();
		}

		void QtPlot::receivedHZoomRatio(int zr)
		{
			// update horizontal zoom ratio indicator
			QString s;
			s = "1:"+(s.setNum(zr));
			_hzLabel->setText(s);
		}

		void QtPlot::hScrollValue(int value)
		{
			if(_controller)
			{
				_controller->UpdateHViewport(value);
			}
		}

		void QtPlot::setMaxHScroll(int value)
		{
			if(value > 0)
			{
				_hsb->setMaxValue(value-_surf->width());
			}
		}

		void QtPlot::updateHScroll(int value)
		{
			_hsb->setValue(value);
		}

		void QtPlot::vZoomIn()
		{
			if(_controller) _controller->VZoomIn();
		}

		void QtPlot::vZoomOut()
		{
			if(_controller) _controller->VZoomOut();
		}

		void QtPlot::receivedVZoomRatio(int zr)
		{
			// update vertical zoom ratio indicator
			QString s;
			s = "1:"+(s.setNum(zr));
			_vzLabel->setText(s);
		}

		void QtPlot::setMaxVScroll(int value)
		{
			if(value > 0)
			{
				_vsb->setMaxValue(value-_surf->height());
			}
		}

		void QtPlot::vScrollValue(int value)
		{
			if(_controller)
			{
				_controller->UpdateVViewport(value);
			}
		}

		void QtPlot::updateVScroll(int value)
		{
			_vsb->setValue(value);
		}

		void QtPlot::AddToMainLayout(QLayout* layout)
		{
			_mainLayout->addLayout(layout);
		}

		void QtPlot::setXRulerScaleDiv(QwtScaleDiv div)
		{
			_xRuler->setScaleDiv(div);
		}

		void QtPlot::setYRulerScaleDiv(QwtScaleDiv div)
		{
			_yRuler->setScaleDiv(div);
		}

		void QtPlot::SetXRulerLabelFormat(char f,int prec,int fieldWidth)
		{
			_xRuler->setLabelFormat(f,prec,fieldWidth);
		}

		void QtPlot::SetYRulerLabelFormat(char f,int prec,int fieldWidth)
		{
			_yRuler->setLabelFormat(f,prec,fieldWidth);
		}

		void QtPlot::Label(const std::string& label)
		{
			setCaption(label.c_str());
		}

		void QtPlot::Geometry(int x,int y,int w,int h)
		{
			setGeometry(x,y+18,w,h);
		}

		void QtPlot::Show()
		{
			show();
		}

		void QtPlot::Hide()
		{
			hide();
		}

		void QtPlot::closeEvent(QCloseEvent *e)
		{
			QtPlotter::Remove(this);
			e->accept();
		}

		void QtPlot::SwitchDisplayColors(bool b)
		{
			(b) ? _btoggle_color->show() : _btoggle_color->hide();
		}

		void QtPlot::switchColors()
		{
			if(_btoggle_color->isOn())
			{
				DisplayBackgroundBlack();
			}
			else
			{
				DisplayBackgroundWhite();	
			}
			_surf->updateGL();
		}
	}
}

// END
