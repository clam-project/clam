#ifndef __CONTROLADAPTER__
#define __CONTROLADAPTER__

#include "InControl.hxx"
#include "OutControl.hxx"
#include "ModelController.hxx"
#include "InputControlModel.hxx"

namespace CLAMVM
{
		using CLAM::InControl;
		using CLAM::OutControl;
		using CLAM::TControlData;
		
		class ControlAdapter
				: public  ModelController, public  InputControlModel
		{
		private:

				struct tManagedEmitter
				{
				
						OutControl              mControl;
						TControlData            mValueToEmit;
						bool                    mIsDirty;
						
						tManagedEmitter();
						
						void Emit(  );
		
				};
		
				tManagedEmitter         mEmitter;
				bool                    mIsBound;
				bool                    mValueRangeChanged;
				TControlData            mMinValue;
				TControlData            mMaxValue;
		protected:
		
				void DispatchControlValue( TControlData value );

				void DeleteSubAdapters();

		public:
				ControlAdapter();

				virtual ~ControlAdapter();
				
				virtual const char* GetClassName() const 
				{
						return "ControlAdapter";
				}
				
				virtual bool Publish();

				virtual bool Update();

				void SetValueRange( TControlData min, TControlData max )
				{
						mMinValue = min;
						mMaxValue = max;
						mValueRangeChanged = true;
				}

				virtual bool BindTo( InControl&  adaptee );
				
				bool IsBound() const
				{
						return mIsBound;
				}


		};

}

#endif // ControlAdapter.hxx
