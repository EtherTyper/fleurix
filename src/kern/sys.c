#include <param.h>
#include <x86.h>
#include <proto.h>
#include <proc.h>

#include <unistd.h>

int errno = 0;

static uint sys_routines[NSYSC] = {
    [NR_setup] = &sys_setup,
    [NR_putn]  = &sys_putn,
    [NR_fork]  = &sys_fork,
    [NR_nice]  = &sys_nice,
    [NR_debug] = &sys_debug,
    0,
}; 

/***********************************************************/

void sys_debug(struct trap *tf){
}

void sys_fork(struct trap *tf){
    int ret = copy_proc(tf);
    if (ret<0){
        panic("error fork()\n");
    }
    tf->eax = ret;
}

void sys_nice(struct trap *tf){
    int n = tf->ebx & 0xff;
    if (n > 20) n = 20;
    if (n<-19) n=-19;
}

void sys_putn(struct trap *tf){
    printf("%d", tf->ebx);
}

/***********************************************************/

void syserr(uint err){
    current->p_error = err;
}

void do_syscall(struct trap *r){
    //printf("do_syscall();\n");
    void (*fn)(struct trap *r);
    fn = sys_routines[r->eax];
    
    if(fn){
        (*fn)(r);
    }
}

