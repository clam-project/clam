#ifndef __SIGNALV1__
#define __SIGNALV1__

#include "Functor1.hxx"
#include "Signal.hxx"
#include "Connection.hxx"

// MRJ: Argh! I hate this, but I hope at least this is a
// quite clear way to differentiate both implementations
#if defined(_MSC_VER) && (_MSC_VER < 1310)
#include "Signalv1ImplVC6.hxx"
#else
#include "Signalv1ImplSerious.hxx"
#endif


#endif // Signalv1.hxx
