#ifndef __SMSHARMONIZERCONFIGURATOR__
#define __SMSHARMONIZERCONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SMSTransformationConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class SMSHarmonizerConfigurator : public SMSConfigurator
	{
	public:
		SMSHarmonizerConfigurator();
		virtual ~SMSHarmonizerConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		CLAM::SMSTransformationConfig mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSHarmonizerConfigurator.hxx
