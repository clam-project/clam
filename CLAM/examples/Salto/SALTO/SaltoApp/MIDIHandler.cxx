#include "MIDIHandler.hxx"

namespace CLAM
{

	void MIDIHandlerConfig::DefaultInit()
	{
		AddAll();	
		UpdateData();
	}

	MIDIHandler::MIDIHandler():
		mVelocityIn("Velocity", this, &MIDIHandler::UpdateVelocity),
		mNoteIn("Note", this, &MIDIHandler::UpdateNote),
		mPitchBendIn( "PitchBend", this, &MIDIHandler::UpdatePitchBend ),
		mAirSpeedIn( "AirSpeed", this, &MIDIHandler::UpdateAirSpeed ),
		mStatusOut( "Status", this ),
		mVelocity( 0.0 )
	{
		MIDIHandlerConfig cfg;

		mParams = 0;
 
		Configure( cfg );
	}

	MIDIHandler::MIDIHandler( const MIDIHandlerConfig& cfg ):
		mVelocityIn("Velocity", this, &MIDIHandler::UpdateVelocity),
		mNoteIn("Note", this, &MIDIHandler::UpdateNote),
		mPitchBendIn( "PitchBend", this, &MIDIHandler::UpdatePitchBend ),
	    mAirSpeedIn( "AirSpeed", this, &MIDIHandler::UpdateAirSpeed ),
		mStatusOut( "Status", this ),
		mVelocity( 0.0 )
	{
		mParams = 0;

		Configure( cfg );
	}

	MIDIHandler::~MIDIHandler()
	{ 
	}

	bool MIDIHandler::ConcreteConfigure( const ProcessingConfig& cfg)throw( std::bad_cast )
	{
		mConfig = dynamic_cast< const MIDIHandlerConfig& >( cfg );

		return true;
	}

	bool MIDIHandler::ConcreteStart()
	{
		return true;
	}

	bool MIDIHandler::ConcreteStop()
	{
		return true;
	}

