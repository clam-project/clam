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


#ifndef _Processing_hxx_
#define _Processing_hxx_


#include "ErrProcessingObj.hxx"
#include "PublishedInControls.hxx"
#include "PublishedOutControls.hxx"
#include "PublishedInPorts.hxx"
#include "PublishedOutPorts.hxx"
#include "ProcessingConfig.hxx"


#include <list>
#include <typeinfo>
#include <string>

namespace CLAM {

	class Processing;
	class InPortBase;
	class OutPortBase;
	class InControl;
	class OutControl;
	class ProcessingComposite;

	/**
	 * Processings are the building blocks of any CLAM system,
	 * they are an abstraction of an audio/music processing step.
	 * This is the base of all the CLAM processing object classes,
	 * defines their interface and contains common infrastructure.
	 * 
	 * @section Ports: Sincronous data flow
	 * A Processing can be feeded with data tokens and it produces
	 * data tokens in a given step whenever the Do method is executed.
	 * 
	 * Data flow from and into the Processing can be done using
	 * Ports. The number of tokens that are seen/consumed/produced
	 * though the port is flexible.
	 *
	 * @see InPortBase, OutPortBase
	 * 
	 * @section Controls: Asyncronous data flow
	 *
	 * A processing can also receive an event from another one in an
	 * asyncronous way in order to change the way the processing is
	 * done.
	 * 
	 * @see InControl, OutControl
	 *
	 * @section Configuration
	 *
	 * An initial setup is needed before using any processing.
	 * The ProcessingConfig object is an object that contains
	 * configuration parameters.
	 *
	 * @dot
	 * digraph example {
	 * 	node [shape=record, fontname=Helvetica, fontsize=10];
	 * 	b [ label="class B" URL="\ref B"];
	 * 	c [ label="class C" URL="\ref C"];
	 * 	b -> c [ arrowhead="open", style="dashed" ];
	 * }
	 * @enddot
	 *
	 * @see ProcessingConfig
	 *
	 * All the Processings have a 
	 * 
	 */
	class Processing {
	public:
		/** Processing Object possible execution states. */
		typedef enum {
			Unconfigured=0,
			Disabled,
			Ready,
			Running
		} ExecState;

// Basic usage interface:

	public:

		/** Configuration change method.
		 *  This is the method to be used in order to update the
		 *  configuration of a processing object. Note that the object
		 *  must NOT be running when calling this method.
		 *  This method performs some execution state checkings and
		 *  updates, and calls the ConcreteConfigure method of the
		 *  concrete class to perform the actual configuration.
		 *  @param Reference to the configuration object.
		 *  @throw ErrProcessingObj if the processing object is in
		 *  running or disabled state, or if the argument is not
		 *  an object of the configuration class matching the concrete
		 *  processing class of the processing object.
		 */
		bool Configure(const ProcessingConfig&);

		/** Method to turn the object into running state.
		 * This method must be called before any call to Do() methods.
		 * @pre the processing object is in ready state
		 */
		void Start(void);

		/**
		 * Supervised mode execution method (using ports)
		 * @return A boolean telling whether it has been output
		 */
		virtual bool Do(void)=0;

		/** Method to put the object out of running state When in
		 * execution mode, this method must be called before any
		 * further call to Configure() methods
		 * @pre The processing object is in runnig state (or disabled).
		 */
		void Stop(void);

		Processing();
		virtual ~Processing();

// Overridable interface:
	public:

		/** Override it in every subclass and retur the name of that class */
		virtual const char * GetClassName() const = 0;
		
		/** Override this method if your processing cannot process inplace*/
		virtual bool CanProcessInplace() { return true; }

		/** Configuration method interface.
		 * The Processing base class forces all the concrete
		 * classes derived from it to implement this method, which
		 * must actually perform the specific configuration
		 * tasks.
		 * <p> Note that the user can not call this method
		 * directly. He will use Configure instead. The argument is
		 * expected to be an object of the necesary concrete configuration
		 * class.
		 * @param Reference to the configuration object.
		 * @return <b>false</b> if the object is not yet fully configured.
		 * (For example, if some important configuration attribute
		 * such as number of ports was set to zero in the config
		 * object)
		 * <p> <b> true </b> if the processing object is left in a
		 * consistent state, and can be executed.
		 * @throw This method must throw a bad_cast exception if the
		 * argument is not an object of the expected configuration class.
		 * @todo ConcreteConfig should be protected
		 * */
		virtual bool ConcreteConfigure(const ProcessingConfig&) = 0; //TODO should be protected
		
	protected:
		/**
		 * Processing objects have to redefine this method when starting
		 * them implies some internal changes. ie: adquiring resources.
		 * @returns Whether start changes have been successful
		 */
		virtual bool ConcreteStart() {return true;};

		/**
		 * Processing objects have to redefine this method when stoping
		 * them implies some internal changes. ie: releasing resources.
		 * @returns Whether stop changes have been successful
		 */
		virtual bool ConcreteStop() {return true;};

// Public interface:	
	public:
		/** Check that Supervised Do() can be safely called */
		bool CanConsumeAndProduce();

