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

		bool Do( Parameters& params );

	private:
		MIDIHandlerConfig							mConfig;
		InControlTmpl< MIDIHandler >				mVelocityIn;
		InControlTmpl< MIDIHandler >				mNoteIn;
		InControlTmpl< MIDIHandler >				mPitchBendIn;
		InControlTmpl< MIDIHandler >				mAirSpeedIn;
		OutControl									mStatusOut;
		TControlData								mVelocity;
		TControlData								mNote;
		TControlData								mNoteOff;
		TControlData								mPitchBend;
		TControlData								mAirSpeed;
		TIndex										mTranspose;
		double										mPitchModRange;
		int											mLastPitch;
		enum Status {
			eNoteOff	 = 0,
			eNoteOn		 = 1,
			eCtrAirSpeed = 2,
			eIdle		 = 3,
		}   mStatus;

	protected:

		bool ConcreteConfigure( const ProcessingConfig& cfg ) throw(std::bad_cast);
		bool ConcreteStart();
		bool ConcreteStop();
		void NoteOn(Parameters& params);
		void NoteOff(Parameters& params);
		void CtrAirSpeed( Parameters &params );

		void ScaleNote( TControlData& note )
		{
		    note += mTranspose;

			if (note<=50) 
				note = 0;       //we work with two octaves 
			else if (note >=75)
				note =24;
			else              // 0 is lowest pitch
				note -= 50;
		}

		int UpdateVelocity( TControlData velocity )
		{
			mVelocity = velocity;

			return 0;
		}

		int UpdateNote ( TControlData note )
		{
			ScaleNote( note );

			if( ( mStatus != eNoteOff ) && ( note != 0 ) ) // Processing new note
			{
				if( mVelocity == 0 )  // Note Off
				{
					if( note == mNote ) // Note Off for the actual note
					{
						mStatus = eNoteOff;
						mNoteOff = note;
						mNote = 0.0;
						mAirSpeed = 0.0;
					}
				}
				else // Note On or Note On + Air Speed message
				{
					if( mStatus != eCtrAirSpeed ) 
						mStatus = eNoteOn;

					mNote = note;
					mLastPitch = 0.0;
				}

			}

			return 0;
		}

		int UpdatePitchBend( TControlData value )
		{
	
			mPitchBend = ((double) value - 70 ) / 70.0 * mPitchModRange + 1.0;

			return 0;
		}

		int UpdateAirSpeed( TControlData airSpeed )
		{
			mAirSpeed = airSpeed;
			mStatus = eCtrAirSpeed;

			return 0;
		}

	
	};
}

#endif