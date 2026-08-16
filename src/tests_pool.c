#include "pool.h"
#include <stdio.h>
#include <time.h>

int main(){
    clock_t start_t, end_t;
    start_t = clock();
    {   
        SorcererPool *pool = sorcerer_create(1024*8);
        if(pool==NULL){
            return 1;
        }
        for(int i=0;i<500000;i++){
            int *p =  sorcerer_alloc(pool,sizeof(int));
            *p = i;
            //printf("%d\t|%p\n",(int)*p,(void*)p);
        }
        end_t = clock();
        double tm = ((double)((end_t-start_t))/(double)CLOCKS_PER_SEC);
        printf("耗时 %.4f s\n",tm);
        sorcerer_destroy(pool);
    }
    return 0;
}