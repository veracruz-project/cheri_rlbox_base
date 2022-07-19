.PHONY: bootstrap clean

RLBOX_ROOT:=./rlbox_wasm2c_sandbox
WASI_SDK_ROOT:=$(RLBOX_ROOT)/build/_deps/wasiclang-src/
WASM2C_ROOT=./$(RLBOX_ROOT)/build/_deps/mod_wasm2c-src

WASI_SDK_FLAGS=-Wl,--export-all -Wl,--growable-table -Wl,--no-entry

CC_HYBRID=clang -march=morello
CXX_HYBRID=clang++ -march=morello
CC_PURE=cc
CXX_PURE=c++

# Bootstrap sandboxing and compiler toolchain for compiling on non-CHERI system
bootstrap_host:
	git submodule update --init --recursive
	cd rlbox_wasm2c_sandbox && cmake -S . -B ./build && cmake --build ./build --parallel && cmake --build ./build --target all

# Can't build all of rlbox on Cheri guest, but we still need headers which rlbox pulls in.
# So we just fake it
bootstrap_guest:
	git submodule update --init --recursive
	mkdir -p $(RLBOX_ROOT)/build/_deps
	cd $(RLBOX_ROOT)/build/_deps && git clone https://github.com/PLSysSec/wasm2c_sandbox_compiler mod_wasm2c-src
	cd $(RLBOX_ROOT)/build/_deps && git clone https://github.com/PLSysSec/rlbox_api_cpp17.git rlbox-src