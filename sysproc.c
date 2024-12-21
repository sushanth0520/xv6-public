#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "syscall.h"
#include "trace.h" //project 2.4

extern int lock_resource(int id);    // extra credit
extern int release_resource(int id); // extra credit
// project 2.4
extern struct trace_event trace_buffer[];
extern int trace_buf_index;
extern int trace_buf_count;
extern struct spinlock trace_lock;
// project 2.4

int strcmp(const char *, const char *); // project 3.1

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_nice(void)
{
  int pid, value;

  if (argint(0, &pid) < 0 || argint(1, &value) < 0)
    return -1;

  // Validate the nice value (must be between 1 and 5)
  if (value < 1 || value > 5)
    return -1;

  return setnice(pid, value);
}


int sys_set_priority(void)
{
  int priority;
  if (argint(0, &priority) < 0)
    return -1;
  if (priority < 1)
    priority = 1; 
  myproc()->priority = priority;
  return 0;
}

int sys_lock(void)
{
  int id;
  struct lock_t *lk;
  if (argint(0, &id) < 0)
    return -1;
  lk = get_lock(id);
  if (!lk)
    return -1;
  acquire_lock(lk);
  return 0;
}

int sys_release(void)
{
  int id;
  struct lock_t *lk;
  if (argint(0, &id) < 0)
    return -1;
  lk = get_lock(id);
  if (!lk)
    return -1;
  release_lock(lk);
  return 0;
}

//  // extra credit-1
// project
int sys_trace(void)
{
  int enable;
  struct proc *curproc = myproc();
  if (argint(0, &enable) < 0)
    return -1;
  curproc->tracing = enable;
  return 0;
}


// project 2.4
int sys_get_trace(void)
{
  struct trace_event *user_events;
  int max_events;

  if (argint(1, &max_events) < 0)
    return -1;
  if (argptr(0, (void *)&user_events, sizeof(struct trace_event) * max_events) < 0)
    return -1;

  int events_to_copy;
  int i;

  acquire(&trace_lock); 

  events_to_copy = trace_buf_count < max_events ? trace_buf_count : max_events;

  int index = (trace_buf_index - trace_buf_count + TRACE_BUF_SIZE) % TRACE_BUF_SIZE;
  for (i = 0; i < events_to_copy; i++)
  {
    if (copyout(myproc()->pgdir, (uint)(user_events + i), (void *)&trace_buffer[(index + i) % TRACE_BUF_SIZE], sizeof(struct trace_event)) < 0)
    {
      release(&trace_lock);
      return -1;
    }
  }

  release(&trace_lock); 

  return events_to_copy; 
}



int sys_tracefilter(void)
{
  char *sc;
  if (argstr(0, &sc) < 0)
    return -1;

  struct proc *curproc = myproc();
  if (sc[0] == '\0')
  {
    curproc->filtering = 0;
    curproc->filter_syscall[0] = '\0';
  }
  else
  {
    curproc->filtering = 1;
    safestrcpy(curproc->filter_syscall, sc, sizeof(curproc->filter_syscall));
  }
  return 0;
}
// project 3.1

// project 3.2
int sys_traceonlysuccess(void)
{
  int enable;
  if (argint(0, &enable) < 0)
    return -1;

  struct proc *curproc = myproc();
  curproc->success_only = enable;
  return 0;
}
// project 3.2

// project 3.3
int sys_traceonlyfail(void)
{
  int enable;
  if (argint(0, &enable) < 0)
    return -1;

  struct proc *curproc = myproc();
  curproc->fail_only = enable;
  
  if (enable == 1)
    curproc->success_only = 0;
  return 0;
}
