#ifndef __INPUTCONTROLMODEL__
#define __INPUTCONTROLMODEL__

#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "Slotv1.hxx"
#include "OutControl.hxx"

namespace CLAMVM
{
		using SigSlot::Signalv1;
		using SigSlot::Slotv1;
		using CLAM::TControlData;

		class InputControlModel
		{
		protected:

			virtual void DispatchControlValue( TControlData value ) = 0;

		public:

				InputControlModel( );

				virtual ~InputControlModel();

				// Signals
				Signalv1< TControlData >                  ValuePublished;
				Signalv1< TControlData, TControlData >    ValueRangePublished;
				
				// Slots
				Slotv1< TControlData>                     UpdateValue;

		};
}

#endif // InputControlModel.hxx
