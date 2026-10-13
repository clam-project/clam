#include <QLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include "Qt_SMS_Gender_Selector.hxx"
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>

namespace QtSMS
{
	Qt_SMS_Gender_Selector::Qt_SMS_Gender_Selector(QWidget* parent)
		: QWidget(parent)
	{
		QGroupBox* groupBox = new QGroupBox("Select", this);
		groupBox->setFont(QFont("Sans",11));

		QRadioButton* m2f = new QRadioButton(groupBox);
		QRadioButton* f2m = new QRadioButton(groupBox);

		QButtonGroup* buttonGroup = new QButtonGroup(this);
		buttonGroup->setExclusive(true);
		buttonGroup->addButton(m2f, 0);
		buttonGroup->addButton(f2m, 1);

		QBoxLayout* innerLayout = new QVBoxLayout(groupBox);
		innerLayout->setContentsMargins(10, 10, 10, 10);
		innerLayout->setSpacing(1);
		innerLayout->addWidget(m2f);
		innerLayout->addWidget(f2m);

		m2f->setText("Change a male voice into a female one");
		f2m->setText("Change a female voice into a male one");

		m2f->setChecked(true);

		// layout
		QGridLayout* layout = new QGridLayout(this);
		layout->setContentsMargins(10, 10, 10, 10);
		layout->addWidget(groupBox, 1, 0);

		connect(buttonGroup, &QButtonGroup::idClicked, this, &Qt_SMS_Gender_Selector::genderChanged);
	}

	Qt_SMS_Gender_Selector::~Qt_SMS_Gender_Selector()
	{
	}
}

// END

