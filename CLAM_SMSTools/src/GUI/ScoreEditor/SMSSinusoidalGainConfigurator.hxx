#ifndef __SMSSINUSOIDALGAINCONFIGURATOR__
#define __SMSSINUSOIDALGAINCONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SMSTransformationConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class SMSSinusoidalGainConfigurator : public SMSConfigurator
	{
	public:
		SMSSinusoidalGainConfigurator();
		virtual ~SMSSinusoidalGainConfigurator();

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


#endif // SMSSinusoidalGainConfigurator.hxx
