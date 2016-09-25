CC=g++
CFLAGS=-std=c++11
AR=ar
LIBLUVES=libluves.a

SOURCES=$(shell find luves -name "*.cpp")
HEADERS=$(shell find luves -name "*.h")
OBJ=$(SOURCES:%.cpp=%.o)
SRCPATH=luves/

build:$(LIBLUVES)
	echo "hello"

$(LIBLUVES):$(OBJ)
	AR -rs $@ $<

$(SRCPATH)%.o:$(SRCPATH)%.cpp $(SRCPATH)%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -fr luves/*.o
