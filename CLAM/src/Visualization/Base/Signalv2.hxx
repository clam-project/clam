#ifndef __SIGNALV2__
#define __SIGNALV2__

#include "Functor2.hxx"
#include "Signal.hxx"
#include "Connection.hxx"
#include "Slot.hxx"

#if defined( _MSC_VER )&&(_MSC_VER < 1310)
#include "Signalv2ImplVC6.hxx"
#else
#include "Signalv2ImplSerious.hxx"
#endif


#endif //Signalv2.hxx
