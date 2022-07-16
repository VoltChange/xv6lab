#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64 getfreemem();
uint64 getnproc();
uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
//在kernel/sysproc.c中添加一个sys_trace()函数
uint64
sys_trace(void)
{
   // 取 a0 寄存器中的值返回给 mask
  int mask;
  if(argint(0, &mask) < 0)
    return -1;
  // 把 mask 传给现有进程的 mask
  struct proc *p =myproc();
  p->trace_mask=mask;
  return 0;
}
uint64
sys_sysinfo(void)
{
  struct sysinfo info;
  info.freemem =getfreemem();
  info.nproc=getnproc();
  uint64 addr;
  struct proc* p =myproc();
  //用argaddr读进来我们要保存的在用户态的数据sysinfo的指针地址
  if(argaddr(0, &addr) < 0)
    return -1;
  //把info的数据拷贝到指针指向的数据里
  if(copyout(p->pagetable, addr, (char *)&info, sizeof(info)) < 0)
    return -1;
  printf("abcdefg111");
  return 0;
}