/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __QTCONFIGURATOR__
#define __QTCONFIGURATOR__

#include <map>
#include <string>

#include "Assert.hxx"
#include "Enum.hxx"
#include "DataTypes.hxx"
#include "DynamicType.hxx"

#include <qpushbutton.h>
#include <qwidget.h>
#include <qvbox.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qvalidator.h>
#include <qcombobox.h>

#define HorPos fl_width(name)+5+(mWidgetNum/20)*340
#define VerPos 5+25*(mWidgetNum%20)

namespace CLAM{
	class Visitor {
	public:
		virtual ~Visitor() {}
		virtual void VisitConfig()=0;
	};

	template <typename Configuration, typename Builder>
	class VisitorGetter : public Visitor {
	public:
		VisitorGetter(Configuration * config, Builder* builder) {
			mBuilder = builder;
			mConfig = config;
		}

		virtual ~VisitorGetter() {}
		
		template <typename T>
		void Accept(const char *name, T &value) { 
			mBuilder->AddWidget(name, &value, value); 
		}

		virtual void VisitConfig() {
			mConfig->VisitAll(*this);
		}

	private:
		Builder * mBuilder;
		Configuration * mConfig;
		
	};
	template <typename Configuration, typename Builder>
	class VisitorSetter : public Visitor {
	public:
		VisitorSetter(Configuration * config, Builder* builder) {
			mBuilder = builder;
			mConfig = config;
		}

		virtual ~VisitorSetter() {}
		
		template <typename T>
		void Accept(const char *name, T &value) { 
			mBuilder->RetrieveValue(name, &value, value); 
		}

		virtual void VisitConfig() {
			mConfig->VisitAll(*this);
		}

	private:
		Builder * mBuilder;
		Configuration * mConfig;
		
	};

	class QTConfigurator : public QVBox {
		Q_OBJECT
		typedef QVBox super;
		typedef std::map<std::string, QWidget*> tWidgets;
		typedef std::map<std::string, QTConfigurator*> tSubConfigurators;
	public:
		QTConfigurator(QWidget * parent = NULL, char * name = "Edit the configuration") 
			: super(parent, name)
			
		{
			mWidgetNum = 0;
			mSetter = 0;
			mGetter = 0;
		}

		virtual ~QTConfigurator() {
			if (mSetter) delete mSetter;
			if (mGetter) delete mGetter;
		}

		template <class Config>
		void SetConfig(Config & config) {
			if (mSetter) delete mSetter;
			mSetter = new VisitorSetter<Config,QTConfigurator>(&config, this);
			if (mGetter) delete mGetter;
			mGetter = new VisitorGetter<Config,QTConfigurator>(&config, this);

			setSpacing(3);
			GetInfo();
			
			QFrame * frame = new QFrame(this);
			frame->setMinimumHeight(10);

			QGrid *grid = new QGrid( 2, this );
			
			QPushButton * applyButton = new QPushButton("Apply", grid, "apply");
			connect( applyButton, SIGNAL(clicked()), this, SLOT(Apply()) );

			QPushButton * discardButton = new QPushButton("Discard", grid, "discard");
			connect( discardButton, SIGNAL(clicked()), this, SLOT(Discard()) );
		}
	private:

		void GetInfo() {
			CLAM_ASSERT(mGetter,"Configurator: No config to set");
			mGetter->VisitConfig();
		}
		void SetInfo() {
			CLAM_ASSERT(mSetter,"Configurator: No config to set");
			mSetter->VisitConfig();
		}

		QWidget * GetWidget(const char * name) {
			tWidgets::iterator found=mWidgets.find(name);
			if (found==mWidgets.end()) return NULL;
			return found->second;
		}

	public:
		
		/** Default implementation, do nothing */
		template <typename T>
		void AddWidget(const char *name, void *foo, T& value) {
		}
		/** Default implementation, do nothing */
		template <typename T>
		void RetrieveValue(const char *name, void *foo, T& value) {
		}

		template <typename T>
		void AddWidget(const char *name, std::string *foo, T& value) {
			QHBox * cell = new QHBox(this);
			new QLabel(QString(name), cell);
			QLineEdit * mInput = new QLineEdit(QString(value.c_str()), cell);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, std::string *foo, T& value) {
			QLineEdit * mInput = dynamic_cast<QLineEdit*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=mInput->text().latin1();
		}

		template <typename T>
		void AddWidget(const char *name, TData *foo, T& value) {
			QHBox * cell = new QHBox(this);
			new QLabel(QString(name), cell);
			std::stringstream val;
			val << value << std::ends;
			QLineEdit * mInput = new QLineEdit(QString(val.str().c_str()), cell);
			mInput->setValidator(new QDoubleValidator(mInput));
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, TData *foo, T& value) {
			QLineEdit * mInput = dynamic_cast<QLineEdit*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const char * readValue=mInput->text().latin1();
			std::stringstream s(readValue);
			s >> value;
		}

		template <typename T>
		void AddWidget(const char *name, TSize *foo, T& value) {
			QHBox * cell = new QHBox(this);
			new QLabel(QString(name), cell);
			QSpinBox * mInput = new QSpinBox(cell);
			QIntValidator * validator = new QIntValidator(mInput);
			validator->setBottom(0);
			mInput->setValidator(validator);
			mInput->setValue(value);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, TSize *foo, T& value) {
			QSpinBox * mInput = dynamic_cast<QSpinBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value = mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, bool *foo, T& value) {
			QPushButton * mInput = new QPushButton(name, this);
			mInput->setToggleButton(true);
			mInput->setOn(value);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, bool *foo, T& value) {
			QPushButton * mInput = dynamic_cast<QPushButton*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=mInput->isOn();
		}

		template <typename T>
		void AddWidget(const char *name, Enum *foo, T& value) {
			QHBox * cell = new QHBox(this);
			new QLabel(QString(name), cell);
			QComboBox * mChoice = new QComboBox(/*editable*/false, cell);

			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			for (unsigned i = 0; mapping[i].name; i++) {
				mChoice->insertItem( mapping[i].name );
				if (mapping[i].value==value.GetValue()) mChoice->setCurrentItem(i);
			}
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mChoice));
		}
		template <typename T>
		void RetrieveValue(const char *name, Enum *foo, T& value) {
			QComboBox * mInput = dynamic_cast<QComboBox*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			CLAM_BEGIN_CHECK
				int i;
				for (i = 0; mapping[i].name; i++);
				CLAM_ASSERT(mInput->currentItem()<i, "Configurator: Choice value out of range");
			CLAM_END_CHECK
			value=mapping[mInput->currentItem()].value;
		}

		template <typename T>
		void AddWidget(const char *name, DynamicType *foo, T&value) {
			QHBox * cell = new QHBox(this);
			new QLabel(QString(name), cell);
			QPushButton * mInput = new QPushButton("Details...", cell);
			QTConfigurator * subConfigurator = new QTConfigurator();
			subConfigurator->SetConfig(value);
			mSubConfigurators.insert(tSubConfigurators::value_type(name, subConfigurator));
			connect( mInput, SIGNAL(clicked()), subConfigurator, SLOT(show()) );
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, DynamicType *foo, T&value) {
		}

	public slots:

		void Apply() {
			SetInfo();
		}
		void Discard() {
			close();
		}
	public:
	
		void QTConfigurator::show() {

			super::show();
		}

	private:
		Visitor * mGetter;
		Visitor * mSetter;
		tWidgets mWidgets;
		tSubConfigurators mSubConfigurators;
		int mWidgetNum;

	};
}
#endif//__QTCONFIGURATOR__
