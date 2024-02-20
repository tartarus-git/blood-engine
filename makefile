PLATFORM := LINUX_X64

COMPILER := clang++-16
OPTIMIZATION := -O0
COMPILER_PREAMBLE := $(COMPILER) $(OPTIMIZATION)

.PHONY: all lib test clean

all: lib test

lib: bin/lib/main/libblood.so bin/lib/main/blood_engine.so

test: bin/test

bin/lib/main/libblood.so: bin/lib/main/.dirstamp src/bindings/libblood.cpp
	$(COMPILER_PREAMBLE) -Wall -c src/bindings/libblood.cpp -o bin/lib/main/libblood.so

blood_engine_dll_sources := \
bin/lib/api.o \
bin/lib/context.o \
bin/lib/pipeline.o \
bin/lib/program.o

bin/lib/main/blood_engine.so: bin/lib/main/.dirstamp $(blood_engine_dll_sources)
	$(COMPILER_PREAMBLE) -Wall -shared -fvisibility=hidden $(blood_engine_dll_sources) -o bin/lib/main/blood_engine.so

$(shell $(COMPILER) -MM src/dll/api.cpp | aprepend --front bin/lib/ > .make_temp_file)
include .make_temp_file
bin/lib/api.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -Wall -c src/dll/api.cpp -o bin/lib/api.o

$(shell $(COMPILER) -MM src/dll/context.cpp | aprepend --front bin/lib/ > .make_temp_file)
include .make_temp_file
bin/lib/context.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -Wall -c src/dll/context.cpp -o bin/lib/context.o

$(shell $(COMPILER) -MM src/dll/pipeline.cpp | aprepend --front bin/lib/ > .make_temp_file)
include .make_temp_file
bin/lib/pipeline.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -Wall -c src/dll/pipeline.cpp -o bin/lib/pipeline.o

$(shell $(COMPILER) -MM src/dll/program.cpp | aprepend --front bin/lib/ > .make_temp_file)
include .make_temp_file
bin/lib/program.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -Wall -c src/dll/program.cpp -o bin/lib/program.o

bin/test: bin/test.o
	$(COMPILER_PREAMBLE) -Wall bin/test.o -o bin/test

$(shell $(COMPILER) -MM -I. test/main.cpp | aprepend --front bin/ > .make_temp_file)
include .make_temp_file
bin/test.o: bin/.dirstamp
	$(COMPILER_PREAMBLE) -Wall -I. -c test/main.cpp -o bin/test.o

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

bin/lib/.dirstamp: bin/.dirstamp
	mkdir -p bin/lib
	touch bin/lib/.dirstamp

bin/lib/main/.dirstamp: bin/.dirstamp bin/lib/.dirstamp
	mkdir -p bin/lib/main
	touch bin/lib/main/.dirstamp

clean:
	git clean -fdx -e "*.swp"
