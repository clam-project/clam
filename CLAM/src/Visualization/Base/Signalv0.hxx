#ifndef __SIGNALV0__
#define __SIGNALV0__

#include "Functor0.hxx"
#include "Signal.hxx"
#include "Connection.hxx"
#include "Slot.hxx"

// MRJ: Argh! I hate this, but I hope at least this is a
// quite clear way to differentiate both implementations
#if defined(_MSC_VER) && (_MSC_VER < 1310)
#include "Signalv0ImplVC6.hxx"
#else
#include "Signalv0ImplSerious.hxx"
#endif


#endif // Signalv0.hxx
