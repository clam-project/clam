#ifndef __SMSMORPHCONFIGURATOR__
#define __SMSMORPHCONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SMSMorphConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class SMSMorphConfigurator : public SMSConfigurator
	{
	public:
		SMSMorphConfigurator();
		virtual ~SMSMorphConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		CLAM::SMSMorphConfig          mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSMorphConfigurator.hxx
