
CC = g++
CFLAGS += -Wall

VPATH := $(VPATH)

HEADERS = $(patsubst %, -I%, $(subst :, , $(VPATH)))

%.o : %.c
	$(CC) $(CFLAGS) $(HEADERS) -c $< 

%.o : %.cpp
	$(CC) $(CFLAGS) $(HEADERS) -c $< 

OBJECTS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

$(BINARY) : $(OBJECTS)
	$(CC) -o $(BINARY) $(OBJECTS) $(LIBS)

binary: $(OBJECTS)
	$(CC) -o $(BINARY) $(OBJECTS) $(LIBS)
	-rm $(CLEAN) *.o

clean:
	-rm $(BINARY) $(CLEAN) *.o
