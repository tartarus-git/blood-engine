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
bin/lib/context.o

bin/lib/main/blood_engine.so: bin/lib/main/.dirstamp $(blood_engine_dll_sources)
	$(COMPILER_PREAMBLE) -Wall -shared -fvisibility=hidden $(blood_engine_dll_sources) -o bin/lib/main/blood_engine.so

$(eval bin/lib/$(shell $(COMPILER) -MM src/dll/api.cpp))
bin/lib/api.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -Wall -c src/dll/api.cpp -o bin/lib/api.o

$(eval bin/lib/$(shell $(COMPILER) -MM src/dll/context.cpp))
bin/lib/context.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -Wall -c src/dll/context.cpp -o bin/lib/context.o

bin/test: bin/test.o
	$(COMPILER_PREAMBLE) -Wall bin/test.o -o bin/test

$(eval bin/$(shell $(COMPILER) -MM -I. test/main.cpp))
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
