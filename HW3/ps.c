//
// Created by Jingcheng Lu on 2018/3/2.
//

//
// Created by Jingcheng Lu on 2018/3/2.
//

#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"
#include "uproc.h"

#define PGSIZE 4096


int main(int argc, char *argv[])
{
    int MAX = 32767;
    struct uproc *u = (struct uproc *)malloc(sizeof(struct uproc *));

    int i=0;
    while (i<MAX){
        if(getprocinfo(i,u)!=-1){
            printf(1,"pid:%d \n",u->pid);
            printf(1,"size:%d \n",u->sz);
        }
        i++;
    }
    return 0;
}