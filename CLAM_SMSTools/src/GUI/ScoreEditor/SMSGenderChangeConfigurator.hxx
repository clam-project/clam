#ifndef __SMSGENDERCHANGECONFIGURATOR__
#define __SMSGENDERCHANGECONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SMSTransformationConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class SMSGenderChangeConfigurator : public SMSConfigurator
	{
	public:
		SMSGenderChangeConfigurator();
		virtual ~SMSGenderChangeConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual void SetConfig( const CLAM::ProcessingConfig& );
		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		CLAM::SMSTransformationConfig mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSGenderChangeConfigurator.hxx
