#ifndef __MIDIHANDLER__
#define __MIDIHANDLER__

#include "Processing.hxx"
#include "Parameters.hxx"
#include <iostream>

namespace CLAM
{
	class MIDIHandlerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDIHandlerConfig, 3, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TIndex, Transpose);
		DYN_ATTRIBUTE (2, public, double, PitchModRange);
	protected:
		void DefaultInit(void);
	};

	class MIDIHandler: public Processing
	{
	public:	
		MIDIHandler();
		MIDIHandler( const MIDIHandlerConfig& cfg );

		virtual ~MIDIHandler();

		const ProcessingConfig &GetConfig() const { return mConfig; }
		bool Do(void) { return true; }

		void SetParams(Parameters* params) { mParams = params; }
	private:
		MIDIHandlerConfig							mConfig;

		InControlTmpl< MIDIHandler >				mVelocityIn;
		InControlTmpl< MIDIHandler >				mNoteIn;
		InControlTmpl< MIDIHandler >				mPitchBendIn;
		InControlTmpl< MIDIHandler >				mAirSpeedIn;

		OutControl mStatusOut;

		TControlData mVelocity;
		
		Parameters* mParams;
		Parameters& Params(void) { return *mParams; }
	protected:
		bool ConcreteConfigure( const ProcessingConfig& cfg ) throw(std::bad_cast);
		bool ConcreteStart();
		bool ConcreteStop();

		int UpdateVelocity( TControlData velocity )
		{
			//std::cout << "velocity = " << velocity << std::endl;
			mVelocity = velocity;

			return 0;
		}

		int UpdateNote ( TControlData note );

		int UpdatePitchBend( TControlData value );

		int UpdateAirSpeed( TControlData airSpeed );

	};
}

#endif
