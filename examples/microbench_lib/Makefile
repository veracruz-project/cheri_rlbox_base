.PHONY: clean

include ../Makefile.inc

################### Native builds ####################

my_lib.wasm: lib.c lib.h
	$(WASI_SDK_ROOT)/bin/clang --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot $(RLBOX_ROOT)/c_src/wasm2c_sandbox_wrapper.c -c -o wasm2c_sandbox_wrapper.o
	$(WASI_SDK_ROOT)/bin/clang++ --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot/ lib.c wasm2c_sandbox_wrapper.o -o my_lib.wasm  $(WASI_SDK_FLAGS)

my_lib.wasm.c: my_lib.wasm
	$(WASM2C_ROOT)/bin/wasm2c my_lib.wasm -o $@
	
my_lib_wasm2c.so: my_lib.wasm.c 
	$(CC) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o $@  $(WASM2C_ADDINS)

my_lib.o: lib.c lib.h
	$(CC) lib.c -c -o $@

my_lib_native.so: lib.c lib.h 
	$(CXX) --shared -fPIC lib.c -o $@


microbenchmarks_wasm2c: my_lib_wasm2c.so microbenchmarks.cpp
	$(CXX) -std=c++17 -DWASM2C_SANDBOX microbenchmarks.cpp -o $@ $(RLBOX_INCLUDES) -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ $(RLBOX_FLAGS)

microbenchmarks_noop: my_lib.o microbenchmarks.cpp
	$(CXX) -std=c++17 -DNOOP_SANDBOX microbenchmarks.cpp my_lib.o -o $@ $(RLBOX_INCLUDES) -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ $(RLBOX_FLAGS)


#################### Cross-compiled Cheri builds ##############################

# Compile object in purecap mode for cheri-noop
my_lib_cheri.o: lib.c lib.h
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) lib.c -c -o my_lib_cheri.o

# Compile dynamic library in purecap mode for cheri-dylib
my_lib_cheri.so: lib.c lib.h 
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS)  --shared -fPIC lib.c -o my_lib_cheri.so

# 1) Morello noop pure => pure
microbenchmarks_cheri_pure_noop: my_lib_cheri.o microbenchmarks.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -std=c++17 -DCHERI_NOOP_SANDBOX microbenchmarks.cpp my_lib_cheri.o -o $@ $(RLBOX_INCLUDES) $(RLBOX_FLAGS)

# 2) Morello dylib pure => pure
microbenchmarks_cheri_pure_dylib: my_lib_cheri.so microbenchmarks.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -std=c++17 -DCHERI_DYLIB_SANDBOX microbenchmarks.cpp -o $@ $(RLBOX_INCLUDES) $(RLBOX_FLAGS)

cheri_libs: microbenchmarks_cheri_pure_noop microbenchmarks_cheri_pure_dylib microbenchmarks_cheri_hybrid_dylib



#########################Cross-compiled Cheri MSWasm build ##########################

my_lib.mswasm: lib.c lib.h
	$(C_TO_MSWASM) $(RLBOX_ROOT)/c_src/wasm2c_sandbox_wrapper.c -c -o mswasm_sandbox_wrapper.o
	$(C_TO_MSWASM) lib.c mswasm_sandbox_wrapper.o -o $@  

my_lib.mswasm.c: my_lib.mswasm
	$(RWASM) $(RWASM_FLAGS) my_lib.mswasm  my_lib.mswasm

my_lib_mswasm_cheri.so: my_lib.mswasm.c 
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -lm --shared -fPIC my_lib.mswasm.c -o $@

# Morello mswasm pure => pure
microbenchmarks_cheri_mswasm: my_lib_mswasm_cheri.so microbenchmarks.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -std=c++17 -DCHERI_MSWASM_SANDBOX microbenchmarks.cpp -o $@ $(RLBOX_INCLUDES) $(RLBOX_FLAGS)



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
	-rm -f mswasm_sandbox_wrapper.o
	-rm -f my_lib.mswasm
	-rm -f my_lib.mswasm.c
	-rm -f my_lib.mswasm.h
	-rm -f my_lib_mswasm_cheri.so
	-rm -f microbenchmarks_cheri_mswasm

