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

#ifndef __CONFIGURATIONVISITOR__
#define __CONFIGURATIONVISITOR__

namespace CLAM{
	class ConfigurationVisitor {
	public:
		virtual ~ConfigurationVisitor() {}
		virtual void VisitConfig()=0;
	};

	/**
	 * A DynamicType attribute visitor that builds a widget
	 * for each attribute using a user interface builder.
	 * The builder should implement AddWidget overloaded methods
	 * for every type whe want to support.
	 * @ingroup Configurators
	 * @see ConfigurationSetter, QTConfigurator, FLTKConfigurator, DynamicType
	 */
	template <typename Configuration, typename Builder>
	class ConfigurationGetter : public ConfigurationVisitor {
	public:
		ConfigurationGetter(Configuration * config, Builder* builder) {
			mBuilder = builder;
			mConfig = config;
		}

		virtual ~ConfigurationGetter() {}
		
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
	/**
	 * A DynamicType attribute visitor that takes the attribute
	 * values from an a 'Builder' which implements the RetrieveValue.
	 * for each attribute using a user interface builder.
	 * The builder should implement RetrieveValue overloaded methods
	 * for every type whe want to support.
	 * @ingroup Configurators
	 * @see configurationGetter, QTConfigurator, FLTKConfigurator, DynamicType
	 */
	template <typename Configuration, typename Builder>
	class ConfigurationSetter : public ConfigurationVisitor {
	public:
		ConfigurationSetter(Configuration * config, Builder* builder) {
			mBuilder = builder;
			mConfig = config;
		}

		virtual ~ConfigurationSetter() {}
		
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

/**
 * @defgroup Configurators Dynamic Types Configurators: Quick generation of graphic dialogs for editing DTs
 *
 * Configurators quickly builds GUI dialogs to modify a given DynamicType object.
 * They use DynamicType's introspection features to build a dialog for a given graphic toolkit.
 * Qt and FLTK suported at the moment but it would be easy to extend to other toolkits
 * by provident a new Configurator Object.
 *
 * For example, to show a Qt configurator dialog.
 * @code
 * CLAM::QTConfigurator dialog;
 * dialog.SetConfig(myDinamicType);
 * dialog.show();
 * @endcode
 * 
 * For FLTK you can achieve nearly the same by doing:
 * @code
 * CLAM::FLTKConfigurator * configurator = new CLAM::FLTKConfigurator;
 * configurator->SetConfig(myDynamicType);
 * configurator->show();
 * @endcode
 */

}
#endif//__CONFIGURATIONVISITOR__
