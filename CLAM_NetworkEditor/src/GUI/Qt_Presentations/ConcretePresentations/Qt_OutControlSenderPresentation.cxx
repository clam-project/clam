
#include "Qt_OutControlSenderPresentation.hxx"
#include "Qt_OutControlPresentation.hxx"
#include "OutControlSender.hxx"

#include <qslider.h>
#include <qpainter.h>
#include <iostream> // TODO: remove

namespace NetworkGUI
{

Qt_OutControlSenderPresentation::Qt_OutControlSenderPresentation()
	: mSlider(0)
{
	std::cout << "out control sender presentation" << std::endl;

	mSlider = new QSlider( Vertical, this );
	mSlider->setFixedSize( 20, 50 );
	mSlider->move( 18, 14 );

	connect( mSlider, SIGNAL( valueChanged( int ) ), 
		 this, SLOT( SlotValueChanged( int ) ) );


}

void Qt_OutControlSenderPresentation::UpdateSize()
{
	setFixedSize(56, 76 );
}

void Qt_OutControlSenderPresentation::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	mSlider->setPalette( QPalette( GetColorOfState()));
	p.setBrush( GetColorOfState() );

	QRegion reg(12,7,width()-24, height()-14);
		
	ConnectionPointPresentationsList::iterator it;
	for (it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end();it++)
	{
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*it);
		reg += out->GetRegion();
	}

	setMask(reg);
	p.setPen( QPen( blue, 1 ));
	p.drawRect( 12,7, width()-24,height()-14); // draw a rectangle

	p.setPen( QPen( black,1 ));
	
	if(mSelected)
		DrawSelectedRepresentation();
}

void Qt_OutControlSenderPresentation::ConfigurationUpdated( bool ok )
{
	parentWidget()->setFocus();
	parentWidget()->grabKeyboard();

	CLAM::OutControlSenderConfig * config = dynamic_cast<CLAM::OutControlSenderConfig *>(mConfig->GetConfig());
	CLAM_ASSERT( config, " Qt_OutControlSenderPresentation::ConfigurationUpdated(), config has the wrong concrete type" );
	mMin = config->GetMin();
	mDefault = config->GetDefault();
	mMax = config->GetMax();
	mStep = config->GetStep();
	
	mSlider->setValue( (int)(mDefault/mStep) );
	mSlider->setRange( (int)(mMin/mStep), 
			   (int)(mMax/mStep) +1);

}
void Qt_OutControlSenderPresentation::SlotValueChanged( int value )
{
	std::cout << "value: " << value << std::endl;
	std::cout << "changing value: " << (CLAM::TControlData)( value*mStep ) << std::endl;
	SignalSendOutControlValue.Emit( "out", (CLAM::TControlData)( value*mStep ) );
}


} // namespace NetworkGUI

