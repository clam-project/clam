

#ifndef __QT_PROCESSINGCONFIGPRESENTATION_HXX__
#define __QT_PROCESSINGCONFIGPRESENTATION_HXX__

#include <qdialog.h>
#include "Slotv1.hxx"
#include "ProcessingConfigPresentation.hxx"

class QGroupBox;
class QLabel;

namespace NetworkGUI
{

class ActionButton;

class Qt_ProcessingConfigPresentation : public QDialog, public ProcessingConfigPresentation
{
protected:
	QGroupBox* mGroupBox;
//	QPushButton* mOkButton;
//	QPushButton* mCancelButton;
	ActionButton * mOkButton;
	ActionButton * mCancelButton;
	QLabel * mName;

	void Ok( bool );
	void Cancel( bool );

public:
	Qt_ProcessingConfigPresentation( QWidget *parent = 0, const char *name = 0);
	virtual ~Qt_ProcessingConfigPresentation();
	virtual void Show();
	virtual void Hide();

public: // slots
	SigSlot::Slotv1< bool > mOkButtonIsPressed;
	SigSlot::Slotv1< bool > mCancelButtonIsPressed;
};

} // namespace NetworkGUI

#endif // __QT_PROCESSINGCONFIGPRESENTATION_HXX__
