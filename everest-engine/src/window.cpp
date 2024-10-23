#include <stdio.h>
#include "core/log.h"

int test(void){
    Everest::Debug::Log("hello log");
    printf("Hello window\n");
    return 0;
}
