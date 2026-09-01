#include "FocusPropertyFollower.hxx"
#include <QApplication>

FocusPropertyFollower::FocusPropertyFollower(QWidget * parent)
	: QLabel(parent)
{
	connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
		this, SLOT(updateMessageOnFocus()));
}
FocusPropertyFollower::~FocusPropertyFollower() = default;
void FocusPropertyFollower::updateMessageOnFocus()
{
	QWidget * focused = QApplication::focusWidget();
	if (not focused) return;
	setText(focused->whatsThis());
}

