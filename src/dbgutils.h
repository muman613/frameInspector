#ifndef	__DBGUTILS_H__
#define	__DBGUTILS_H__

#include <stdio.h>
#include <stdarg.h>

//#define _ENABLE_DEBUG		1

#ifdef	_WIN32
#include <windows.h>
#endif

#ifdef	_ENABLE_DEBUG

inline void debug(const char* sFmt, ...)
{
    va_list		valist;
    va_start(valist,sFmt);
#ifdef	_WIN32
    static char	dbgBuffer[1024];
    vsnprintf(dbgBuffer, 1024, sFmt, valist);
    OutputDebugStringA(dbgBuffer);
#else	// _WIN32
    vfprintf(stderr, sFmt, valist);
    fflush(stderr);
#endif	// _WIN32
}

#define	debug_function(x) debug("%s::%s\n", (x), __FUNCTION__)

#else	//	_ENABLE_DEBUG

inline void debug(const char* sFmt, ...)
{
}

#define debug_function(x)

#endif

#endif	// __DBGUTILS_H__
