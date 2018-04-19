#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
#include "defs.h"
#include "thread.h"


void thread_cond_init(thread_cond_t *cond) {
    cond->threadId = -1;
    cond->next = 0;
    return;
}

void thread_cond_signal(thread_cond_t *cond) {
    thread_cond_t *mycond = cond;
    while (mycond->threadId == -1)
        mycond = mycond->next;
    if (mycond != 0) {
        int threadId = mycond->threadId;
        thread_cond_t *garbage = mycond;
        cond->next = cond->next->next;
        free(garbage);
        wakeup(threadId);
    }
    return;
}

void thread_cond_wait(thread_cond_t *cond, thread_mutex_t *mutex) {
    thread_cond_t *mycond = cond;
    thread_mutex_unlock(mutex);

    //iterate the linked-list pointer to the end;
    while (mycond->next != 0)
        mycond = mycond->next;

    mycond->next = (thread_cond_t *) malloc(sizeof(thread_cond_t));
    mycond->next->threadId = getpid();
    mycond->next->next = 0;
    thread_mutex_lock(mutex);
    thread_mutex_unlock(mutex);
    sleep(mutex);
    thread_mutex_lock(mutex);
    return;
}

// Thread 1 (sender)
void *
send(void *arg) {
    while (1) {
        thread_mutex_lock(&q->m);
        while (q->ptr != 0)
            ;
        q->ptr = p;
        thread_cond_signal(&q->cv);
        thread_mutex_unlock(&q->m);
    }
}

// Thread 2 (receiver)

void *
recv(void *arg) {
    void *p;

    thread_mutex_lock(&q->m);

    while ((p = q->ptr) == 0)
        thread_cond_wait(&q->cv, &q->m);
    q->ptr = 0;

    thread_mutex_unlock(&q->m);
    return p;
}


volatile unsigned int delay(unsigned int d) {
    unsigned int i;
    for (i = 0; i < d; i++) {
        __asm volatile( "nop":: : );
    }

    return i;
}

void thread_spin_init(struct thread_spinlock *lk) {
    lk->locked = 0;
}

void thread_spin_lock(struct thread_spinlock *lk) {

    // The xchg is atomic.
    while (xchg(&lk->locked, 1) != 0);

    __sync_synchronize();

}

void thread_spin_unlock(struct thread_spinlock *lk) {
    __sync_synchronize();
    asm volatile("movl $0, %0" : "+m" (lk->locked) : );

}

void thread_mutex_lock(struct thread_mutex *m) {
    while (xchg(&m->locked, 1) != 0) {
        sleep(1);
    }
    __sync_synchronize();
}

void thread_mutex_unlock(struct thread_mutex *m) {
    __sync_synchronize();
    asm volatile("movl $0, %0" : "+m" (m->locked) : );
}

void thread_mutex_init(struct thread_mutex *m) {
    m->locked = 0;
}



void do_work(void *arg) {
    int i;
    int old;

    struct balance *b = (struct balance *) arg;
    printf(1, "Starting do_work: s:%s\n", b->name);


    for (i = 0; i < b->amount; i++) {
//        thread_spin_lock(&lk);
        thread_mutex_lock(&m);
        old = total_balance;
        delay(100000);
        total_balance = old + 1;
        thread_mutex_unlock(&m);
//        thread_spin_unlock(&lk);
    }

    printf(1, "Done s:%x\n", b->name);

    thread_exit();
}


int main(int argc, char *argv[]) {
//    thread_spin_init(&lk);
//    thread_mutex_init(&m);

    // Initialize
    thread_cond_init(&q->cv);
    thread_mutex_init(&q->m);

    struct balance b1 = {"b1", 3200};
    struct balance b2 = {"b2", 2800};

    void *s1, *s2;
    int t1, t2, r1, r2;

    s1 = malloc(4096);
    s2 = malloc(4096);

    t1 = thread_create(send, (void *) &b1, s1);
    t2 = thread_create(recv, (void *) &b2, s2);

    r1 = thread_join();
    r2 = thread_join();

    printf(1, "Threads finished: (%d):%d, (%d):%d\n",
           t1, r1, t2, r2);

    exit();
}

