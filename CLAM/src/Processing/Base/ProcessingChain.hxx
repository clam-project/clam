#include "ProcessingComposite.hxx"
#include "ProcessingData.hxx"
#include "Port.hxx"


#ifndef _ProcessingComposite_
#define _ProcessingComposite_

namespace CLAM{

	//if proved valid this should go to Processing.hxx
	typedef std::list<ProcessingConfig*>::iterator config_iterator;
	
	//template <typename T>
	class ProcessingChainConfig:public ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE (ProcessingChainConfig, 2,ProcessingConfig);
		/** Name of the SMSTransformation object*/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** Array of children configurations */
		DYN_ATTRIBUTE (1, public, std::list<ProcessingConfig*>, Configurations);

		void DefaultInit()
		{
			AddAll();
			UpdateData();
		}
	public:
		~ProcessingChainConfig(){}
		
	};
	

	template <typename Data>
	class ProcessingChain:public ProcessingComposite
	{
	public:
		ProcessingChain():mChainInput("Input",this,1),mChainOutput("Output",this,1),mTmpData()
		{
			mArePortsAttached=false;
		}

		~ProcessingChain()
		{
		
		}

		bool ConcreteStart()
		{
			iterator obj=composite_begin();
			(*(*obj)->FirstInput())->Attach(mChainInput.GetData());
			obj=composite_end();
			obj--;
			(*(*obj)->FirstOutput())->Attach(mChainOutput.GetData());
			
			mTmpData=mChainInput.GetData();
			
			return true;
		}

		bool Do()
		{
			CLAM_DEBUG_ASSERT(GetExecState() != Unconfigured &&
		                  GetExecState() != Ready,
		                  "ProcessingChain: Do(): Not in execution mode");

			if (GetExecState() == Disabled)
				return true;
			
			//Before doing this, Ports should be connected, check!
			bool result=true;
			iterator obj;
			for (obj=composite_begin(); obj!=composite_end(); obj++)
			{
				try {
					result&=(*obj)->Do();
				}
				catch (Err &e)
				{
					if ((*obj)->GetExecState() != Running)
						throw(ErrProcessingObj("Do(): Child is not running",this));
					else
						throw e;
				}
				
			}
			return result;
		}
		
		bool Do(const Data& in, Data& out)
		{
			iterator obj;
			bool result=true;
			mTmpData=out;
			const Data *pIn;
			for (obj=composite_begin(); obj!=composite_end(); obj++)
			{
				if(obj==composite_begin()) pIn=&in;
				else pIn=&mTmpData;
				try {
					result&=(U*>(*obj))->Do(*pIn,mTmpData);
				}
				catch (Err &e)
				{
					if ((*obj)->GetExecState() != Running)
						throw(ErrProcessingObj("Do(in,out): Child is not running",this));
					else
						throw e;
				}
				
			}
			out=mTmpData;
			return result;
		}

		bool ConcreteConfigure(const ProcessingConfig& c)
		{
			mConfig=dynamic_cast<const ProcessingChainConfig&>(c);
			CLAM_ASSERT(mConfig.GetConfigurations().size()==composite_size(),"Number of configurations should be the same as number of children");
			bool result=true;
			
			iterator obj;
			config_iterator cfg=mConfig.GetConfigurations().begin();
			for(obj=composite_begin();obj!=composite_end();obj++,cfg++)
			{
				result&=(*obj)->Configure(*(*cfg));
				//connecting ports for non-supervised mode
				(*(*obj)->FirstInput())->Attach(mTmpData);
				(*(*obj)->FirstOutput())->Attach(mTmpData);
			}

			return result;
		}

		const ProcessingConfig& GetConfig() const
		{
			return mConfig;
		}
		InPortTmpl<Data>     mChainInput;
		OutPortTmpl<Data> mChainOutput;
	protected:
		ProcessingChainConfig mConfig;
		Data mTmpData;
		
		bool mArePortsAttached;
	};



};//namespace

#endif