#include <stdio.h>

void black(){
    printf("\033[0;30m");
    return ;
}

void red(){
    printf("\033[1;31m");
    return ;
}

void green(){
    printf("\033[1;32m");
    return ;
}

void yellow(){
    printf("\033[1;33m");
    return ;
}

void blue(){
    printf("\033[1;34m");
    return ;
}

void purple(){
    printf("\033[1;35m");
    return ;
}

void cyan(){
    printf("\033[1;36m");
    return ;
}

void white(){
    printf("\033[1;37m");
    return ;
}

void reset(){
    printf("\033[0m");
    return ;
}
