#include "stdio.h"
#include "string.h"
#include "lib.h"


int empty_0args(){
    return 0;
}

int empty_2intargs(int arg0, int arg1){
    return 0;
}
int empty_4intargs(int arg0, int arg1, int arg2, int arg3){
    return 0;
}
int empty_6intargs(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5){
    return 0;
}
int empty_8intargs(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7){
    return 0;
}
// these two will spill to the stack
int empty_10intargs(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9){
    return 0;
}
int empty_12intargs(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11){
    return 0;
}

