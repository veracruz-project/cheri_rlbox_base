.PHONY: bootstrap clean

RLBOX_ROOT:=../../cheri_rlbox
WASI_SDK_ROOT:=$(RLBOX_ROOT)/build/_deps/wasiclang-src/
WASM2C_ROOT=./$(RLBOX_ROOT)/build/_deps/mod_wasm2c-src

WASI_SDK_FLAGS=-Wl,--export-all -Wl,--growable-table -Wl,--no-entry

CC_HYBRID=clang -march=morello
CXX_HYBRID=clang++ -march=morello
CC_PURE=cc
CXX_PURE=c++

my_lib.wasm: lib.c lib.h
	$(WASI_SDK_ROOT)/bin/clang --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot $(RLBOX_ROOT)/c_src/wasm2c_sandbox_wrapper.c -c -o wasm2c_sandbox_wrapper.o
	$(WASI_SDK_ROOT)/bin/clang++ --sysroot $(WASI_SDK_ROOT)/share/wasi-sysroot/ lib.c wasm2c_sandbox_wrapper.o -o my_lib.wasm  $(WASI_SDK_FLAGS)


my_lib.wasm.c: my_lib.wasm
	$(WASM2C_ROOT)/bin/wasm2c my_lib.wasm -o my_lib.wasm.c
	
my_lib.so: my_lib.wasm.c 
	$(CC_PURE) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so  $(WASM2C_ROOT)/wasm2c/wasm-rt-impl.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-unix.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-win.c $(WASM2C_ROOT)/wasm2c/wasm-rt-wasi.c -I$(WASM2C_ROOT)/wasm2c/


my_lib_cheri.so: 
	$(CC_PURE) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so  $(WASM2C_ROOT)/wasm2c/wasm-rt-impl.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-unix.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-win.c $(WASM2C_ROOT)/wasm2c/wasm-rt-wasi.c -I$(WASM2C_ROOT)/wasm2c/


my_lib_hybrid.so:
	$(CC_HYBRID) -I$(WASM2C_ROOT) -shared -fPIC -O3 my_lib.wasm.c -o my_lib.so  $(WASM2C_ROOT)/wasm2c/wasm-rt-impl.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-unix.c $(WASM2C_ROOT)/wasm2c/wasm-rt-os-win.c $(WASM2C_ROOT)/wasm2c/wasm-rt-wasi.c -I$(WASM2C_ROOT)/wasm2c/


app_sandboxed: my_lib.so
	$(CXX_PURE) -std=c++17 app_sandboxed.cpp -o app_sandboxed -I $(RLBOX_ROOT)/build/_deps/rlbox-src/code/include -I $(RLBOX_ROOT)/include -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ -lpthread -ldl

app_sandboxed_better: my_lib.so
	$(CXX_PURE) -std=c++17 app_sandboxed_better.cpp -o app_sandboxed_better -I $(RLBOX_ROOT)/build/_deps/rlbox-src/code/include -I $(RLBOX_ROOT)/include -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ -lpthread -ldl

app_sandboxed_cheri: 
	$(CXX_PURE) -std=c++17 app_sandboxed.cpp -o app_sandboxed -I $(RLBOX_ROOT)/build/_deps/rlbox-src/code/include -I $(RLBOX_ROOT)/include -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ -lpthread -ldl

app_sandboxed_better_cheri: 
	$(CXX_PURE) -std=c++17 app_sandboxed_better.cpp -o app_sandboxed_better -I $(RLBOX_ROOT)/build/_deps/rlbox-src/code/include -I $(RLBOX_ROOT)/include -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ -lpthread -ldl


app_sandboxed_hybrid: 
	$(CXX_HYBRID) -std=c++17 app_sandboxed.cpp -o app_sandboxed -I $(RLBOX_ROOT)/build/_deps/rlbox-src/code/include -I $(RLBOX_ROOT)/include -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ -lpthread -ldl

app_sandboxed_better_hybrid: 
	$(CXX_HYBRID) -std=c++17 app_sandboxed_better.cpp -o app_sandboxed_better -I $(RLBOX_ROOT)/build/_deps/rlbox-src/code/include -I $(RLBOX_ROOT)/include -I $(RLBOX_ROOT)/build/_deps/mod_wasm2c-src/wasm2c/ -lpthread -ldl

clean:
	-rm my_lib.wasm
	-rm my_lib.wasm.c 
	-rm my_lib.wasm.h
	-rm wasm2c_sandbox_wrapper.o
	-rm my_lib.so
	-rm app_sandboxed 
	-rm app_sandboxed_better
