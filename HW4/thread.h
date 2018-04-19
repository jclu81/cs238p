typedef struct thread_spinlock {
    uint locked;       // Is the lock held?
} thread_spinlock_t;

typedef struct thread_mutex {
    uint locked;       // Is the lock held?
} thread_mutex_t;

typedef struct thread_cond {
    int threadId;
    struct thread_cond *next;
} thread_cond_t;

typedef struct q {
    thread_cond_t cv;
    thread_mutex_t m;
    void *ptr;
} q_t;

struct balance {
    char name[32];
    int amount;
};

int count = 0;
volatile int total_balance = 0;

thread_cond_t tct = {
        threadId:-1,
        thread_cond = 0
};

thread_spinlock_t lk = {
        locked: 0
};

thread_mutex_t m = {
        locked: 0
};

q_t *q = {
        cv:tct,
        m:m
};

