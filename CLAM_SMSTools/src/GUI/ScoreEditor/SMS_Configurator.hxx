#ifndef __SMSCONFIGURATOR__
#define __SMSCONFIGURATOR__

#include <FL/Fl_Widget.H>

namespace CLAM
{
	class ProcessingConfig;
}

namespace CLAMVM
{
	class SMSConfigurator 
	{
	public:
		virtual ~SMSConfigurator() { }

		virtual Fl_Widget* GetParametersWidget() = 0;
		virtual Fl_Widget* GetHelpWidget() = 0;
		virtual const CLAM::ProcessingConfig& GetConfig() = 0;
	};
}

#endif // SMSConfigurator.hxx
