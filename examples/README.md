# morello-examples
This repo contains all the libraries that I am testing morellobox with.

## To build
`make bootstrap_host` to download and build rlbox on a non-Cheri host  
`make bootstrap_guest` to download rlbox on a Cheri guest  

## Examples

### SimpleLib
I currently have 2 examples, all in the `simple_lib` library.  
`app_sandboxed` implements a simple hashing library.  
`app_sandboxed_better` implements the same library, but with more rigorous sandboxing. In particular, it correctly sandboxes a callback function to prevent confused deputy attacks.  

Both of these libraries can be compiled for CHERI, non-CHERI, and hybrid-CHERI architectures.  

If you are on a non-CHERI system, run `make app_sandboxed` and `make_sandboxed_better` from the `simple_lib` directory to build these two applications.  
To build these applications in hybrid-CHERI mode (on a CheriBSD system), compile them using `make app_sandboxed_hybrid` and `make app_sandboxed_better_hybrid`.    
To build these applications in purecap-CHERI mode (on a CheriBSD system), compile them using `make app_sandboxed_cheri` and `make app_sandboxed_better_cheri`.  
For the Cheri targets, you need to compile the wasm2c c file outside of the guest.

Currently hybrid examples run correctly, purecap examples compile but do not run.

