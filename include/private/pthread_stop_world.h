/*
 * Copyright (c) 1994 by Xerox Corporation.  All rights reserved.
 * Copyright (c) 1996 by Silicon Graphics.  All rights reserved.
 * Copyright (c) 1998 by Fergus Henderson.  All rights reserved.
 * Copyright (c) 2000-2009 by Hewlett-Packard Development Company.
 * All rights reserved.
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 */

#ifndef GC_PTHREAD_STOP_WORLD_H
#define GC_PTHREAD_STOP_WORLD_H

EXTERN_C_BEGIN

struct thread_stop_info {
#   ifdef SIGNAL_BASED_STOP_WORLD
      volatile AO_t last_stop_count;
                        /* The value of GC_stop_count when the thread   */
                        /* last successfully handled a suspend signal.  */
#     ifdef GC_ENABLE_SUSPEND_THREAD
        volatile AO_t ext_suspend_cnt;
                        /* An odd value means thread was suspended      */
                        /* externally.  Incremented on every call of    */
                        /* GC_suspend_thread() and GC_resume_thread().  */
                        /* Updated with the GC lock held, but could be  */
                        /* read from a signal handler.                  */
#     endif
#   endif

    ptr_t stack_ptr;            /* Valid only when stopped.             */

#   ifdef NACL
      /* Grab NACL_GC_REG_STORAGE_SIZE pointers off the stack when      */
      /* going into a syscall.  20 is more than we need, but it's an    */
      /* overestimate in case the instrumented function uses any callee */
      /* saved registers, they may be pushed to the stack much earlier. */
      /* Also, on amd64 'push' puts 8 bytes on the stack even though    */
      /* our pointers are 4 bytes.                                      */
#     ifdef ARM32
        /* Space for r4-r8, r10-r12, r14.       */
#       define NACL_GC_REG_STORAGE_SIZE 9
#     else
#       define NACL_GC_REG_STORAGE_SIZE 20
#     endif
      ptr_t reg_storage[NACL_GC_REG_STORAGE_SIZE];
#   elif defined(PLATFORM_HAVE_GC_REG_STORAGE_SIZE)
      word registers[PLATFORM_GC_REG_STORAGE_SIZE]; /* used externally */
#   endif
};

GC_INNER void GC_stop_init(void);

EXTERN_C_END

#endif
