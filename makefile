CC=g++
CFLAGS=-std=c++11
AR=ar
LIBLUVES=libluves.a

SOURCES=$(shell find luves -name "*.cpp")
HEADERS=$(shell find luves -name "*.h")
OBJ=$(SOURCES:%.cpp=%.o)

build:$(LIBLUVES)
	echo "hello world"

$(LIBLUVES):$(OBJ)
	AR -crv $@ $(OBJ)

$(OBJ):$(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -fr luves/*.o
