
#ifndef __Qt_OutControlSenderPresentation_hxx__
#define __Qt_OutControlSenderPresentation_hxx__

#include "Qt_ProcessingPresentation.hxx"
#include "DataTypes.hxx"

class QSlider;

namespace NetworkGUI
{

class Qt_OutControlSenderPresentation : public Qt_ProcessingPresentation
{
	Q_OBJECT

	QSlider * mSlider;
public:
	Qt_OutControlSenderPresentation();
	void UpdateSize( bool hasToResize = true );
protected:
	void ConfigurationUpdated( bool ok );
	void ExecuteResize( const QPoint & difference );
	void paintEvent( QPaintEvent * );

	CLAM::TControlData mMin;
	CLAM::TControlData mMax;
	CLAM::TControlData mStep;
	CLAM::TControlData mDefault;
public slots:
	void SlotValueChanged( int );
};
	
} // namespace NetworkGUI

#endif //__Qt_OutControlSenderPresentation_hxx__  
