#include "AboutBox.hxx"

namespace QtSMS
{	
	AboutBox::AboutBox(QWidget* parent)
		: QDialog(parent)
	{
		setupUi(this);
		// QtSMSTools::showAboutBox() allocates the box with new and never deletes it
		setAttribute(Qt::WA_DeleteOnClose);
	}
		
	AboutBox::~AboutBox()
	{
	}
}

// END

