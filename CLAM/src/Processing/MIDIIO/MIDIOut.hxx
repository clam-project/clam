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

#ifndef __MIDIOut__
#define __MIDIOut__

#include "Processing.hxx"
#include "MIDIDevice.hxx"
#include "MIDIEnums.hxx"

namespace CLAM {

/** Configuration of the MIDIOut class. You can specify
 * a device (string), which will be used by the MIDIManager, when creating
 * the MIDIDevice objects. When you don't specify a concrete device, or
 * specify "default", the MIDIManager will choose the default device for your
 * setup.
 * @see MIDIIn, MIDIOut, MIDIDevice, MIDIManager, ProcessingConfig
 */
class MIDIOutConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (MIDIOutConfig, 4, ProcessingConfig);

	/** The Device that will be used by this MIDIOut instantiation, if it 
	 *  is not indicated, or its value is "default:default", system will
	 *  choose the best device for this object
	 **/
	DYN_ATTRIBUTE (0, public, std::string, Device);

	/** The Message attribute allows you to create a MIDIOut that sends midi
	 *  messages of a certain type. */
	DYN_ATTRIBUTE (1, public, unsigned short, Message);

	/** The Channel attribute allows you to create a MIDIOut that sends midi
	 *  messages on a certain channel. If you leave it 0 (default), an 
	 *  InControl will be created to control the channel of each MIDI message.
	 **/
	DYN_ATTRIBUTE (2, public, unsigned short, Channel);

	/** The Message attribute allows you to create a MIDIOut that sends midi
	 *  messages with a certain first data byte (typically usefull for
	 *  control messages). If you leave it 128 (default), an InControl will
	 *  be created to control the first data byte. */
	DYN_ATTRIBUTE (3, public, unsigned short, FirstData);

protected:
	void DefaultInit(void)
	{
		AddDevice();
		AddMessage();
		AddChannel();
		AddFirstData();

		UpdateData();

		SetDevice("default:default");
		SetMessage(0);

		/* 0 means: channel is controlled by and InControl */
		SetChannel(0);
		
		/* 128 means: first data byte is controlled by an InControl */
		SetFirstData(128); 
	}
};


/** This class is the interface to an input of an MIDIDevice.
 *  @see Processing, MIDIInConfig, MIDIIn, MIDIManager, MIDIDevice
 */
class MIDIOut: public Processing
{
friend class MIDIManager;
friend class MIDIDevice;
protected:
	MIDIOutConfig mConfig;
	MIDIDevice* mpDevice;
public:

	/** Configuration method interface. The Processing base class forces all the concrete classes derived from it to implement this method, which must actually perform the specific configuration tasks.  
	 *  Note that the user can not call this method directly. He will use Configure instead. The argument is expected to be an object of the necesary concrete configuration class.
	 *  @return true if the object has been configured correctly; true otherwise
	 *  @param The related ProcessingConfig object
	 *  @throws ErrProcessingObj when there is an error on device adquisition
	 */
	virtual bool ConcreteConfigure(const ProcessingConfig& c)
		throw(ErrProcessingObj);
	
	/** Getter for the configuration of the class
	 *  @return The ProcessingConfig object attached to this Processing object
	 */		
	const ProcessingConfig &GetConfig() const { return mConfig;}
protected:
	virtual void Handle(unsigned char* msg,int size) = 0;
public:
	/** Constructor of the class with a boolean as parameter to choose whether
	 *  to configure the object. It has been implemented by this way to solve
	 *  a problem between constructor of this class and constructors of classes
	 *  derived of MIDIOut. The derived class constructor will call the MIDIOut
	 *  constructor which will call Configure. Obviously, this will cause
	 *  problems, when the ConcreteConfigure is reimplemented in the derived
	 *  class, but the MIDIOut constructor calls MIDIOut::ConcreteConfigure
	 *  instead. The solution is a default constructor of MIDIOut with the
	 *  signature MIDIOut::MIDIOut(bool configure = true). The configure flag
	 *  indicated that the constructor should call Configure (by default),
	 *  but not in the case of derived classes.
	 *  @param configure Boolean parameter to decide if configure object, like
	 *  explained before. True by default.
	 */
	MIDIOut(bool configure = true)
	{ 
		mpDevice = 0;
		if (configure)
		{
			Configure(MIDIOutConfig());
		}
	}

	/** Constructor of the class with an MIDIOutConfig object constructed by
	 *  the user as parameter.
	 *  @param c The concrete MIDIOutConfig that will be used for this
	 *  construction
	 */		
	MIDIOut(const MIDIOutConfig &c)
	{
		mpDevice = 0;
		Configure(c);
	}

	/** Destructor unregisters this MIDIOut instance from the associated 
	 *  device */
	~MIDIOut() { if (mpDevice) mpDevice->Unregister(*this); }

	const char * GetClassName() const {return "MIDIOut";}

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
