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

#ifndef __FLTK_GUI_BUILDER__
#define __FLTK_GUI_BUILDER__

#include <map>
#include <string>

#include "Assert.hxx"
#include "Enum.hxx"
#include "DataTypes.hxx"

#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Menu_Item.H>

#define HorPos fl_width(name)+5+(mWidgetNum/20)*340
#define VerPos 5+25*(mWidgetNum%20)

namespace CLAM{
	template <typename Configuration>
	class FLTKConfigurator : public Fl_Window {
		typedef std::map<std::string, Fl_Widget*> tWidgets;
	public:
		FLTKConfigurator() 
			: Fl_Window(345, 40, "Edit the configuration")
			
		{
			mWidgetNum = 0;
			mConfig = 0;
		}

		virtual ~FLTKConfigurator() {}

		/** @todo Clear previously related configs */
		void SetConfig(Configuration & config) {
			mConfig =&config;
		}

		void GetInfo() {
			CLAM_ASSERT(mConfig,"Configurator: Config not set")
			VisitorGetter getter(this);
			mConfig->VisitAll(getter);
		}
		void SetInfo() {
			CLAM_ASSERT(mConfig,"Configurator: Config not set")
			VisitorSetter setter(this);
			mConfig->VisitAll(setter);
			mConfig->Debug();
		}

		Fl_Widget * GetWidget(const char * name) {
			tWidgets::iterator found=mWidgets.find(name);
			if (found==mWidgets.end()) return NULL;
			return found->second;
		}

		
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
			fl_font(FL_HELVETICA,12);
			Fl_Input * mInput = new Fl_Input(HorPos, VerPos, 330-fl_width(name), 20);
			mInput->label( name );
			mInput->value( value.c_str() );
			mInput->labelsize(12);
			mInput->textsize(12);
			mInput->align(FL_ALIGN_LEFT);
			add(*mInput);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, std::string *foo, T& value) {
			Fl_Input * mInput = dynamic_cast<Fl_Input*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=mInput->value();
		}

		template <typename T>
		void AddWidget(const char *name, TData *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Float_Input * mInput = new Fl_Float_Input(HorPos, VerPos, 330-fl_width(name), 20);

			std::stringstream val;
			val << value << std::ends;
			mInput->value( val.str().c_str() );

			mInput->label( name );
			mInput->labelsize(12);
			mInput->align(FL_ALIGN_LEFT);
			add(*mInput);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, TData *foo, T& value) {
			Fl_Float_Input * mInput = dynamic_cast<Fl_Float_Input*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const char * readValue=mInput->value();
			std::stringstream s(readValue);
			s >> value;
		}

		template <typename T>
		void AddWidget(const char *name, TSize *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Int_Input * mInput = new Fl_Int_Input(HorPos, VerPos, 330-fl_width(name), 20, name);

			std::stringstream val;
			val << value << std::ends;
			mInput->value( val.str().c_str() );

			mInput->labelsize(12);
			mInput->align(FL_ALIGN_LEFT);
			add(*mInput);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		void RetrieveValue(const char *name, TSize *foo, T& value) {
			Fl_Int_Input * mInput = dynamic_cast<Fl_Int_Input*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const char * readValue=mInput->value();
			std::stringstream s(readValue);
			s >> value;
		}

		template <typename T>
		void AddWidget(const char *name, bool *foo, T& value) {
			Fl_Light_Button * mButton = new Fl_Light_Button(70+(mWidgetNum/20)*340, VerPos, 200, 20);
			mButton->label( name );
			mButton->labelsize(12);
			mButton->value( value );
			add(*mButton);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mButton));
		}
		template <typename T>
		void RetrieveValue(const char *name, bool *foo, T& value) {
			Fl_Light_Button * mInput = dynamic_cast<Fl_Light_Button*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			value=(mInput->value()!=0);
		}

		template <typename T>
		void AddWidget(const char *name, Enum *foo, T& value) {
			fl_font(FL_HELVETICA,12);

			Fl_Choice* mChoice = new Fl_Choice(HorPos, VerPos, 330-fl_width(name), 20);
			mChoice->label( name );
			mChoice->labelsize(12);
			mChoice->textsize(12);
			mChoice->align(FL_ALIGN_LEFT);

			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			for (unsigned i = 0; mapping[i].name; i++) {
				mChoice->add( mapping[i].name );
				if (mapping[i].value==value.GetValue()) mChoice->value(i);
			}
			add(*mChoice);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mChoice));
		}
		template <typename T>
		void RetrieveValue(const char *name, Enum *foo, T& value) {
			Fl_Choice * mInput = dynamic_cast<Fl_Choice*>(GetWidget(name));
			CLAM_ASSERT(mInput,"Configurator: Retrieving a value/type pair not present");
			const Enum::tEnumValue * mapping = value.GetSymbolMap();
			CLAM_BEGIN_CHECK
				int i;
				for (i = 0; mapping[i].name; i++);
				CLAM_ASSERT(mInput->value()<i, "Configurator: Choice value out of range");
			CLAM_END_CHECK
			value=mapping[mInput->value()].value;
		}

