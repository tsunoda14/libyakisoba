lib_LTLIBRARIES = libyakisoba.la

include_HEADERS = yakisoba.h

dist_sysconf_DATA = bcas_keys

libyakisoba_la_SOURCES = \
	Crypto.c \
	Decoder-ECM.c \
	Decoder-EMM.c \
	Keyset.c \
	Global.h \
	Crypto.h \
	Keyset.h \
	yakisoba.h

libyakisoba_la_LDFLAGS = -export-symbols-regex ^bcas_ -version-info 0:1:0

AM_TESTS_ENVIRONMENT = BCAS_KEYS_FILE=$(srcdir)/bcas_keys

TESTS = test-ecm

check_PROGRAMS = test-ecm
test_ecm_SOURCES = test-ecm.c yakisoba.h
test_ecm_LDADD = libyakisoba.la
