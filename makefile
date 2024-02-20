PLATFORM := LINUX_X64

COMPILER := clang++-16
OPTIMIZATION := -O0
COMPILER_PREAMBLE := $(COMPILER) $(OPTIMIZATION)

.PHONY: all lib test clean

all: lib test

lib: bin/lib/libblood.so bin/lib/blood_engine.o

test: bin/test

bin/lib/libblood.so: bin/lib/.dirstamp src/lib/so/libblood.cpp
	$(COMPILER_PREAMBLE) -Wall -c src/lib/so/libblood.cpp -o bin/lib/libblood.so

#bin/lib/blood_engine.o:

bin/test: bin/test.o
	$(COMPILER_PREAMBLE) -Wall bin/test.o -o bin/test

bin/test.o: bin/.dirstamp src/test/main.cpp blood.h
	$(COMPILER_PREAMBLE) -Wall -I. src/test/main.cpp -o bin/test.o

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

bin/lib/.dirstamp: bin/.dirstamp
	mkdir -p bin/lib
	touch bin/lib/.dirstamp

clean:
	git clean -fdx -e "*.swp"
