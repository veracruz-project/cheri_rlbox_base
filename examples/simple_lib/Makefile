.PHONY: bootstrap clean

include ../Makefile.inc

my_lib.wasm: lib.c lib.h
	$(WASI_SDK_ROOT)/bin/clang --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot $(RLBOX_ROOT)/c_src/wasm2c_sandbox_wrapper.c -c -o wasm2c_sandbox_wrapper.o
	$(WASI_SDK_ROOT)/bin/clang++ --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot/ lib.c wasm2c_sandbox_wrapper.o -o my_lib.wasm  $(WASI_SDK_FLAGS)


my_lib.wasm.c: my_lib.wasm
	$(WASM2C_ROOT)/bin/wasm2c my_lib.wasm -o my_lib.wasm.c
	
my_lib.so: my_lib.wasm.c 
	$(CC_PURE) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so  $(WASM2C_ADDINS)


my_lib_cheri.so: 
	$(CC_PURE) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so   $(WASM2C_ADDINS)


my_lib_hybrid.so:
	$(CC_HYBRID) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so  $(WASM2C_ADDINS)


app_sandboxed: my_lib.so
	$(CXX_PURE) -std=c++17 app_sandboxed.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

app_sandboxed_better: my_lib.so
	$(CXX_PURE) -std=c++17 app_sandboxed_better.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

app_sandboxed_cheri: 
	$(CXX_PURE) -std=c++17 app_sandboxed.cpp -o $@ $(RLBOX_INCLUDES) $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

app_sandboxed_better_cheri: 
	$(CXX_PURE) -std=c++17 app_sandboxed_better.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)


app_sandboxed_hybrid: 
	$(CXX_HYBRID) -std=c++17 app_sandboxed.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)

app_sandboxed_better_hybrid: 
	$(CXX_HYBRID) -std=c++17 app_sandboxed_better.cpp -o $@ $(RLBOX_INCLUDES)  $(WASM2C_INCLUDES) $(RLBOX_FLAGS)





my_lib.mswasm: lib.c lib.h
	$(MSWASM_CC) --target=wasm32-wasi --sysroot $(MSWASM_SYSROOT) $(RLBOX_ROOT)/c_src/wasm2c_sandbox_wrapper.c -c -o mswasm_sandbox_wrapper.o
	$(MSWASM_CC) --target=wasm32-wasi --sysroot $(MSWASM_SYSROOT) lib.c mswasm_sandbox_wrapper.o -o my_lib.mswasm  

my_lib.mswasm.c: my_lib.mswasm
	$(RWASM) $(RWASM_FLAGS) my_lib.mswasm  my_lib.mswasm

# my_lib_wasm2c.so: my_lib.wasm.c 
# 	$(CC) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so  $(WASM2C_ADDINS)
my_lib_mswasm_cheri.so: my_lib.mswasm.c 
	$(MORELLO_CC) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) --shared -fPIC my_lib.mswasm.c -o my_lib_mswasm_cheri.so

# Morello mswasm pure => pure
app_sandboxed_cheri_mswasm: my_lib_mswasm_cheri.so app_sandboxed.cpp
	$(MORELLO_CXX) $(MORELLO_CFLAGS) $(MORELLO_PURE_FLAGS) -DCHERI_MSWASM_SANDBOX -std=c++17  app_sandboxed.cpp -o $@ $(RLBOX_INCLUDES) $(RLBOX_FLAGS)




clean:
	-rm -f  my_lib.wasm
	-rm -f  my_lib.wasm.c 
	-rm -f my_lib.wasm.h
	-rm -f wasm2c_sandbox_wrapper.o
	-rm -f my_lib.so
	-rm -f app_sandboxed 
	-rm -f app_sandboxed_better
	rm -f my_lib.mswasm
	rm -f my_lib.mswasm.c
	rm -f my_lib.mswasm.h
	rm -f my_lib_mswasm_cheri.so
	rm -f app_sandboxed_cheri_mswasm
