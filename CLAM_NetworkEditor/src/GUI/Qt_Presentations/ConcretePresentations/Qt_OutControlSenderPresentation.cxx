
#include "Qt_OutControlSenderPresentation.hxx"
#include "Qt_OutControlPresentation.hxx"
#include "OutControlSender.hxx"
#include <cmath>

#include <qslider.h>
#include <qpainter.h>
#include <iostream> // TODO: remove

namespace NetworkGUI
{

Qt_OutControlSenderPresentation::Qt_OutControlSenderPresentation()
	: mSlider(0)
{
	mSlider = new QSlider( Vertical, this );
	mSlider->setMinimumSize( 20, 50 );
	mSlider->resize(20, 50 );
	mSlider->move( 18, 14 );

	connect( mSlider, SIGNAL( valueChanged( int ) ), 
		 this, SLOT( SlotValueChanged( int ) ) );


}

void Qt_OutControlSenderPresentation::UpdateSize( bool hasToResize)
{
	setMinimumSize(56, 76 );
	if(hasToResize)
		resize(56,76);
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

	CLAM::OutControlSenderConfig * config = dynamic_cast<CLAM::OutControlSenderConfig *>(mConfig->GetConfig());
	CLAM_ASSERT( config, " Qt_OutControlSenderPresentation::ConfigurationUpdated(), config has the wrong concrete type" );
	mMin = config->GetMin();
	mDefault = config->GetDefault();
	mMax = config->GetMax();
	mStep = config->GetStep();

	mSlider->setRange( (int)(round(mMin/mStep)),
			   (int)(round(mMax/mStep)) );
	
	mSlider->setValue( (int)(mDefault/mStep) );
	SignalSendOutControlValue.Emit( "out", mDefault );

}
void Qt_OutControlSenderPresentation::SlotValueChanged( int value )
{
	SignalSendOutControlValue.Emit( "out", (CLAM::TControlData)( value*mStep ) );
}

void Qt_OutControlSenderPresentation::ExecuteResize( const QPoint & difference )
{	
	QRect newGeometry = geometry();
	switch( mResizePosition )
	{
		case UpLeft:
			newGeometry.setTopLeft( difference + pos() );
			break;
		case Up:
			newGeometry.setTop( difference.y() + pos().y() );
			break;
		case UpRight:
			newGeometry.setTopRight( difference + pos() );	
			newGeometry.setWidth( difference.x() + width() );
			break;
		case DownLeft:
			newGeometry.setBottomLeft( difference + pos() );
			newGeometry.setHeight( difference.y() + height() );
			break;
		case Down:
			newGeometry.setHeight( difference.y() + height() );
			break;
		case DownRight:
			newGeometry.setWidth( difference.x() + width() );
			newGeometry.setHeight( difference.y() + height() );
			break;
	}
	setGeometry( newGeometry );
	//QRect sliderGeometry = newGeometry;
	//sliderGeometry.setWidth( width() - 24 );
	//sliderGeometry.setHeight( height() - 14 );
	mSlider->resize( geometry().width() - 36, geometry().height() - 28 );
	std::cout << "slider measures: " << mSlider->width() << "  " << mSlider->height() << std::endl;
	UpdateOutControlsPosition();
}

} // namespace NetworkGUI

