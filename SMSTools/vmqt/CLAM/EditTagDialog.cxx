#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <CLAM/EditTagDialog.hxx>

namespace CLAM
{
    namespace VM
    {
		EditTagDialog::EditTagDialog(const QString& contents, QWidget* parent)
			: QDialog(parent)
		{
			setWindowTitle("Edit tag");
			InitEditTagDialog(contents);
		}

		EditTagDialog::~EditTagDialog()
		{
		}

		QString EditTagDialog::text() const
		{
			return mLineEdit->text();
		}

		void EditTagDialog::InitEditTagDialog(const QString& contents)
		{
			// main layout
			QBoxLayout* mainLayout = new QVBoxLayout(this);
			mainLayout->setContentsMargins(5, 5, 5, 5);
			mainLayout->addStretch();

			// top: label and lineedit
			QBoxLayout* topLayout = new QHBoxLayout;
			mainLayout->addLayout(topLayout);
			mLabel = new QLabel(this);
			mLabel->setText("Tag");
			mLineEdit = new QLineEdit(contents,this);
			topLayout->addWidget(mLabel);
			topLayout->addWidget(mLineEdit);

			mainLayout->addStretch();

			// bottom: ok and cancel buttons
			QBoxLayout* bottomLayout = new QHBoxLayout;
			mainLayout->addLayout(bottomLayout);
			mOkButton = new QPushButton(this);
			mOkButton->setText("Ok");
			mOkButton->setDefault(true);
			mCancelButton = new QPushButton(this);
			mCancelButton->setText("Cancel");
			bottomLayout->addWidget(mOkButton);
			bottomLayout->addStretch();
			bottomLayout->addWidget(mCancelButton);
	    
			// connections
			connect(mOkButton,SIGNAL(clicked()),this,SLOT(accept()));
			connect(mCancelButton,SIGNAL(clicked()),this,SLOT(reject()));
		}
    }
}

// END

