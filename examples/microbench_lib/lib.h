#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Callback to indicate the task is completed
typedef void(OnCompletion)(char*);

void print_version();

long long get_hash(char* in, OnCompletion* on_completion, char* result_msg);

long long get_hash_alt(char* in, OnCompletion* on_completion, char* result_msg);


// Aarch64 has 8 arg values before it spills to stack

int empty_0args();

int empty_2intargs(int arg0, int arg1);
int empty_4intargs(int arg0, int arg1, int arg2, int arg3);
int empty_6intargs(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5);
int empty_8intargs(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7);
// these two will spill to the stack
int empty_10intargs(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9);
int empty_12intargs(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);


#ifdef __cplusplus
}
#endif
