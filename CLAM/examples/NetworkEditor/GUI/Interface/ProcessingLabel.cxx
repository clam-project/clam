
#include "ProcessingLabel.hxx"
#include <iostream>
#include "Oscillator.hxx"
#include "FactoryToolBox.hxx"
#include <qframe.h>
#include <qlineedit.h>

namespace NetworkGUI
{

ProcessingLabel::ProcessingLabel(   const std::string & name, QWidget *parent )
	: QLabel( parent , "processing label" ),
	  mName( name )
{
	setPalette( QPalette( QColor( 255, 255, 255 )));
	resize(130,30);
	setFont(QFont( "Verdana", 11));
	setFrameStyle( QFrame::Panel | QFrame::Sunken );
	setAlignment( AlignCenter );
	setText(QString(name.c_str()));	
}

ProcessingLabel::~ProcessingLabel()
{
}

void ProcessingLabel::mouseDoubleClickEvent( QMouseEvent *)
{
	std::cout << "creat nou " << mName << std::endl;
	setPalette( QPalette( QColor( 150, 150, 250 )));
	FactoryToolBox* parent = (FactoryToolBox*)parentWidget();

	std::stringstream name;
	name.str("");
	name << mName;
	name << "_" << parent->GetNumProcessings();
	parent->IncrementNumProcessings();
	parent->AddNewProcessing.Emit(name.str(), new CLAM::Oscillator);

}

void ProcessingLabel::mouseReleaseEvent( QMouseEvent *)
{
	setPalette( QPalette( QColor( 255, 255, 255 )));
}

} // namespace NetworkGUI
