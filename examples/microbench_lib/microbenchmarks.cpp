#include "lib.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#include <stdint.h>
#include <assert.h>
#include <fstream>
#define RLBOX_SINGLE_THREADED_INVOCATIONS
#include "rlbox_wasm2c_sandbox.hpp"
#include "rlbox.hpp"

typedef rlbox::rlbox_sandbox<rlbox::rlbox_wasm2c_sandbox> wasm2c_sbox;


#if defined(__ARM_ARCH) && __ARM_ARCH >= 6
// aarch64 cycle counter
inline uint64_t start_timer(void){
  uint32_t pmccntr;
  uint32_t pmuseren;
  uint32_t pmcntenset;
  // Read the user mode perf monitor counter access permissions.
  asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
  if (pmuseren & 1) {  // Allows reading perfmon counters for user mode code.
    asm volatile("mrc p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
    if (pmcntenset & 0x80000000ul) {  // Is it counting?
      asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
      // The counter is set up to count every 64th cycle
      return static_cast<int64_t>(pmccntr) * 64;  // Should optimize to << 6
    }
  }
}

inline uint64_t end_timer(void){
  uint32_t pmccntr;
  uint32_t pmuseren;
  uint32_t pmcntenset;
  // Read the user mode perf monitor counter access permissions.
  asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
  if (pmuseren & 1) {  // Allows reading perfmon counters for user mode code.
    asm volatile("mrc p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
    if (pmcntenset & 0x80000000ul) {  // Is it counting?
      asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
      // The counter is set up to count every 64th cycle
      return static_cast<int64_t>(pmccntr) * 64;  // Should optimize to << 6
    }
  }
}

#elif defined(__x86_64__) || defined(__amd64__)
#include <x86intrin.h>
#include <cpuid.h>
// x86 cycle counter using rdtsc
inline uint64_t start_timer(void)
{
//    unsigned a, d;
//    __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));
//    return ((uint64_t)a) | (((uint64_t)d) << 32);;
    unsigned x0,x1,x2,x3;
    __cpuid_count(0, 0, x0, x1, x2, x3);
    return __rdtsc();
}

// x86 cycle counter using rdtsc
inline uint64_t end_timer(void)
{
   unsigned int a;
   unsigned x0,x1,x2,x3;
   uint64_t result = __rdtscp(&a);
    __cpuid_count(0, 0, x0, x1, x2, x3);
    return result;
}
#endif
// macro DECLARE_BENCH(name, iterations, target_func)

uint64_t rounded_average(uint64_t total, uint64_t iterations){
    return (uint64_t)(total / iterations); 
}

uint64_t get_ctr_overhead(uint64_t iterations) {
    double ctr = 0;
    for(int idx = 0; idx < iterations; idx++){
        uint64_t start = start_timer(); 
        uint64_t end = end_timer();
        assert(end >= start);
        ctr += end - start;
    }
    uint64_t result = (uint64_t)(ctr / iterations);
    return result;
}

uint64_t bench_empty_0args(wasm2c_sbox& sandbox, uint64_t iterations, uint64_t ctr_overhead) {
    uint64_t ctr = 0;
    for(int idx = 0; idx < iterations; idx++){
        uint64_t start = start_timer(); 
        sandbox.invoke_sandbox_function(empty_0args).UNSAFE_unverified();
        uint64_t end = end_timer();
        assert(end >= start + ctr_overhead);
        ctr += end - (start + ctr_overhead);
    }
    return ctr;
}

uint64_t bench_empty_2args(wasm2c_sbox& sandbox, uint64_t iterations, uint64_t ctr_overhead) {
    uint64_t ctr = 0;
    for(int idx = 0; idx < iterations; idx++){
        uint64_t start = start_timer(); 
        sandbox.invoke_sandbox_function(empty_2intargs, 0, 1).UNSAFE_unverified();
        uint64_t end = end_timer();
        assert(end >= start + ctr_overhead);
        ctr += end - (start + ctr_overhead);
    }
    return ctr;
}

uint64_t bench_empty_4args(wasm2c_sbox& sandbox, uint64_t iterations, uint64_t ctr_overhead) {
    uint64_t ctr = 0;
    for(int idx = 0; idx < iterations; idx++){
        uint64_t start = start_timer(); 
        sandbox.invoke_sandbox_function(empty_4intargs, 0, 1, 2, 3).UNSAFE_unverified();
        uint64_t end = end_timer();
        assert(end >= start + ctr_overhead);
        ctr += end - (start + ctr_overhead);
    }
    return ctr;
}

uint64_t bench_empty_6args(wasm2c_sbox& sandbox, uint64_t iterations, uint64_t ctr_overhead) {
    uint64_t ctr = 0;
    for(int idx = 0; idx < iterations; idx++){
        uint64_t start = start_timer(); 
        sandbox.invoke_sandbox_function(empty_6intargs, 0, 1, 2, 3, 4, 5).UNSAFE_unverified();
        uint64_t end = end_timer();
        assert(end >= start + ctr_overhead);
        ctr += end - (start + ctr_overhead);
    }
    return ctr;
}

uint64_t bench_empty_8args(wasm2c_sbox& sandbox, uint64_t iterations, uint64_t ctr_overhead) {
    uint64_t ctr = 0;
    for(int idx = 0; idx < iterations; idx++){
        uint64_t start = start_timer(); 
        sandbox.invoke_sandbox_function(empty_8intargs, 0, 1, 2, 3, 4, 5, 6, 7).UNSAFE_unverified();
        uint64_t end = end_timer();
        assert(end >= start + ctr_overhead);
        ctr += end - (start + ctr_overhead);
    }
    return ctr;
}

uint64_t bench_empty_10args(wasm2c_sbox& sandbox, uint64_t iterations, uint64_t ctr_overhead) {
    uint64_t ctr = 0;
    for(int idx = 0; idx < iterations; idx++){
        uint64_t start = start_timer(); 
        sandbox.invoke_sandbox_function(empty_10intargs, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9).UNSAFE_unverified();
        uint64_t end = end_timer();
        assert(end >= start + ctr_overhead);
        ctr += end - (start + ctr_overhead);
    }
    return ctr;
}

uint64_t bench_empty_12args(wasm2c_sbox& sandbox, uint64_t iterations, uint64_t ctr_overhead) {
    uint64_t ctr = 0;
    for(int idx = 0; idx < iterations; idx++){
        uint64_t start = start_timer(); 
        sandbox.invoke_sandbox_function(empty_12intargs, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11).UNSAFE_unverified();
        uint64_t end = end_timer();
        assert(end >= start + ctr_overhead);
        ctr += end - (start + ctr_overhead);
    }
    return ctr;
}

uint64_t bench_data_transfer(wasm2c_sbox& sandbox, uint64_t iterations, uint64_t sz, uint64_t ctr_overhead) {
    uint64_t ctr = 0;
    // get host-space buf to copy to sandbox
    char* buf = (char*)malloc(sz); 
    for(int idx = 0; idx < iterations; idx++){
        uint64_t start = start_timer(); 
        auto tainted_buf = sandbox.malloc_in_sandbox<char>(sz);
        memcpy(tainted_buf.unverified_safe_pointer_because(sz, "Writing to sandbox"), buf, sz);
        uint64_t end = end_timer();
        sandbox.free_in_sandbox(tainted_buf);
        assert(end >= start + ctr_overhead);
        ctr += end - (start + ctr_overhead);
    }
    return ctr;
}

auto copy_str_to_sandbox(wasm2c_sbox& sandbox, char* str){
    size_t str_size = strlen(str) + 1;
    auto str_tainted = sandbox.malloc_in_sandbox<char>(str_size);
    // copy to sandbox
    std::strncpy(str_tainted.unverified_safe_pointer_because(str_size, "writing to region"), str, str_size);
    return str_tainted;
}

void print_result_row(std::ofstream& outstream, std::string benchname, uint64_t total, uint64_t iterations){
        std::cout << benchname << " | " << total << " | " << rounded_average(total, iterations) << std::endl;
        outstream << benchname << " | " << total << " | " << rounded_average(total, iterations) << std::endl;
}

int main(int argc, char const *argv[])
{
    wasm2c_sbox sandbox;
    sandbox.create_sandbox("./my_lib.so");

    // check for input from command line
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output path>" << std::endl;
        return 1;
    }

 

	

    uint64_t iterations = 100000;
    auto ctr_overhead = get_ctr_overhead(iterations);

    // Benchmark control-flow transfers
    auto total_empty_0args = bench_empty_0args(sandbox, iterations, ctr_overhead);
    auto total_empty_2args = bench_empty_2args(sandbox, iterations, ctr_overhead);
    auto total_empty_4args = bench_empty_4args(sandbox, iterations, ctr_overhead);
    auto total_empty_6args = bench_empty_6args(sandbox, iterations, ctr_overhead);
    auto total_empty_8args = bench_empty_8args(sandbox, iterations, ctr_overhead);
    auto total_empty_10args = bench_empty_10args(sandbox, iterations, ctr_overhead);
    auto total_empty_12args = bench_empty_12args(sandbox, iterations, ctr_overhead);


    // Benchmark dataflow transfers
    auto total_transfer_8bytes = bench_data_transfer(sandbox, iterations, 8, ctr_overhead);
    auto total_transfer_16bytes = bench_data_transfer(sandbox, iterations, 16, ctr_overhead);
    auto total_transfer_32bytes = bench_data_transfer(sandbox, iterations, 32, ctr_overhead);
    auto total_transfer_64bytes = bench_data_transfer(sandbox, iterations, 64, ctr_overhead);
    auto total_transfer_256bytes = bench_data_transfer(sandbox, iterations, 256, ctr_overhead);
    auto total_transfer_1024bytes = bench_data_transfer(sandbox, iterations, 1024, ctr_overhead);
    auto total_transfer_4K = bench_data_transfer(sandbox, iterations, 4 * 1024, ctr_overhead);
    auto total_transfer_16K = bench_data_transfer(sandbox, iterations, 16 * 1024, ctr_overhead);
    auto total_transfer_64K = bench_data_transfer(sandbox, iterations, 64 * 1024, ctr_overhead);









    // char* copy_str = (char*)argv[1];
    // auto tainted_copy_str = copy_str_to_sandbox(sandbox, copy_str);
   
    // char* result_str = (char*)argv[2];
    // auto tainted_result_str = copy_str_to_sandbox(sandbox, result_str);

    // sandbox.invoke_sandbox_function(print_version);
    // long long hash = sandbox.invoke_sandbox_function(get_hash, tainted_copy_str,
    // on_completion_cb, tainted_result_str).UNSAFE_unverified();
    
    // printf("Hash = %llx\n", hash);

    // sandbox.free_in_sandbox(tainted_copy_str);
    // sandbox.free_in_sandbox(tainted_result_str);

    sandbox.destroy_sandbox();

    std::ofstream outfile;
    outfile.open(argv[1]);

    // Output Results
    std::cout << "Bench name | total | average (Counter overhead = " << ctr_overhead << ", iterations = " << iterations << ")" << std::endl;
    outfile << "Bench name | total | average (Counter overhead = " << ctr_overhead << ", iterations = " << iterations << ")" << std::endl;
    std::cout << "------------- Calls with n arguments --------------" << std::endl;
    outfile << "------------- Calls with n arguments --------------" << std::endl;
    print_result_row(outfile, "0", total_empty_0args, iterations);
    print_result_row(outfile, "2", total_empty_2args, iterations);
    print_result_row(outfile, "4", total_empty_4args, iterations);
    print_result_row(outfile, "6", total_empty_6args, iterations);
    print_result_row(outfile, "8", total_empty_8args, iterations);
    print_result_row(outfile, "10", total_empty_10args, iterations);
    print_result_row(outfile, "12", total_empty_12args, iterations);
    std::cout << "-------------- Transferring N bytes ----------------" << std::endl;
    outfile << "-------------- Transferring N bytes ----------------" << std::endl;
    print_result_row(outfile, "8B", total_transfer_8bytes, iterations);
    print_result_row(outfile, "16B", total_transfer_16bytes, iterations);
    print_result_row(outfile, "32B", total_transfer_32bytes, iterations);
    print_result_row(outfile, "64B", total_transfer_64bytes, iterations);
    print_result_row(outfile, "256B", total_transfer_256bytes, iterations);
    print_result_row(outfile, "1024B", total_transfer_1024bytes, iterations);
    print_result_row(outfile, "4K", total_transfer_4K, iterations);
    print_result_row(outfile, "16K", total_transfer_16K, iterations);
    print_result_row(outfile, "64K", total_transfer_64K, iterations);
    std::cout << "-------------------------------------" << std::endl;
    outfile << "-------------------------------------" << std::endl;

    outfile.close();
    return 0;
}