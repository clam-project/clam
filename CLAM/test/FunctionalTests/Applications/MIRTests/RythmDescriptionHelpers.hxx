#ifndef __RYTHMDESCRIPTIONHELPERS__
#define __RYTHMDESCRIPTIONHELPERS__

#include "DynamicType.hxx"
#include "Text.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "Pulse.hxx"
#include "Audio.hxx"

namespace CLAMTest
{
	namespace RythmDescriptionTests
	{

		class RythmEventsB2B
			: public CLAM::DynamicType
		{
			DYNAMIC_TYPE( RythmEventsB2B, 2 );
			DYN_ATTRIBUTE( 0, public, CLAM::Text,      RelativePath );
			DYN_ATTRIBUTE( 1, public, CLAM::Pulse,     Events );

		protected:
			void DefaultInit();
			
		public:
			
			bool IsValid() const;
		};

		class IOIHistogramB2B
			: public CLAM::DynamicType
		{
			DYNAMIC_TYPE( IOIHistogramB2B, 2 );
			DYN_ATTRIBUTE( 0, public, CLAM::Text,  RelativePath );
			DYN_ATTRIBUTE( 1, public, CLAM::Audio, IOIHistogram );
			
		protected:
			
			void DefaultInit();

		public:

			bool IsValid() const;
		};

	}
}

#endif // RythmDescriptionHelpers.hxx
