#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
void check_user_vaddr(const void *vaddr);

struct file 
{ 
  struct inode *inode; /* File's inode. */ 
  off_t pos; /* Current position. */ 
  bool deny_write; /* Has file_deny_write() been called? */ 
};

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
    check_user_vaddr(f->esp + 4); 
    check_user_vaddr(f->esp + 8); 
    f->eax = create(
      (const char *)*(uint32_t *)(f->esp + 4), 
      (unsigned)*(uint32_t *)(f->esp + 8));
    break;
  case SYS_REMOVE: // 5
    check_user_vaddr(f->esp + 4); 
    f->eax = remove((const char*)*(uint32_t *)(f->esp + 4));
    break;
  case SYS_OPEN: // 6
    check_user_vaddr(f->esp + 4); 
    f->eax = open((const char*)*(uint32_t *)(f->esp + 4));
    break;
  case SYS_FILESIZE: // 7
    check_user_vaddr(f->esp + 4); 
    f->eax = filesize((int)*(uint32_t *)(f->esp + 4));
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
    check_user_vaddr(f->esp + 4); 
    check_user_vaddr(f->esp + 8); 
    seek(
      (int)*(uint32_t *)(f->esp + 4), 
      (unsigned)*(uint32_t *)(f->esp + 8)
    );
    break;        
  case SYS_TELL: // 11
    check_user_vaddr(f->esp + 4); 
    f->eax = tell((int)*(uint32_t *)(f->esp + 4));
    break;           
  case SYS_CLOSE: // 12
    check_user_vaddr(f->esp + 4); 
    close((int)*(uint32_t *)(f->esp + 4));
    break;          
  }
}


void halt(void)
{
  shutdown_power_off();
}

void exit(int status)
{
  int i;
  printf("%s: exit(%d)\n", thread_name(), status);
  thread_current() -> exit_status = status;
  for (i = 3; i < 128; i++) 
  { 
    if (thread_current()->fd[i] != NULL) 
    { 
      close(i); 
    } 
  }
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

bool create (const char *file, unsigned initial_size) 
{ 
  if (file == NULL) 
  { 
    exit(-1); 
  } 
  check_user_vaddr(file);
  return filesys_create(file, initial_size); 
} 

bool remove (const char *file) 
{ 
  if (file == NULL) 
  { 
    exit(-1); 
  } 
  check_user_vaddr(file);
  return filesys_remove(file); 
} 

int open (const char *file) 
{ 
  int i; 
  struct file* fp;

  if (file == NULL) 
  { 
    exit(-1); 
  } 
  check_user_vaddr(file);
  fp = filesys_open(file); 
  if (fp == NULL) 
  { 
    return -1; 
  } 
  else 
  { 
    for (i = 3; i < 128; i++) 
    { 
      if (thread_current()->fd[i] == NULL) 
      { 
        if (strcmp(thread_current()->name, file) == 0) 
        { 
          file_deny_write(fp); 
        } 
        thread_current()->fd[i] = fp; 
        return i; 
      } 
    } 
  } 
  return -1; 
} 

int filesize (int fd) 
{ 
  if (thread_current()->fd[fd] == NULL) 
  { 
    exit(-1); 
  }
  return file_length(thread_current()->fd[fd]); 
} 

int read (int fd, void* buffer, unsigned size) 
{ 
  int i; 
  check_user_vaddr(buffer);
  if (fd == 0) 
  { 
    for (i = 0; i < size; i ++) 
    { 
      if (((char *)buffer)[i] == '\0') 
      { 
        break; 
      } 
    } 
  } 
  else if (fd > 2) 
  { 
    if (thread_current()->fd[fd] == NULL) 
    { 
      exit(-1); 
    }
    return file_read(thread_current()->fd[fd], buffer, size); 
  } 
  return i; 
} 

int write (int fd, const void *buffer, unsigned size) 
{ 
  check_user_vaddr(buffer);
  if (fd == 1) 
  { 
    putbuf(buffer, size); 
    return size; 
  } 
  else if (fd > 2) 
  { 
    if (thread_current()->fd[fd] == NULL) 
    { 
      exit(-1); 
    }
    if (thread_current()->fd[fd]->deny_write) 
    { 
      file_deny_write(thread_current()->fd[fd]); 
    }
    return file_write(thread_current()->fd[fd], buffer, size); 
  } 
  return -1; 
} 

void seek (int fd, unsigned position) 
{ 
  if (thread_current()->fd[fd] == NULL) 
  { 
    exit(-1); 
  }
  file_seek(thread_current()->fd[fd], position); 
}

unsigned tell (int fd) 
{ 
  if (thread_current()->fd[fd] == NULL) 
  { 
    exit(-1); 
  }
  return file_tell(thread_current()->fd[fd]); 
} 

void close (int fd) 
{ 
  struct file* fp;
  if (thread_current()->fd[fd] == NULL) 
  { 
    exit(-1); 
  }
  fp = thread_current()->fd[fd]; 
  thread_current()->fd[fd] = NULL;

  return file_close(thread_current()->fd[fd]); 
}


void check_user_vaddr(const void *vaddr) 
{ 
  if (!is_user_vaddr(vaddr)) 
  { 
    exit(-1); 
  } 
}