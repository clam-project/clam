#ifndef __strfuncs__
#define __strfuncs__

/* a more efficient way of catting strings than strcat */

/* set the ptr where we will be adding, and the max. length */
CLAM_DLL_EXPORT void  strstart(char* tgt,int n);

CLAM_DLL_EXPORT char* stradd(const char* a);

CLAM_DLL_EXPORT char* stradd_range( const char* begin, const char* end );

CLAM_DLL_EXPORT void strend(void);

/* Converts a given path from the UNIX way: a/b/c into 
 * Windows' way, namely a\b\c
 */
CLAM_DLL_EXPORT void winstyle(char* str);

#endif
