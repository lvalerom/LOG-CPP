SUBDIRS = common

lib_LTLIBRARIES = liblog.la

liblog_la_SOURCES = \
	full_log.cpp \
	full_log.h \
	log.cpp \
	log.h \
	simple_log.cpp \
	simple_log.h \
	trivial_log.cpp \
	trivial_log.h

liblog_la_LIBADD = \
	common/util/libutil.la \
	$(BOOST_THREAD_LIB) \
	$(BOOST_SYSTEM_LIB) \
	$(BOOST_LOG_LIB)

liblog_la_LDFLAGS = $(BOOST_LDFLAGS)

#liblog_la_HEADERS = \
#	log.h

#lib_LIBRARIES = libLog.a
#libLog_a_SOURCES =
#include_HEADERS =

#libLog_a_LDADD =
#libLog_a_CPPFLAGS =
#libLog_a_LIBADD = $(BOOSTLIB)
