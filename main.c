#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "spinlock.h" //project 2.4

extern void init_locks(void);   // extra credit-1
extern void pilocks_init(void); // extra credit-1
static void startothers(void);
static void mpmain(void) __attribute__((noreturn));
extern pde_t *kpgdir;
extern char end[];                 // first address after kernel loaded from ELF file
extern struct spinlock trace_lock; // project 2.4
void trace_lock_init(void);        // project 2.4


int main(void)
{
  kinit1(end, P2V(4 * 1024 * 1024));          // phys page allocator
  kvmalloc();                                 // kernel page table
  mpinit();                                   // detect other processors
  lapicinit();                                // interrupt controller
  seginit();                                  // segment descriptors
  picinit();                                  // disable pic
  ioapicinit();                               // another interrupt controller
  consoleinit();                              // console hardware
  uartinit();                                 // serial port
  pinit();                                    // process table
  tvinit();                                   // trap vectors
  binit();                                    // buffer cache
  fileinit();                                 // file table
  ideinit();                                  // disk
  startothers();                              // start other processors
  kinit2(P2V(4 * 1024 * 1024), P2V(PHYSTOP)); // must come after startothers()
  userinit();                                 // first user process
  mpmain();                                   // finish this processor's setup
  pilocks_init();                             // extra credit-1
  // initlog();                                  // extra credit-1
  init_locks();      // extra credit-1
  trace_lock_init(); // project 2.4
}

// Other CPUs jump here from entryother.S.
static void
mpenter(void)
{
  switchkvm();
  seginit();
  lapicinit();
  mpmain();
}

// Common CPU setup code.
static void
mpmain(void)
{
  cprintf("cpu%d: starting %d\n", cpuid(), cpuid());
  idtinit();                    // load idt register
  xchg(&(mycpu()->started), 1); // tell startothers() we're up
  scheduler();                  // start running processes
}

pde_t entrypgdir[]; // For entry.S

// Start the non-boot (AP) processors.
static void
startothers(void)
{
  extern uchar _binary_entryother_start[], _binary_entryother_size[];
  uchar *code;
  struct cpu *c;
  char *stack;

  // Write entry code to unused memory at 0x7000.
  // The linker has placed the image of entryother.S in
  // _binary_entryother_start.
  code = P2V(0x7000);
  memmove(code, _binary_entryother_start, (uint)_binary_entryother_size);

  for (c = cpus; c < cpus + ncpu; c++)
  {
    if (c == mycpu()) // We've started already.
      continue;

    // Tell entryother.S what stack to use, where to enter, and what
    // pgdir to use. We cannot use kpgdir yet, because the AP processor
    // is running in low  memory, so we use entrypgdir for the APs too.
    stack = kalloc();
    *(void **)(code - 4) = stack + KSTACKSIZE;
    *(void (**)(void))(code - 8) = mpenter;
    *(int **)(code - 12) = (void *)V2P(entrypgdir);

    lapicstartap(c->apicid, V2P(code));

    // wait for cpu to finish mpmain()
    while (c->started == 0)
      ;
  }
}



__attribute__((__aligned__(PGSIZE)))
pde_t entrypgdir[NPDENTRIES] = {
    // Map VA's [0, 4MB) to PA's [0, 4MB)
    [0] = (0) | PTE_P | PTE_W | PTE_PS,
    // Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
    [KERNBASE >>
        PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS,
};

