#include <QLayout>
#include <QLabel>
#include <QTimer>
#include <QCloseEvent>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <CLAM/WaitMsgIndicator.hxx>
#include <CLAM/QtWaitMessageImpl.hxx>

namespace CLAM
{
	namespace VM
	{
		QtWaitMessageImpl::QtWaitMessageImpl(QWidget * parent)
			: QDialog(parent)
		{
			mLabel = new QLabel(this);
			mIndicator = new WaitMsgIndicator(this);
			mIndicator->setFixedHeight(25);

			QBoxLayout* layout = new QVBoxLayout(this);
			layout->setContentsMargins(15, 15, 15, 15);
			layout->setSpacing(10);

			layout->addWidget(mLabel);
			layout->addWidget(mIndicator);
		}

		QtWaitMessageImpl::~QtWaitMessageImpl()
		{
		}

		void QtWaitMessageImpl::LabelText(const std::string& label)
		{
			mLabel->setText(label.c_str());
		}

		// Qt3 close events started out ignored; since Qt4 they are accepted by
		// default, so the dialog must refuse them explicitly while the task runs.
		void QtWaitMessageImpl::closeEvent(QCloseEvent* e)
		{
			e->ignore();
		}

		// Keep Esc (QDialog::keyPressEvent -> reject()) from dismissing it too.
		void QtWaitMessageImpl::reject()
		{
		}
	}
}

// END

