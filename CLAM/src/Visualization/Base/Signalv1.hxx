#ifndef __SIGNALV1__
#define __SIGNALV1__

#include "Functor1.hxx"
#include "Signal.hxx"
#include "Connection.hxx"

// MRJ: Argh! I hate this, but I hope at least this is a
// quite clear way to differentiate both implementations
#if (_MSC_VER >= 1310)||defined( __GNUC__ )
#include "Signalv1ImplSerious.hxx"
#else
#include "Signalv1ImplVC6.hxx"
#endif


#endif // Signalv1.hxx
