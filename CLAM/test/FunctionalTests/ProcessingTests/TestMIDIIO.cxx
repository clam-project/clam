#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "MIDIOutControl.hxx"
#include "MIDIClocker.hxx"
#include <vector>

using namespace CLAM;

main()
{
	MIDIManager manager;
	MIDIInConfig inNoteCfg;
	MIDIOutConfig outNoteCfg;

	inNoteCfg.SetDevice("file:test.mid");
	inNoteCfg.SetMessageMask(
			MIDI::MessageMask(MIDI::eNoteOn)|
			MIDI::MessageMask(MIDI::eNoteOff));
	inNoteCfg.SetChannelMask(MIDI::ChannelMask(-1));
	
	MIDIClockerConfig clockerCfg;

	clockerCfg.SetDevice("file:test.mid");
		
	MIDIClocker clocker(clockerCfg);
	
	outNoteCfg.SetDevice("textfile:test.txt");
	outNoteCfg.SetMessage(MIDI::eNoteOn);
	outNoteCfg.SetChannel(1);

	MIDIInControl inNote(inNoteCfg);
	MIDIOutControl outNote(outNoteCfg);
	
	//control for stoping at eof 
	MIDIInConfig inStopCfg;
	inStopCfg.SetDevice("file:test.mid");
	inStopCfg.SetChannelMask(CLAM::MIDI::SysMsgMask(CLAM::MIDI::eStop)); //it is a sys message that uses channel byte for actual data
	inStopCfg.SetMessageMask(CLAM::MIDI::MessageMask(CLAM::MIDI::eSystem));
	
	MIDIInControl inStop(inStopCfg);
	InControl stopReceiver("stop-receiver");

	inStop.GetOutControls().GetByNumber(0).AddLink(
			&stopReceiver);
	
	inNote.GetOutControls().GetByNumber(0).AddLink(
			&outNote.GetInControls().GetByNumber(0));
	inNote.GetOutControls().GetByNumber(1).AddLink(
			&outNote.GetInControls().GetByNumber(1));
	inNote.GetOutControls().GetByNumber(2).AddLink(
			&outNote.GetInControls().GetByNumber(0));
	inNote.GetOutControls().GetByNumber(3).AddLink(
			&outNote.GetInControls().GetByNumber(1));
	
	manager.Start();

	TTime curTimeInc = 10;
	TTime curTime = 0;

	while (stopReceiver.GetLastValue()==0)
	{
		//we send a timing control to the MIDI clocker 
		clocker.GetInControls().GetByNumber(0).DoControl(curTime);
		
		//we check for new events in the MIDI manager
		manager.Check();
		
		//we increment the time counter
		curTime ++;
	}
}
