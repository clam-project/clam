#include "TestSignalv1Classes.hxx"
#include <iostream>

namespace CLAMTest
{

		// Signaler class

		void Signaler::Simulate()
		{
				mNewInteger.Emit( 33 );
		}

		Signalv1<int>& Signaler::GetNewIntegerSignal()
		{
			return mNewInteger;
		}

// Slotted class

		void Slotted::sReact( int value )
		{
			smNotified++;
			std::cout << "Value received " << value << std::endl;
		}

		void Slotted::PureVirtualReact( int value )
		{
			mPureVirtualNotified++;
			std::cout << "Value received" << value << std::endl;
		}

		void Slotted::React( int value )
		{
			mNotified++;
			std::cout << "Value received " << value << std::endl;
		}

		void Slotted::LinkWith( Signaler& sig )
		{
				mSignalSlot.Bind( sig.GetNewIntegerSignal().Connect( this, &Slotted::React ) );
				mStaticSignalSlot.Bind( sig.GetNewIntegerSignal().Connect( &Slotted::sReact ) );
				mPureVirtualSignalSlot.Bind( sig.GetNewIntegerSignal().Connect( this, &Slotted::PureVirtualReact ) );
		}

		bool Slotted::WasCorrectlyNotified()
		{
			return (mNotified==1) 
					&& (smNotified==1) 
					&& (mPureVirtualNotified==1);
		}

		int Slotted::smNotified = 0;

// SlottedMultipleSignals class

		bool SlottedMultipleSignals::WasCorrectlyNotified()
		{
			return ( mNotified == mNumOfExpectedCalls )
				&& ( smNotified == mNumOfExpectedCalls )
				&& ( mPureVirtualNotified == mNumOfExpectedCalls );
		}


		void SlottedMultipleSignals::LinkWith( tSignalerVector& signalers )
		{
			smNotified = 0;
			tSignalerIterator i = signalers.begin();
			tSignalerIterator end = signalers.end();

			mNumOfExpectedCalls = signalers.size();

			while( i != end )
			{
				mSignalSlot.Bind( i->GetNewIntegerSignal().Connect( this, &Slotted::React ) );
				mStaticSignalSlot.Bind( i->GetNewIntegerSignal().Connect( &Slotted::sReact ) );
				mPureVirtualSignalSlot.Bind( i->GetNewIntegerSignal().Connect( this, &Slotted::PureVirtualReact ) );

				i++;
			}

		}

}

