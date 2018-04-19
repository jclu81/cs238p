//
// Created by Jingcheng Lu on 2018/3/2.
//

#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

#define PGSIZE 4096

void print(void* addr, void *buffer, int size)
{
    int i = 0;
    while(i<size){
        printf(1,"0x%x:\t", addr + i * 4);
        int j;
        for (j = 0; j < 4; j++)
        {
            printf(1,"0x%08x\t", (uint)(*((uint*)(buffer + i + j))));
        }
        printf(1, "\n");
        i += 4;
    }
}


int main(int argc, char *argv[])
{
    void *addr;
    addr = 0;
    void *buffer = malloc(PGSIZE * sizeof(void));
    int pid = fork();
    sbrk(0);
    if (pid == 0) {
        /* child spins and yields */
        while (1) {
            sleep(5);
        };
    }
    /* parent dumps memory of the child */
    while (dump(pid, addr, buffer, PGSIZE)!=-1){
        print(addr,buffer,PGSIZE);
        addr+=PGSIZE;
    }
//    int tmp=0;
//    while (tmp!=-2){
//        int tmp= dump(pid, addr, buffer, PGSIZE);
//        if(tmp > -1){
//            print(addr,buffer,PGSIZE);
//        }
//        addr+=PGSIZE;
//    }
    kill(pid);
    return 0;
}

