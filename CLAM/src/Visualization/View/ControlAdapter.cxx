#include "ControlAdapter.hxx"

namespace CLAMVM
{
		ControlAdapter::tManagedEmitter::tManagedEmitter()
				: mControl("OutControl"), mValueToEmit(0), mIsDirty( false )
		{
		}

		ControlAdapter::ControlAdapter()
				: mEmitter(), mIsBound( false )
		{
			UpdateValue.Wrap( this, &InputControlModel::DispatchControlValue );
		}

		ControlAdapter::~ControlAdapter()
		{
		}
		
		bool ControlAdapter::BindTo( InControl& adaptee )
		{
				mEmitter.mControl.AddLink( &adaptee );

				mIsBound = true;

				return mIsBound;
		}
		
		bool ControlAdapter::Publish()
		{
				ValuePublished.Emit( mEmitter.mValueToEmit );

				return true;
		}
		
		bool ControlAdapter::Update()
		{
				if ( mEmitter.mIsDirty )
				{
						mEmitter.mControl.SendControl( mEmitter.mValueToEmit );
						mEmitter.mIsDirty = false;
				}

				return true;
		}

		void ControlAdapter::DispatchControlValue( TControlData value )
		{
				mEmitter.mIsDirty = true;
				mEmitter.mValueToEmit = value;
		}

}
