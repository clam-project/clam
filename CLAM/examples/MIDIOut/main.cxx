#include "MIDIManager.hxx"
#include "MIDIOutControl.hxx"

using namespace CLAM;

main()
{
	MIDIManager manager;
	MIDIOutConfig outNoteCfg;

	// NOTE OUT EXAMPLE WITH FIXED CHANNEL`
	outNoteCfg.SetDevice("alsa:hw:1,0");
	outNoteCfg.SetChannel(1);

	outNoteCfg.SetMessage(MIDI::eNoteOn);

	MIDIOutControl outNote(outNoteCfg);
	
	manager.Start();
	
	outNote.GetInControls().GetByNumber(0).DoControl(60);
	outNote.GetInControls().GetByNumber(1).DoControl(120);

	sleep(1);

	outNote.GetInControls().GetByNumber(0).DoControl(60);
	outNote.GetInControls().GetByNumber(1).DoControl(0);
	
	sleep(1);

	// STOP HERE
	return 0;
	// NOTE OUT EXAMPLE WITH VARIABLE CHANNEL
	MIDIOutConfig outNote2Cfg;

	outNote2Cfg.SetDevice("default:default");
	outNote2Cfg.SetChannel(0); // means: create an in control!

	outNote2Cfg.SetMessage(MIDI::eNoteOn);

	MIDIOutControl outNote2(outNote2Cfg);
	
	outNote2.GetInControls().GetByNumber(0).DoControl(3);
	outNote2.GetInControls().GetByNumber(1).DoControl(60);
	outNote2.GetInControls().GetByNumber(2).DoControl(120);

	outNote2.GetInControls().GetByNumber(0).DoControl(3);
	outNote2.GetInControls().GetByNumber(1).DoControl(60);
	outNote2.GetInControls().GetByNumber(2).DoControl(120);

	// VOLUME CONTROL CHANGE EXAMPLE
	MIDIOutConfig volCtrlCfg;

	volCtrlCfg.SetDevice("default:default");
	volCtrlCfg.SetChannel(1);
	volCtrlCfg.SetFirstData(7); // 7 == volume ctrl change
	

	volCtrlCfg.SetMessage(MIDI::eControlChange);

	MIDIOutControl volCtrl(volCtrlCfg);

	// send value 110 to volume control change on channel 1
	volCtrl.GetInControls().GetByNumber(0).DoControl(110);
}
