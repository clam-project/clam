/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __MIDIIn__
#define __MIDIIn__

#include "Processing.hxx"
#include "MIDIDevice.hxx"
#include "MIDIEnums.hxx"

namespace CLAM {

/** Configuration of the MIDIIn class. You can specify
 * a device (string), which will be used by the MIDIManager, when creating
 * the MIDIDevice objects. If you don't specify a concrete device, or
 * specify "default", the MIDIManager will choose the default device for your
 * setup.
 * @see MIDIIn, MIDIOut, MIDIDevice, MIDIManager, ProcessingConfig
 */
class MIDIInConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MIDIInConfig, 4, ProcessingConfig);

	/** The Device that will be used by this MIDIIn instantiation, if it 
	 *  is not indicated, or its value is "default:default", system will
	 *  choose the best device for this object
	 **/
	DYN_ATTRIBUTE (0, public, std::string, Device);
	
	/** The ChannelMask allows you to create a MIDIIn that receives midi
	 *  messages on a certain channel or channels only. The way to calculate
	 *  the ChannelMask is by adding the channel masks for the channels you 
	 *  want to filter with bitwise OR. You can use the function 
	 *  MIDI::ChannelMask(channel) to obtain the mask for a certain channel.
	 **/
	DYN_ATTRIBUTE (1, public, unsigned short, ChannelMask);

	/** The MessageMask allows you to create a MIDIIn that receives midi
	 *  messages of a certain type only. The way to calculate the MessageMask
	 *  is by adding the message masks for the message types you want to 
	 *  filter with logical OR. You can use the function 
	 *  MIDI::MessageMask(channel) to obtain the mask for a certain
	 *  channel.
	 **/
	DYN_ATTRIBUTE (2, public, unsigned short, MessageMask);

	/** The Filter allows you to create a MIDIIn that receives midi messages
	 *  where the second byte (first data byte) has a certain value. This is
	 *  particularly useful for control change messages, where the second byte
	 *  specifies the type of control change.
	 **/
	DYN_ATTRIBUTE (3, public, unsigned char, Filter);
protected:
	void DefaultInit(void)
	{
		AddDevice();
		AddChannelMask();
		AddMessageMask();
		AddFilter();

		UpdateData();

		SetDevice("default:default");
		SetMessageMask(MIDI::sAllMessageMask);
		SetChannelMask(MIDI::sAllChannelMask);
		SetFilter(0xFF);
	}
};

/** This class is the interface to an input of an MIDIDevice.
 *  @see Processing, MIDIInConfig, MIDIOut, MIDIManager, MIDIDevice
 */
class MIDIIn: public Processing
{
friend class MIDIManager;
friend class MIDIDevice;
protected:
	MIDIInConfig mConfig;
	MIDIDevice* mpDevice;
public:
	/** Configuration method interface. The Processing base class forces
	 *  all the concrete classes derived from it to implement this method,
	 *  which must actually perform the specific configuration tasks.  
	 *  Note that the user can not call this method directly. He will use 
	 *  Configure instead. The argument is expected to be an object of the 
	 *  necesary concrete configuration class.
	 *  @return true if the object has been configured correctly
	 *  @param The related ProcessingConfig object
	 *  @throws ErrProcessingObj when there is an error on device adquisition
	 */
	virtual bool ConcreteConfigure(const ProcessingConfig& c)
		throw(ErrProcessingObj);
	
	/** Getter for the configuration of the class
	 *  @return The ProcessingConfig object attached to this Processing object
	 */		
	const ProcessingConfig &GetConfig() const { return mConfig;}

	virtual void Handle(unsigned char* msg,int size) = 0;
	
	/** Constructor of the class with a boolean as parameter to choose whether
	 *  to configure the object. It has been implemented by this way to solve
	 *  a problem between constructor of this class and constructors of classes
	 *  derived of MIDIIn. The derived class constructor will call the MIDIIn 
	 *  constructor which will call Configure. Obviously, this will cause
	 *  problems, when the ConcreteConfigure is reimplemented in the derived
	 *  class, but the MIDIIn constructor calls MIDIIn::ConcreteConfigure
	 *  instead. The solution is a default constructor of MIDIIn with the
	 *  signature MIDIIn::MIDIIn(bool configure = true). The configure flag
	 *  indicated that the constructor should call Configure (by default),
	 *  but not in the case of derived classes.
	 *  @param configure Boolean parameter to decide if configure object, like
	 *  explained before. True by default.
	 */
	MIDIIn(bool configure = true)
	{ 
		mpDevice = 0;
		if (configure)
		{
			Configure(MIDIInConfig());
		}
	}

	/** Constructor of the class with an MIDIInConfig object constructed by
	 *  the user as parameter.
	 *  @param c The concrete MIDIInConfig that will be used for this
	 *  construction
	 */		
	MIDIIn(const MIDIInConfig &c)
	{
		mpDevice = 0;
		Configure(c);
	}

	/** Destructor unregisters this MIDIOut instance from the associated 
	 *  device */
	~MIDIIn() { if (mpDevice) mpDevice->Unregister(*this); }

	const char * GetClassName() const {return "MIDIIn";}

	/** Non supervised mode of Do function. Non implemented yet
	 */
	bool Do(void)
	{ 
		return true;
	}

	/** Getter for the Info of Device Object attached to this MIDIOut
	 *  instantiation
	 *  @param info TInfo object that method will modify with the values of
	 *  Tinfo internal object
	 */
	void GetDeviceInfo(MIDIDevice::TInfo &) const;

protected:
	bool ConcreteStart(void);
};



} // namespace CLAM
#endif
