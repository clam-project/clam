#include "EPhaseGeneration.hxx"

namespace CLAM
{
	Enum::tEnumValue EPhaseGeneration::sEnumValues[] = {
		{EPhaseGeneration::eAlign,"Align"},
		{EPhaseGeneration::eRandom,"Random"},
		{EPhaseGeneration::eContinuation,"Continuation"},
		{0,NULL}
	};
	
	Enum::tValue EPhaseGeneration::sDefault = EPhaseGeneration::eAlign;

}
