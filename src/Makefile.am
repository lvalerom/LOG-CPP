SUBDIRS = log

bin_PROGRAMS = main
main_SOURCES = main.cpp

AM_DLFLAGS = $(BOOST_LDFLAGS)

main_LDADD = log/liblog.la \
		$(BOOST_THREAD_LIB) \
		$(BOOST_SYSTEM_LIB) \
		$(BOOST_LOG_LIB)
