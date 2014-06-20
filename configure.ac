#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([linthesia], [0.5.0], [victor.lavaud@gmail.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
#AM_LANG([C++])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

LT_PREREQ([2.2])
LT_INIT()

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MKDIR_P
# Checks for libraries.

PKG_CHECK_MODULES(RTMIDI, rtmidi)

# Checks for header files.
AC_CHECK_HEADERS([string string.h iostream algorithm locale vector sstream queue functional map iomanip set exception sys/time.h])

AC_LANG_PUSH([C++])
AC_CHECK_HEADER(
    [SFML/Config.hpp],
    [],
    [AC_MSG_ERROR([SFML headers not found.])]
)
AC_LANG_POP([C++])

CXXFLAGS="$CXXFLAGS -std=c++11"

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
