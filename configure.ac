#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.5)
AC_INIT(frameInspector, 1.32, Michael_Uman@sdesigns.com)
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([src/viewApp.cpp])
AM_CONFIG_HEADER([config.h])
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
PKG_PROG_PKG_CONFIG

AM_OPTIONS_WXCONFIG()
AM_PATH_WXCONFIG(3.0.0, wxWin=1, wxWin=0, std)

if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWindows version >= 2.7.0 must be installed on the system
		but wx-config script could not be found.
	])
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_LIB([gcrypt], [gcry_md_open],
	[opt_gcrypt_installed=1],
	[opt_gcrypt_installed=0])

if test "$opt_gcrypt_installed" = "0"; then
	AC_MSG_ERROR([Please install libgcrypt11-dev])
else
	AC_DEFINE([HAVE_LIBGCRYPT], [1], [Define to '1' if libgcrypt is available])
	LIBS+=-lgcrypt
fi

# Check for FAM header
#AC_CHECK_HEADER([fam.h], opt_enable_fam=yes, opt_enable_fam=no)

AC_ARG_ENABLE([fam],
	[AS_HELP_STRING([--enable-fam],[Enable File Alteration monitor])],
	[opt_enable_fam=$enableval])

AC_ARG_ENABLE([optimize],
	[AS_HELP_STRING([--enable-optimize],[Enable optimizations])],
	[opt_optimize=$enableval],
	[opt_optimize=false])

AC_ARG_ENABLE([debug],
	[AS_HELP_STRING( [--enable-debug],[Enable debug messages])],
	[opt_debug_build=$enableval],
	[opt_debug_build=no])

AC_ARG_ENABLE([libswscale],
	[AS_HELP_STRING( [--enable-libswscale],[Enable ffmpeg libswscale library])],
	[opt_libswscale=$enableval],
	[opt_libswscale=no])

if test "$opt_libswscale" = yes; then
	PKG_CHECK_MODULES([LIBSWSCALE], [libswscale],
		[AC_DEFINE([HAVE_LIBSWSCALE], [1], [Define to '1' if libswscale is available])])
fi

if test "$opt_debug_build" = yes; then
	DEBUGFLAGS="-D_ENABLE_DEBUG -g3 -O0"
else
	DEBUGFLAGS="-DNDEBUG -g -O2"
fi

if test "$opt_enable_fam" = yes; then
	AC_DEFINE([HAVE_FAM], [1], [Define to '1' if FAM is available])
	AM_CONDITIONAL(INCLUDE_FAM, true)
	LIBS="-lfam"
else
	AM_CONDITIONAL(INCLUDE_FAM, false)
fi

if test "$opt_optimize" = yes; then
	OPTIMIZE="-march=pentium4 -msse2 -mfpmath=sse"
fi

CXXFLAGS="$OPTIMIZE $DEBUGFLAGS $WX_CXXFLAGS $LIBSWSCALE_CFLAGS -std=c++11"
LIBS="$WX_LIBS $LIBS $LIBSWSCALE_LIBS -lX11"

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