	int MIDIHandler::UpdateNote ( TControlData note )
	{
		int	pitch = int(note+Params().GetTranspose());
		int velocity = int(mVelocity);

		pitch -= 50;
		if (pitch<0) pitch=0;
		else if (pitch>24) pitch=24;
			
		//std::cout << "UpdateNote" << note << "(vel=" << mVelocity << ")" << std::endl;

		if( Params().GetUseBreathController() )
		{
			
			if (velocity)
			{
				// MidiNote to Frequency :: F=440*2^((P-69)/12)
				Params().SetStatResFreq(440.0*pow(2,(pitch+48-69)/12.0));

				if(Params().GetPlay()               // this means transition !
					&& Params().GetAttackTimbre().GetLevel()>0
					&! Params().GetTransitionInit()) 
				{
					Params().SetTransitionInit(true); // step into trans init process
					Params().SetTransitionMode(true);
					Params().SetDisplayedValuesChanged(true); // display needs update 

					TIndex val = 30-velocity;
					#ifdef TRUMPET
					val = CLIP(val,1,10);
					#endif           
					#ifdef ALTOSAX    
					val = CLIP(val,2,10);
					#endif            
					//Params().SetAttackTimbreTransFrames(val);    

					if (pitch>=Params().GetAttackTimbre().GetPitch())   // check if transition up or down
						Params().SetTransitionUpwards(true);
					else
						Params().SetTransitionUpwards(false);
						Params().SetTransitionInterval(abs(pitch-Params().GetAttackTimbre().GetPitch()));			 
				}

				if (Params().GetTransitionMode()) // check for note transition 
				{
					mStatusOut.SendControl( 2 );
					// set only transition specific parameters
					// we dont need a new attack
					Params().SetAttackTimbrePitch(pitch);
					Params().SetDisplayedValuesChanged(true);
				}
				else // play note from beginning, use attack and set timbre vektor
				{
					mStatusOut.SendControl( 1 );
					Params().SetAttackTimbrePitch(pitch);
					int attack_reduction = 25;
					velocity -= attack_reduction; //decrase attack velocity, because always is bigger than stationary part
					//cout << " velocity: " << velocity << endl;
					if (velocity>=(125-attack_reduction)) //110
					{
						Params().SetAttackTimbreAttack(VHARD);
						Params().SetBreathOnlySound(false);
					}
					else if (velocity>=(85-attack_reduction))//(velocity>=60) //80
					{
						Params().SetAttackTimbreAttack(HARD);
						Params().SetBreathOnlySound(false);
					}
					else if (velocity>=(50-attack_reduction))
					{
						Params().SetAttackTimbreAttack(SOFT);
						Params().SetBreathOnlySound(false);
					}
					else if (velocity>=(25-attack_reduction))
					{
						Params().SetAttackTimbreAttack(VSOFT);
						Params().SetBreathOnlySound(false);
					}
					else
					{
						Params().SetAttackTimbreAttack(VSOFT);
						Params().SetBreathOnlySound(true);
					//              Params().SetAttackTimbreAttack(BREATHONLY);
					}
					Params().SetDisplayedValuesChanged(true); // display needs update
					/* activate play */
					Params().SetPlay(true);
				}
			}
			else 
			{
				// note off

				if(Params().GetAttackTimbre().GetLevel()==0)//Params().GetAttackTimbre().GetPitch())
				{
					Params().SetPlay(false);
					Params().SetTransitionMode(false);
					Params().SetTransitionInit(false);
					//std::cout << "RELEASE\n";
					mStatusOut.SendControl(4);
					// else ignore note off
				}
			}
		}
		else
		{
			if(velocity)
			{
			 
			// MidiNote to Frequency :: F=440*2^((P-69)/12)
			 Params().SetStatResFreq(440.0*pow(2,(pitch+48-69)/12.0));
     
     
			 if( Params().GetPlay() )// this means transition !
			 {
			   Params().SetTransitionInit(true); // step into trans init process
			   Params().SetTransitionMode(true);
			   Params().SetDisplayedValuesChanged(true);
			   //XA clutch
			   /*TIndex val = 50-velocity;
			   val = CLIP(val,3,30);*/

			   //TIndex val = 5; // too much noise in the transition

			   TIndex val=2;
			   Params().SetAttackTimbreTransFrames(val);
     
			   if (pitch>=Params().GetAttackTimbre().GetPitch())   // check if transition up or down
				Params().SetTransitionUpwards(true);
			   else
				Params().SetTransitionUpwards(false);
			   Params().SetTransitionInterval(abs(pitch-Params().GetAttackTimbre().GetPitch()));
			 }
     
			 if (Params().GetTransitionMode()) // check for note transition 
			 {
				 
				 mStatusOut.SendControl( 2 ); // Sending transition state notification to CSaltoDSP
				 //std::cout << "Note: "<< mNote << std::endl;

				// set only transition specific parameters
				// we dont need a new attack
				Params().SetAttackTimbrePitch(pitch);
				double ipFactor = velocity/60.0;
				/*
				if (ipFactor<0.1) 
				  ipFactor=0.1;
				Params().SetInterPolFactor(ipFactor); 
				*/
				Params().SetDisplayedValuesChanged(true);
			 }
			 else // play note from beginning, use attack and set timbre vektor
			 {
				mStatusOut.SendControl( 1 );  // Sending Attack state notification to CSaltoDSP
			   Params().SetAttackTimbrePitch(pitch);
       
			   //XA: changed to meet breathcontroller Params()
			   if (velocity>=100)          
				  Params().SetAttackTimbreAttack(VHARD);
			   else if (velocity>=60)
				  Params().SetAttackTimbreAttack(HARD);
			   else if (velocity>=25)
				  Params().SetAttackTimbreAttack(SOFT);
			   else
				  Params().SetAttackTimbreAttack(VSOFT);       
        
       
			   if (velocity>=50) // use forte stat templates
			   {
				  // select current stat template depending on pitch region
				  if (pitch > 9)
					Params().SetCurrentStatTemplate(7);
				  else if (pitch > 6)
					Params().SetCurrentStatTemplate(6);
				  else if (pitch > 3)
					Params().SetCurrentStatTemplate(5);
				  else if (pitch >= 0)
					Params().SetCurrentStatTemplate(4);
       
				  // map breath volume to stationary timbre template 
				  double ipFactor = (velocity-50)/140.0;
				  Params().SetInterPolFactor(ipFactor);
				}
				else // use piano stat templates
				{
				  // select current stat template depending on pitch region
				  if (pitch > 9)
					Params().SetCurrentStatTemplate(3);
				  else if (pitch > 6)
					Params().SetCurrentStatTemplate(2);
				  else if (pitch > 3)
					Params().SetCurrentStatTemplate(1);
				  else if (pitch >= 0)
					Params().SetCurrentStatTemplate(0);
         
				 // map breath volume to stationary timbre template 
				  double ipFactor =velocity/50.0;
				  if (ipFactor<0.1) 
					ipFactor=0.1;
				  else if (ipFactor>=1)
					ipFactor=1;
				  Params().SetInterPolFactor(1-ipFactor);
				} 
				Params().SetAttackTimbreLevel(velocity*0.5+63);  
				Params().SetDisplayedValuesChanged(true); // display needs update
				Params().SetPlay(true); // activate play
			 }
			}
			else 
			{
			  if(pitch==Params().GetAttackTimbre().GetPitch())
			  {
				  Params().SetPlay(false);
				  //std::cout << "RELEASE\n";
				  mStatusOut.SendControl(4);
			  }	
			}
		}

		return 0;
	}

