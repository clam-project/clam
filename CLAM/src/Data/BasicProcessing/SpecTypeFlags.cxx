#include "SpecTypeFlags.hxx"

namespace CLAM
{
	Flags<4>::tFlagValue SpecTypeFlags::sFlagValues[] = {
		{SpecTypeFlags::eComplex, "Complex"},
		{SpecTypeFlags::ePolar, "Polar"},
		{SpecTypeFlags::eMagPhase, "MagPhase"},
		{SpecTypeFlags::eMagPhaseBPF, "MagPhaseBPF"},
		{0,NULL}
	};

	SpecTypeFlags::SpecTypeFlags()
	:
		Flags<4>(sFlagValues),
		bComplex(operator[](eComplex)),
		bPolar(operator[](ePolar)),
		bMagPhase(operator[](eMagPhase)),
		bMagPhaseBPF(operator[](eMagPhaseBPF))
	{
		bComplex = 0;
		bPolar = 0; 
		bMagPhase = 1;
		bMagPhaseBPF = 0;
	}

	SpecTypeFlags::SpecTypeFlags( const SpecTypeFlags &t) : 
		Flags<4>(sFlagValues,t),
		bComplex(operator[](eComplex)),
		bPolar(operator[](ePolar)),
		bMagPhase(operator[](eMagPhase)),
		bMagPhaseBPF(operator[](eMagPhaseBPF))
	{}
}
