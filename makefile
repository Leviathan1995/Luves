CC=g++
CFLAGS=-std=c++11 -fPIC -c
DFLAGS=-std=c++11 -shared
LIBLUVES=libluves.so

SOURCES=$(shell find luves -name "*.cpp")
OBJ=$(SOURCES:%.cpp=%.o)

build:$(LIBLUVES)
	echo "hello world"

$(LIBLUVES):$(OBJ)
	$(CC) $(DFLAGS) -o $@ $(OBJ)


luves/%.o: luves/%.cpp luves/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -fr *.o
