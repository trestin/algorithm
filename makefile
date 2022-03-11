# makefile for building leetcode

CXX	= g++
LINKER	   = g++
CFLAGS	  = -c -fPIC -g -Wall -DDEBUG -O2
LFLAGS     = -shared


.cpp.o:
	$(CXX) $(CFLAGS) -o $@ $<

TARGET = code.exe
TGTOBJ = hashmap.o trie.o minheap.o

$(TARGET) : $(TGTOBJ)
	$(LINKER) -lm -static -o $@ $^


all: $(TARGET)

clean :
	del *.o *.exe
