CC=g++
CFLAGS=-std=c++11 -fPIC -c
DFLAGS=-std=c++11 -shared
LIBLUVES=libluves.so

SOURCES=$(shell find luves -name "*.cpp")
OBJ=$(SOURCES:%.cpp=%.o)
PREFIX=luves

build:$(LIBLUVES)
	cp -fr luves /usr/local/include
	cp $(LIBLUVES) /usr/local/lib
	rm $(PREFIX)/*.o $(LIBLUVES)

$(LIBLUVES):$(OBJ)
	$(CC) $(DFLAGS) -o $@ $(OBJ)

$(PREFIX)/%.o: $(PREFIX)/%.cpp $(PREFIX)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(LIBLUVES)
