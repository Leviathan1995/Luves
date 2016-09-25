CC=g++
CFLAGS=-std=c++11 -fPIC -c
DFLAGS=-std=c++11 -shared
LIBLUVES=libluves.so

SOURCES=$(shell find luves -name "*.cpp")
OBJ=$(SOURCES:%.cpp=%.o)
PREFIX=luves

build:$(LIBLUVES)
	echo "hello world"
	rm $(PREFIX)/*.o

$(LIBLUVES):$(OBJ)
	$(CC) $(DFLAGS) -o $@ $(OBJ)


$(PREFIX)/%.o: $(PREFIX)/%.cpp $(PREFIX)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(PREFIX)/*.o
