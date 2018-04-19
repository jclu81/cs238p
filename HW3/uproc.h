//
// Created by Jingcheng Lu on 2018/3/2.
//
enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct uproc {
    void *chan;                  // If non-zero, sleeping on chan
    int killed;                  // If non-zero, have been killed
    enum procstate state;        // Process state
    uint sz;                     // Size of process memory (bytes)
    int parentid;                // Parent process
    int pid;                     // Process ID
    char name[16];               // Process name (debugging)
};