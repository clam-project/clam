#ifndef __SMSPITCHSHIFTCONFIGURATOR__
#define __SMSPITCHSHIFTCONFIGURATOR__

#include "SMS_Configurator.hxx"
#include "SMSTransformationConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class Fl_SMS_PitchShift_Editor;


	class SMSPitchShiftConfigurator : public SMSConfigurator
	{
	public:
		SMSPitchShiftConfigurator();
		virtual ~SMSPitchShiftConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		Fl_SMS_PitchShift_Editor*     mEditorWidget;
		CLAM::SMSTransformationConfig mConfig;
		static const char*            mHelpText;
		
		
	};
}

#endif // SMSPitchShiftConfigurator.hxx
