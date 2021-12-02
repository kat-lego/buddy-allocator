#include "allocator.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char **argv)
{

    int *data,i;
    double *ddata;
    bool done;
    printf("Starting tests...\n");

    printf("Basic malloc....");
    ddata = (double*) custom_malloc(sizeof(double));
    *ddata = 12345.6789;
    printf("%s\n",(*ddata)==12345.6789?"'done'":"failed!");
    printf("Array malloc....");  
    data = (int*) custom_malloc(1028*sizeof(int));
    for(i = 0; i < 1028; i++) data[i] = i;
    done = true;
    for(i = 0; i < 1028; i++) done &= (data[i]==i);
    printf("%s\n",done?"'done'":"failed!");
    
    printf("Basic free......");  
    custom_free(ddata);
    printf("'done'\n");

    printf("Array free......");  
    custom_free(data);
    printf("'done'\n");

    printf("Basic realloc...");
    ddata = (double*) custom_malloc(sizeof(double));
    *ddata = 12345.6789;
    double* old = ddata;
    ddata = (double*) custom_realloc(ddata,1000*sizeof(double));
    done = ((old<ddata)&&((*ddata)==12345.6789));
    custom_free(ddata);
    printf("%s\n",done?"'done'":"failed!");
    
    return 0;
}
