SUBDIRS = log

bin_PROGRAMS = main
main_SOURCES = main.cpp

AM_DLFLAGS = $(BOOST_LDFLAGS)

main_CPPFLAGS = \
	$(BOOST_CPPFLAGS) \
	"-DBOOST_LOG_DYN_LINK"

main_LDADD = \
		$(BOOST_THREAD_LIB) \
		$(BOOST_SYSTEM_LIB) \
		$(BOOST_LOG_LIB) \
		log/liblog.la
