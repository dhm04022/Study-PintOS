#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
void check_user_vaddr(const void *vaddr);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  uint32_t syscall_number = *(uint32_t*)(f->esp);

  // debuggin
  // printf("syscall: %d\n", syscall_number);
  // hex_dump(f->esp, f->esp, 100, 1);

  // system call
  switch (syscall_number) {
  case SYS_HALT: // 0
    halt();
    break;
  case SYS_EXIT: // 1
    check_user_vaddr(f->esp + 8);
    exit (*(uint32_t*)(f->esp + 8));
    break;
  case SYS_EXEC: // 2
    check_user_vaddr(f->esp + 8); 
    f->eax = exec((const char *)*(uint32_t *)(f->esp + 8));
    break;
  case SYS_WAIT: // 3
    check_user_vaddr(f->esp + 8); 
    f-> eax = wait((pid_t)*(uint32_t *)(f->esp + 8));
    break;
  case SYS_CREATE: // 4
    
    break;
  case SYS_REMOVE: // 5
    
    break;
  case SYS_OPEN: // 6
    
    break;
  case SYS_FILESIZE: // 7
    
    break;
  case SYS_READ: // 8
    check_user_vaddr(f->esp + 24); 
    check_user_vaddr(f->esp + 28); 
    check_user_vaddr(f->esp + 32); 
    f-> eax = read (
      (int)*(uint32_t *)(f->esp+24), 
      (void *)*(uint32_t *)(f->esp + 28), 
      (unsigned)*((uint32_t *)(f->esp + 32))
    );
    break;              
  case SYS_WRITE: // 9
    check_user_vaddr(f->esp + 24); 
    check_user_vaddr(f->esp + 28); 
    check_user_vaddr(f->esp + 32); 
    f->eax = write(
      (int)*(uint32_t *)(f->esp+24), 
      (void *)*(uint32_t *)(f->esp + 28), 
      (unsigned)*((uint32_t *)(f->esp + 32))
    );
    break;
  case SYS_SEEK: // 10
    
    break;        
  case SYS_TELL: // 11
    
    break;           
  case SYS_CLOSE: // 12
    
    break;          
  }
}


void halt(void)
{
  shutdown_power_off();
}

void exit(int status)
{
  printf("%s: exit(%d)\n", thread_name(), status);
  thread_current() -> exit_status = status;
  thread_exit();
}

pid_t exec (const char *cmd_line) 
{ 
  return process_execute(cmd_line); 
} 

int wait (pid_t pid) 
{ 
  return process_wait(pid); 
} 

int read (int fd, void* buffer, unsigned size) 
{ 
  int i; 
  if (fd == 0) { 
      for (i = 0; i < size; i ++) 
      { 
        if (((char *)buffer)[i] == '\0') 
          { break; } 
      } 
  } 
  return i; 
} 

int write (int fd, const void *buffer, unsigned size)
{
  if (fd == 1) { // stdout
    putbuf(buffer, size);
    return size;
  }
  return -1;
}




void check_user_vaddr(const void *vaddr) 
{ 
  if (!is_user_vaddr(vaddr)) 
  { 
    exit(-1); 
  } 
}