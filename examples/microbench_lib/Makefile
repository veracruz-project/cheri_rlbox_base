.PHONY: clean

# Tool definitions
CC=clang 
CXX=clang++


# Tool-root definitions
# TODO: prebuilt morello-sdk binaries?
MORELLO_SDK_ROOT:=/home/evan/cheri/output/morello-sdk
RLBOX_ROOT:=../../cheri_rlbox

# Morello cross-compilation definitions
MORELLO_CC=$(MORELLO_SDK_ROOT)/bin/clang
MORELLO_CXX=$(MORELLO_SDK_ROOT)/bin/clang++
MORELLO_SYSROOT=$(MORELLO_SDK_ROOT)/sysroot-morello-purecap
# Currently compiles for cheribsd, might need extra work for linux
MORELLO_PURE_FLAGS=--target=aarch64-unknown-freebsd13 -march=morello+c64 -mabi=purecap --sysroot=$(MORELLO_SYSROOT) 
MORELLO_HYBRID_FLAGS=--target=aarch64-unknown-freebsd13 -march=morello --sysroot=$(MORELLO_SYSROOT) 
MORELLO_CFLAGS= -Wall -Wcheri -g 

# Rlbox definitions
RLBOX_INCLUDES=-I $(RLBOX_ROOT)/build/_deps/rlbox-src/code/include -I $(RLBOX_ROOT)/include


# Wasm definitions
WASI_SDK_ROOT:=$(RLBOX_ROOT)/build/_deps/wasiclang-src/
WASM2C_ROOT=./$(RLBOX_ROOT)/build/_deps/mod_wasm2c-src
WASM2C_ADDINS=$(WASM2C_ROOT)/wasm2c/wasm-rt-impl.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-unix.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-win.c $(WASM2C_ROOT)/wasm2c/wasm-rt-wasi.c -I$(WASM2C_ROOT)/wasm2c/
WASI_SDK_FLAGS=-Wl,--export-all -Wl,--growable-table -Wl,--no-entry


# CC_HYBRID=clang -march=morello
# CXX_HYBRID=clang++ -march=morello
# CC_PURE=cc
# CXX_PURE=c++

################### Native builds ####################

my_lib.wasm: lib.c lib.h
	$(WASI_SDK_ROOT)/bin/clang --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot $(RLBOX_ROOT)/c_src/wasm2c_sandbox_wrapper.c -c -o wasm2c_sandbox_wrapper.o
	$(WASI_SDK_ROOT)/bin/clang++ --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot/ lib.c wasm2c_sandbox_wrapper.o -o my_lib.wasm  $(WASI_SDK_FLAGS)

my_lib.wasm.c: my_lib.wasm
	$(WASM2C_ROOT)/bin/wasm2c my_lib.wasm -o my_lib.wasm.c
	
my_lib_wasm2c.so: my_lib.wasm.c 
	$(CC) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so  $(WASM2C_ADDINS)

my_lib.o: lib.c lib.h
	$(CC) lib.c -c -o my_lib.o

my_lib_native.so: lib.c lib.h 
	$(CXX) --shared -fPIC lib.c -o my_lib_native.so


microbenchmarks_wasm2c: my_lib_wasm2c.so microbenchmarks.cpp
	$(CXX) -std=c++17 -DWASM2C_SANDBOX microbenchmarks.cpp -o microbenchmarks_wasm2c $(RLBOX_INCLUDES) -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ -pthread -ldl

microbenchmarks_noop: my_lib.o microbenchmarks.cpp
	$(CXX) -std=c++17 -DNOOP_SANDBOX microbenchmarks.cpp my_lib.o -o microbenchmarks_noop $(RLBOX_INCLUDES) -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ -pthread -ldl 


#################### Cross-compiled Cheri builds ##############################

# Compile object in purecap mode for cheri-noop
my_lib_cheri.o: lib.c lib.h
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) lib.c -c -o my_lib_cheri.o

# Compile dynamic library in purecap mode for cheri-dylib
my_lib_cheri.so: lib.c lib.h 
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) --shared -fPIC lib.c -o my_lib_cheri.so

# 1a) Morello noop pure => pure
microbenchmarks_cheri_pure_noop: my_lib_cheri.o microbenchmarks.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -std=c++17 -DCHERI_NOOP_SANDBOX microbenchmarks.cpp my_lib_cheri.o -o $@ $(RLBOX_INCLUDES) -pthread -ldl 

# # 1b) Morello noop hybrid => pure
# microbenchmarks_cheri_hybrid_noop: my_lib_cheri.o microbenchmarks.cpp
# 	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_HYBRID_FLAGS) -std=c++17 -DCHERI_NOOP_SANDBOX microbenchmarks.cpp my_lib_cheri.o -o $@ $(RLBOX_INCLUDES) -pthread -ldl 

# 2a) Morello dylib pure => pure
microbenchmarks_cheri_pure_dylib: my_lib_cheri.so microbenchmarks.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -std=c++17 -DCHERI_DYLIB_SANDBOX microbenchmarks.cpp -o $@ $(RLBOX_INCLUDES) -pthread -ldl

# 2a) Morello dylib hybrid => pure
microbenchmarks_cheri_hybrid_dylib: my_lib_cheri.so microbenchmarks.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_HYBRID_FLAGS) -std=c++17 -DCHERI_DYLIB_SANDBOX microbenchmarks.cpp -o $@ $(RLBOX_INCLUDES) -pthread -ldl

cheri_libs: microbenchmarks_cheri_pure_noop microbenchmarks_cheri_pure_dylib microbenchmarks_cheri_hybrid_dylib


clean:
	-rm -f my_lib.wasm 
	-rm -f my_lib.wasm.c 
	-rm -f my_lib.wasm.h
	-rm -f my_lib_wasm.so
	-rm -f my_lib_native.so
	-rm -f my_lib.o
	-rm -f microbenchmarks_wasm2c
	-rm -f microbenchmarks_noop
	-rm -f my_lib_cheri.o
	-rm -f my_lib_cheri.so
	-rm -f microbenchmarks_cheri_pure_noop 
	-rm -f microbenchmarks_cheri_pure_dylib 
	-rm -f microbenchmarks_cheri_hybrid_noop 
	-rm -f microbenchmarks_cheri_hybrid_dylib 
