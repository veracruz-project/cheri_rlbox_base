#include "lib.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#define RLBOX_SINGLE_THREADED_INVOCATIONS
#include "rlbox_wasm2c_sandbox.hpp"
#include "rlbox.hpp"



//Callback on completion of library function
// void on_completion(char* result) {
void on_completion(rlbox::rlbox_sandbox<rlbox::rlbox_wasm2c_sandbox>& _,
            rlbox::tainted<char*, rlbox::rlbox_wasm2c_sandbox> tainted_str){
    char result_str[100];
    
    auto result =
      tainted_str.copy_and_verify_string(
          [](std::unique_ptr<char[]> val) {
         return std::strlen(val.get()) < 100 ? std::move(val) : nullptr;
      });
    if(result == nullptr){
        return;
    }
    strcpy(result_str, result.get());
    std::cout << "Done! Result = " << result_str << "\n";
}

auto copy_str_to_sandbox(rlbox::rlbox_sandbox<rlbox::rlbox_wasm2c_sandbox> &sandbox, char* str){
    size_t str_size = strlen(str) + 1;
    auto str_tainted = sandbox.malloc_in_sandbox<char>(str_size);
    // copy to sandbox
    std::strncpy(str_tainted.unverified_safe_pointer_because(str_size, "writing to region"), str, str_size);
    return str_tainted;
}

int main(int argc, char const *argv[])
{
    rlbox::rlbox_sandbox<rlbox::rlbox_wasm2c_sandbox> sandbox;
    sandbox.create_sandbox("./my_lib.so");

    //check for input from command line
    if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " STRING_TO_HASH RESULT_MESSAGE" << std::endl;        
    return 1;
    }

    auto on_completion_cb = sandbox.register_callback(on_completion);

    char* copy_str = (char*)argv[1];
    auto tainted_copy_str = copy_str_to_sandbox(sandbox, copy_str);
   
    char* result_str = (char*)argv[2];
    auto tainted_result_str = copy_str_to_sandbox(sandbox, result_str);

    // char* output_stream = new char[100];
    // auto tainted_output_stream = copy_str_to_sandbox(sandbox, output_stream);

    sandbox.invoke_sandbox_function(print_version);   
    
    long long hash = sandbox.invoke_sandbox_function(get_hash, tainted_copy_str,
    on_completion_cb, tainted_result_str).copy_and_verify([](unsigned ret){
        return ret;
        });
    
    printf("Hash = %llx\n", hash);

    sandbox.free_in_sandbox(tainted_copy_str);
    sandbox.free_in_sandbox(tainted_result_str);

    sandbox.destroy_sandbox();

    return 0;
}