#if 0 // SubConfigs still not supported
		template <typename T>
		virtual void Accept(const char *name, DynamicType *foo, T&value) {
			Fl_Button * mButton = new Fl_Button(70+(mWidgetNum/20)*340, VerPos, 200, 20);
			mButton->label( name );
			mButton->labelsize(12);
			mButton->value( value );
			mButton->callback(...);
			add(*mButton);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
		template <typename T>
		virtual void Accept(const char *name, DynamicType *foo, T&value) {
			Fl_Button * mButton = new Fl_Button(70+(mWidgetNum/20)*340, VerPos, 200, 20);
			mButton->label( name );
			mButton->labelsize(12);
			mButton->value( value );
			mButton->callback(...);
			add(*mButton);
			mWidgetNum++;
			mWidgets.insert(tWidgets::value_type(name, mInput));
		}
#endif

		static void Apply(Fl_Widget* o, void* v) {
			FLTKConfigurator * owner = dynamic_cast<FLTKConfigurator*>(o->window());
			CLAM_ASSERT(owner,"The given widget is not a FLTKConfigurator");
			owner->SetInfo();
		}
		static void Discard(Fl_Widget* o, void* v) {
			delete o->window();	
		}

		void FLTKConfigurator::Show() {
			GetInfo();
			
			Fl_Button *applyButton=new Fl_Button((345+(mWidgetNum/20)*340)/2-110,25*(mWidgetNum>20 ? 21 : mWidgetNum%20+1),100,20);
			applyButton->label( "Apply" );
			applyButton->labelsize(12);
			applyButton->callback(Apply,this);
			add(*applyButton);

			Fl_Button *discardButton=new Fl_Button((345+(mWidgetNum/20)*340)/2+70, 25*(mWidgetNum>20 ? 21 : mWidgetNum%20+1),100,20);
			discardButton->label( "Discard" );
			discardButton->labelsize(12);
			discardButton->callback(Discard, this);
			add(*discardButton);

			size(345+(mWidgetNum/20)*340,35+25*(mWidgetNum>20 ? 21 : mWidgetNum%20+1));

			end();
			set_modal();
			show();
		}

	private:
		tWidgets mWidgets;
		int mWidgetNum;
		Configuration * mConfig;

		class VisitorGetter{
		public:
			VisitorGetter(FLTKConfigurator* otherBuilder) {
				mBuilder = otherBuilder;
			}

			virtual ~VisitorGetter() {}
			
			template <typename T>
			void Accept(const char *name, T &value) { 
				mBuilder->AddWidget(name, &value, value); 
			}

		private:
			FLTKConfigurator* mBuilder;
		};
		class VisitorSetter{
		public:
			VisitorSetter(FLTKConfigurator* otherBuilder) {
				mBuilder = otherBuilder;
			}

			virtual ~VisitorSetter() {}
			
			template <typename T>
			void Accept(const char *name, T &value) { 
				mBuilder->RetrieveValue(name, &value, value); 
			}

		private:
			FLTKConfigurator* mBuilder;
		};
	};
}
#endif
