CC = gcc
CXX = g++
DEFINES =
CFLAGS = -pipe -O2 -Wall  -Wno-deprecated -D_REENTRANT  -std=c++11
CXXFLAGS = -pipe -O2 -Wall  -Wno-deprecated -D_REENTRANT -std=c++11
COPY = cp -f
SHOW = ls -al
INCLUDE = -I .

TARGET = example

LIBPATH = ./

LDFLAGS = 


LIBS =  -lpthread

OBJS = 	main.o \
	TimerThread.o

first: all

$(TARGET):$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)
	$(SHOW) $(TARGET)

all: Makefile $(TARGET)

main.o : ./main.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $^

TimerThread.o : ./TimerThread.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $^

clean:
	rm -f *.o
	#rm -f $(LIBPATH)$(TARGET)
	#rm -f $(RELEASEPATH)$(TARGET)
