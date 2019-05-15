#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  uint32_t syscall_number = *(uint32_t*)(f->esp);

  // debuggin
  printf("syscall: %d\n", syscall_handler);
  hex_dump(f->esp, f->esp, 100, 1);

  // system call
  switch (syscall_number) {
  case SYS_HALT: // 0
    printf("SYS_HALT!\n");
    break;
  case SYS_EXIT: // 1
    printf("SYS_EXIT!\n");
    exit ((int)(f->esp + 4));
    break;
  case SYS_WRITE: // 9
    printf("SYS_WRITE!!\n");
    write (
      (int)(f->esp + 4),
      (void *)(f->esp + 8),
      (unsigned)(f->esp + 12)
    );
    break;
  }
}


void halt(void)
{
}

void exit(int status)
{
  printf("%s: exit(%d)\n", thread_name(), status);
  thread_exit();
}

int write (int fd, const void *buffer, unsigned size)
{
  if (fd == 1) { // stdout
    putbuf(buffer, size);
    return size;
  }
  return -1;
}