		/**
		 *  This method, is temporary, very prone to disappear
		 *  soon, for enabling clients that know concrete Processing object
		 *  type to call safely the ConcreteConfigure(). See the functional
		 *  test of AudioFileIn and and its usage example for more details
		 *  on when and how to use them.
		 */
		void PreConcreteConfigure( const ProcessingConfig& c); //TODO depracate it
		
		/**
		 *  This method, is temporary, very prone to disappear
		 *  soon, for enabling clients that know concrete Processing object
		 *  type to call safely the ConcreteConfigure(). See the functional
		 *  test of AudioFileIn and and its usage example for more details
		 *  on when and how to use them.
		 */
		void PostConcreteConfigure(); //TODO depracate it

		/** Configuration getter.
		 * Gets the configuration parameters used to create the object.
		 * @return Const reference to the configuration object. The
		 * usual way to perform a configuration change in the
		 * processing object is to take a copy of this reference, to
		 * change it and to send it with the Configure method.
		 */
		virtual const ProcessingConfig &GetConfig() const = 0;

		/** State getter.
		 * Returns the object execution state. This may be used for
		 * debugging purposes, and should also be used inside the
		 * execution (Do) methods to confirm that the object is either
		 * in running or in disabled state.
		 */
		ExecState GetExecState() const {return mExecState;}
		

		void PublishOutPort(OutPortBase* out);
		void PublishInPort(InPortBase* in);
		void PublishOutControl(OutControl* out);
		void PublishInControl(InControl* in);

		void SetParent(Processing *p);

		/**
		 * This method is used to determine if a given processing can change its interface of ports/controls after
		 * its construction (i.e. changing the name of ports in ConcreteConfigure). If a concrete processing
		 * can do this, it should reimplement the method returning true, in order to notify networks, graphical
		 * interfaces, etc.
		 */
		virtual bool ModifiesPortsAndControlsAtConfiguration()	{ return false;	}
	
		InPortBase & GetInPort( const std::string & name )
		{
			return mPublishedInPorts.Get(name);
		}
		OutPortBase & GetOutPort( const std::string & name )
		{
			return mPublishedOutPorts.Get(name);
		}
		InControl & GetInControl( const std::string & name )
		{
			return mPublishedInControls.Get(name);
		}		
		OutControl & GetOutControl( const std::string & name )
		{
			return mPublishedOutControls.Get(name);
		}

		/** Accessor to published Controls manager */
		PublishedInControls& GetInControls() { return mPublishedInControls; }
		
		/** Accessor to published Controls manager */
		PublishedOutControls& GetOutControls() { return mPublishedOutControls; }
		
		/** Accessor to published Ports manager */
		PublishedInPorts& GetInPorts() { return mPublishedInPorts; }
		
		/** Accessor to published Portss manager */
		PublishedOutPorts& GetOutPorts() { return mPublishedOutPorts; }

		/** Returns a string describing configuration errors if any */
		const std::string& GetConfigErrorMessage() const { return mConfigErrorMessage; }
	
// Helpers only for subclasses
	protected:
		
		/** Method to prepend a message to mConfigErrorMessage
		* CLAM_ASSERT */
		void AddConfigErrorMessage( const std::string& msg );
	
		/** In debug-mode checks that the processing is configured and started. 
		 * And always checks it's not disabled */
		bool AbleToExecute(void) const;

		/**
		 * Helper template to convert a reference to a ProcessingConfig to the concrete
		 * ProcessingConfig specified on the first parameter.
		 * @param concrete The copy destination (it forces the runtime type for abstract)
		 * @param abstract A reference to the configuration to be copied
		 * @pre The object runtime type must be exactly the type required by the first parameter
		 */
		template <typename ConcreteConfig>
		void CopyAsConcreteConfig(ConcreteConfig & concrete, const ProcessingConfig & abstract) const;

// Attributes:
	protected:
		/** Pointer to the parent (composite) processing object, or 0 */
		ProcessingComposite *mpParent;

		/** Processing object execution state */
		ExecState mExecState;

		/** Status description, for debugging */
		std::string mConfigErrorMessage;


	private:
		PublishedInControls mPublishedInControls;
		PublishedOutControls mPublishedOutControls;
		PublishedInPorts mPublishedInPorts;
		PublishedOutPorts mPublishedOutPorts;
		bool  mPreconfigureExecuted;
	};

	

// -----------------------------------------------------------------------------------------------	
// Inline implementations

inline bool Processing::AbleToExecute(void) const
{
	CLAM_BEGIN_DEBUG_CHECK
		if (GetExecState() == Unconfigured ||
		    GetExecState() == Ready)
		{
			std::string err(GetClassName());
			err += ": Do(): Not in execution mode - did you call Start on this "
					"object, the composite it is in, or the ToplevelProcessing singleton?";
				CLAM_DEBUG_ASSERT( false, err.c_str() );
		}
	CLAM_END_DEBUG_CHECK
	return GetExecState() != Disabled;
}

template <typename ConcreteConfig>
inline void Processing::CopyAsConcreteConfig(ConcreteConfig & concrete, const ProcessingConfig & abstract) const 
{
	CLAM_ASSERT(typeid(ConcreteConfig)==typeid(abstract), 
		"Configuring a Processing with a configuration not being the proper type.");
	concrete = static_cast<const ConcreteConfig &>(abstract);
}

};//namespace CLAM

#endif

