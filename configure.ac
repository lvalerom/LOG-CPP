dnl Process this file with autoconf to produce a configure script.

# Require a minimum Autoconf version
AC_PREREQ([2.69])

# Autoconf initialization
AC_INIT([blalog],[0.1])

# Auxiliary scripts should be in config/
AC_CONFIG_AUX_DIR([config])
# Safety source file check.
AC_CONFIG_SRCDIR([src/main.cpp])
# m4 macros should be in m4/
AC_CONFIG_MACRO_DIR([m4])

#AX_BOOST_BASE([1.59],, [AC_MSG_ERROR([blalog needs Boost])])
#AX_BOOST_SYSTEM
#AX_BOOST_FILESYSTEM

AM_PROG_AR

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# init libtool
LT_INIT

# Check for programs
# C++ compiler
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_MKDIR_P

AX_BOOST_BASE([1.59],[],[AC_MSG_ERROR([Boost needed...])])
AX_BOOST_LOG
AX_BOOST_LOG_SETUP
AX_BOOST_THREAD
AX_BOOST_SYSTEM

# Check for libraries

# AC_CHECK_LIB(lib, funct, [],[])

# Check for header files

# Check for typedefs, struct, and compiler characteristics.

# Output files

# config.h file
AC_CONFIG_HEADERS([src/config.h])

AC_CONFIG_FILES([
Makefile
src/Makefile
])

# blalog output files
AC_CONFIG_FILES([
src/log/Makefile
src/log/common/Makefile
src/log/common/util/Makefile
src/log/common/util/RapidXml/Makefile
])

AC_OUTPUT
