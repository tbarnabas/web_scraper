
BINARY = wsr

CFLAGS += -DPROCESSOR_ARCHITECTURE=PAF_X86 -DOPERATING_SYSTEM=OSF_LINUX

VPATH += base thread

LIBS += -lpthread

SOURCES = \
  base.cpp \
  base_cipaddress.cpp \
  base_cmessage.cpp \
  base_cmutex.cpp \
  base_cobject.cpp \
  base_creference.cpp \
  base_cstring.cpp \
  base_ctime.cpp \
  base_eexception.cpp \
  \
  datastructure.cpp \
  datastructure_crawstream.cpp \
  datastructure_cstream.cpp \
  datastructure_ctextstream.cpp \
  \
  thread.cpp \
  thread_ccondition.cpp \
  thread_cguard.cpp \
  thread_cloopthread.cpp \
  thread_cmutex.cpp \
  thread_cobject.cpp \
  thread_cthread.cpp \
  \
  wsr.cpp \
  \
  main.cpp
  
include common.mk