	int MIDIHandler::UpdatePitchBend( TControlData value )
	{
		value *= TControlData(127)/TControlData(16384);

	  	Params().SetPitchModFactor(
    	((double)value-70)/70.0*mConfig.GetPitchModRange()+1.0);

		//std::cout << "UpdatePitchBend: " << value << std::endl;

		return 0;
	}

	int MIDIHandler::UpdateAirSpeed( TControlData airSpeed )
	{
		int volume = int(airSpeed);

		//std::cout << "UpdateAirSpeed" << volume << std::endl;

		Params().SetAttackTimbreLevel(volume);

		int pitch = Params().GetAttackTimbre().GetPitch();

		if (volume>=100) // use forte stat templates
		{
			// select current stat template depending on pitch region
			if (pitch > 9)
				Params().SetCurrentStatTemplate(7);
			else if (pitch > 6)
				Params().SetCurrentStatTemplate(6);
			else if (pitch > 3)
				Params().SetCurrentStatTemplate(5);
			else if (pitch >= 0)
				Params().SetCurrentStatTemplate(4);
			// map breath volume to stationary timbre template 
			double ipFactor = (volume-100)/200.0;
			Params().SetInterPolFactor(ipFactor);
		}
		else // use piano stat templates
		{
			// select current stat template depending on pitch region
			if (pitch > 9)
				Params().SetCurrentStatTemplate(3);
			else if (pitch > 6)
				Params().SetCurrentStatTemplate(2);
			else if (pitch > 3)
				Params().SetCurrentStatTemplate(1);
			else if (pitch >= 0)
				Params().SetCurrentStatTemplate(0);

			// map breath volume to stationary timbre template 
			double ipFactor =volume/100.0;
			if (ipFactor<0.02) 
				ipFactor=0.02;
			else if (ipFactor>=1)
				ipFactor=1;
			Params().SetInterPolFactor(1-ipFactor);
		}

		if(Params().GetAttackTimbre().GetLevel()==0)//Params().GetAttackTimbre().GetPitch())
		{
			Params().SetPlay(false);
			Params().SetTransitionMode(false);
			Params().SetTransitionInit(false);
		} 

		Params().SetDisplayedValuesChanged(true); // display needs update
		return 0;
	}
}
