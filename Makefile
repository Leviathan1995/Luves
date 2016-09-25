CC=g++
CFLAGS=-std=c++11
AR=ar
LIBLUVES=libluves.a

SOURCES=$(shell find . -name "*.cpp")
HEADERS=$(shell find . -name "*.h")
OBJ=$(SOURCES:%.cpp=%.o)

build:$(LIBLUVES)
	echo "hello"

$(LIBLUVES):$(OBJ)
	AR -rs $@ $<

%.o:%.cpp %.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -fr *.o
