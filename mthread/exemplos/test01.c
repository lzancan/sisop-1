#include	"../include/pithread.h"
#include <stdio.h>

void* func01()
{
    printf(" 1 ");
    piyield();
    printf(" 3 ");
    return NULL;
}

void* func02()
{
    printf(" 2 ");
    piyield();
    printf(" 4 ");
    return NULL;
}

int main(){
    int id1, id2;
    int i;
    
    id1 = picreate(90, func01, (void *)&i);
    id2 = picreate(80, func02, (void *)&i);
    
    printf("\nMain rodando apos criar as threads\n");
    piwait(id2);
    
    printf("\nthreads IDs = %d %d\n",id1,id2);
    
    return 1;
}