#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SiSH/SiSH.h"

#define DEBUG_MODE
// #undef DEBUG_MODE

Env env;

int main() {

    set_Env(&env);

    int sys_fault_count =  -1;

    int ret = sish_loop(&env);
    while (ret == EXIT_FAILURE) {
        ret = sish_loop(&env);
        sys_fault_count--;
        if (sys_fault_count < 0) {
            break;
        }
    }
}
