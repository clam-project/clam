#ifndef __SMSCONFIGURATOR__
#define __SMSCONFIGURATOR__

#include <FL/Fl_Widget.H>
#include "Factory.hxx"

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
		virtual void SetConfig( const CLAM::ProcessingConfig& ) = 0;
	};

	typedef CLAM::Factory<SMSConfigurator> SMSConfiguratorFactory;

}

#endif // SMSConfigurator.hxx
