#include "MIDIInControl.hxx"

namespace CLAM {

MIDIInControl::MIDIInControl():MIDIIn(false)
{ 
	mpDevice = 0;
	Configure(MIDIInConfig());
}

MIDIInControl::MIDIInControl(const MIDIInConfig &c):MIDIIn(false)
{
	mpDevice = 0;
	Configure(c);
}

bool MIDIInControl::ConcreteConfigure(const ProcessingConfig& c)
	throw(ErrProcessingObj)
{
	bool ret = MIDIIn::ConcreteConfigure(c);
	if (ret==false) return false;

	/* We go through the possible status types, and check if this
	 * MIDIInControl has been configured to receive any of those.
	 */
	for (int status = 0;status<8;status++)
	{
		if (mConfig.GetMessageMask()&MIDI::GetMessageInfo(status).mask)
		{
			/* Yes, we want to receive messages of type status */
			
			/* We can specify that we only want to receive messages
			 * with a specific first data byte - typically useful
			 * for control-change messages. The variable first relevant
			 * indicates this. */
			int firstRelevant = 0;
			if (mConfig.GetFilter()!=0xFF) firstRelevant = 1;

			if ((MIDI::Message)status == MIDI::ePitchbend)
			{
				/* we make an exception for pitchbend: instead of putting
				 * out to values (LSB, MSB), we prefer 1 14bit value.
				 *	see also the code in Handle
				 */
				char tmp[256];
				snprintf(tmp,255,"%s:Value",
					MIDI::GetMessageInfo(status).name);
				mMyOutControls.AddElem(new OutControl(tmp,this));
				mMyOutControlInfos.AddElem(new OutControlInfo((MIDI::Message)status,0));
			}
			else
			{
				if (MIDI::GetMessageInfo(status).length<=1)
				{
					mMyOutControls.AddElem(new OutControl("non-data byte",this));
					mMyOutControlInfos.AddElem(new OutControlInfo((MIDI::Message)status,0));
				}
				else
				{
					/* Add controls for all data bytes that we are interested
					 * in, possibly skipping the first (index=0), in case we
					 * specified a filter. */
					for (int i=firstRelevant;i<MIDI::GetMessageInfo(status).length-1;i++)
					{
						if (MIDI::GetMessageInfo(status).field[i])
						{
							char tmp[256];
							snprintf(tmp,255,"%s:%s",
									MIDI::GetMessageInfo(status).name,
									MIDI::GetMessageInfo(status).field[i]);
							mMyOutControls.AddElem(new OutControl(tmp,this));
							mMyOutControlInfos.AddElem(new OutControlInfo((MIDI::Message)status,i));
						}
					}
				}
			}
		}
	}

	return true;
}

void MIDIInControl::Handle(unsigned char* msg,int size)
{
	/* The device has passed the message to this MIDIInControl.
	 * We now need to dispatch the message to the resp. OutControls
	 */
	int n = mMyOutControls.Size();
	for (int i=n-1;i>=0;i--)
	{
		if (MIDI::StatusByteToMessage(msg[0])==mMyOutControlInfos[i]->mMessage)
		{
			if (mMyOutControlInfos[i]->mMessage == MIDI::ePitchbend)
			{
				/* we make an exception for pitchbend: instead of putting
				 * out to values (LSB, MSB), we prefer 1 14bit value.
				 * see also the code in ConcreteConfigure
				*/
				GetOutControls().GetByNumber(i).SendControl(
					msg[mMyOutControlInfos[i]->mField+1]+
					(msg[mMyOutControlInfos[i]->mField+2]<<7)
				);
			}
			else if (mMyOutControlInfos[i]->mMessage == MIDI::eSystem)
			{
				GetOutControls().GetByNumber(i).SendControl(1);
			}
			else
			{
				GetOutControls().GetByNumber(i).SendControl(msg[mMyOutControlInfos[i]->mField+1]);
			}
		}
	}
}

} // namespace CLAM

