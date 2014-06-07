
BINARY = wsr
CFLAGS += -DPROCESSOR_ARCHITECTURE=PAF_X86 -DOPERATING_SYSTEM=OSF_LINUX
VPATH += base datastructure resource

SOURCES = \
  base.cpp \
  base_ccondition.cpp \
  base_cguard.cpp \
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
  resource.cpp \
  resource_capplication.cpp \
  \
  thread_cloopthread.cpp \
  thread_cthread.cpp \
  \
  wsr.cpp \
  wsr_capplication.cpp \
  wsr_creader.cpp \
  wsr_cscraper.cpp \
  wsr_ctask.cpp \
  wsr_cwriter.cpp \
  \
  main.cpp

LIBS += -lpthread
CLEAN += emails.txt
  
include common.mk
