
#include "Qt_ProcessingConfigPresentation.hxx"
#include "ActionButton.hxx"
#include <qlabel.h>
#include <qgroupbox.h>  

#include <iostream>

namespace NetworkGUI
{

Qt_ProcessingConfigPresentation::Qt_ProcessingConfigPresentation( QWidget *parent , const char *name )
	: QDialog(parent, name)
{
	if ( !name )
		setName( "ProcessingConfigDesigner" );
	
	mGroupBox = new QGroupBox( this, "mGroupBox" );
	mGroupBox->setGeometry( QRect( 10, 230, 190, 40 ) );
	
	mOkButton = new ActionButton( mGroupBox, "mOkButton" );
	mOkButton->setGeometry( QRect( 20, 10, 70, 20 ) );
	
	mCancelButton = new ActionButton( mGroupBox, "mCancelButton" );
	mCancelButton->setGeometry( QRect( 100, 10, 70, 20 ) );
	
	mName = new QLabel( this, "mName" );
	mName->setGeometry( QRect( 10, 10, 180, 33 ) );

	setCaption( tr( "Processing Config" ) );
	mGroupBox->setTitle( QString::null );
	mOkButton->setText( tr( "Ok" ) );
	mCancelButton->setText( tr( "Cancel" ) );
	mName->setText( QString::null );

	resize( QSize(209, 275).expandedTo(minimumSizeHint()) );

	mOkButtonIsPressed.Wrap( this, &Qt_ProcessingConfigPresentation::Ok );
	mCancelButtonIsPressed.Wrap( this, &Qt_ProcessingConfigPresentation::Cancel );

	mOkButton->Pressed.Connect( mOkButtonIsPressed );
	mCancelButton->Pressed.Connect( mCancelButtonIsPressed );

}
	
Qt_ProcessingConfigPresentation::~Qt_ProcessingConfigPresentation()
{
}

void Qt_ProcessingConfigPresentation::Ok( bool )
{
	std::cout << "ok is pressed" << std::endl;
	ApplyConfig.Emit( mConfig );
	close();
}

void Qt_ProcessingConfigPresentation::Cancel( bool )
{
	std::cout << "cancel is pressed" << std::endl;
	close();
}

void Qt_ProcessingConfigPresentation::Show()
{
	show();
}

void Qt_ProcessingConfigPresentation::Hide()
{
	hide();
}


} // namespace NetworkGUI
