// -*-C++-*- boost.cpp
// <!!----------------------------------------------------------------------> 
// <!! Copyright (C) 1998 Dietmar Kuehl, Claas Solutions GmbH > 
// <!!> 
// <!! Permission to use, copy, modify, distribute and sell this > 
// <!! software for any purpose is hereby granted without fee, provided > 
// <!! that the above copyright notice appears in all copies and that > 
// <!! both that copyright notice and this permission notice appear in > 
// <!! supporting documentation. Dietmar Kuehl and Claas Solutions make no > 
// <!! representations about the suitability of this software for any > 
// <!! purpose. It is provided "as is" without express or implied warranty. > 
// <!!----------------------------------------------------------------------> 

// Author: Dietmar Kuehl dietmar.kuehl@claas-solutions.de 
// Title:  Entry point for the DLL (this is only used for the WinNT target)

// -------------------------------------------------------------------------- 

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "boost.h"

BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
	// This function currently does nothing. This might change in the future.
	return TRUE;
}
