
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_POSIX_CONFIG_H_INCLUDED_
#define _NGX_POSIX_CONFIG_H_INCLUDED_


#if (NGX_HPUX)
#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED  1
#define _HPUX_ALT_XOPEN_SOCKET_API
#endif


#if (NGX_TRU64)
#define _REENTRANT
#endif


#if (NGX_GNU_HURD)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE             /* accept4() */
#endif
#define _FILE_OFFSET_BITS       64
#endif


#ifdef __CYGWIN__
#define timezonevar             /* timezone is variable */
#define NGX_BROKEN_SCM_RIGHTS   1
#endif


#include <sys/types.h>
#include <sys/time.h>
#if (NGX_HAVE_UNISTD_H)
#include <unistd.h>
#endif
#if (NGX_HAVE_INTTYPES_H)
#include <inttypes.h>
#endif
#include <stdarg.h>
#include <stddef.h>             /* offsetof() */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <glob.h>
#include <time.h>
#if (NGX_HAVE_SYS_PARAM_H)
#include <sys/param.h>          /* statfs() */
#endif
#if (NGX_HAVE_SYS_MOUNT_H)
#include <sys/mount.h>          /* statfs() */
#endif
#if (NGX_HAVE_SYS_STATVFS_H)
#include <sys/statvfs.h>        /* statvfs() */
#endif

#if (NGX_HAVE_SYS_FILIO_H)
#include <sys/filio.h>          /* FIONBIO */
#endif
#include <sys/ioctl.h>          /* FIONBIO */

#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/wait.h>
//#include <sys/mman.h>
#include <sys/resource.h>
#include <sched.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>        /* TCP_NODELAY */
#include <arpa/inet.h>
#include <netdb.h>
//#include <sys/un.h>

#if (NGX_HAVE_LIMITS_H)
#include <limits.h>             /* IOV_MAX */
#endif

#ifdef __CYGWIN__
#include <malloc.h>             /* memalign() */
#endif

#if (NGX_HAVE_CRYPT_H)
#include <crypt.h>
#endif


#ifndef IOV_MAX
#define IOV_MAX   16
#endif


#include <ngx_auto_config.h>


#if (NGX_HAVE_DLOPEN)
#include <dlfcn.h>
#endif


#if (NGX_HAVE_POSIX_SEM)
#include <semaphore.h>
#endif


#if (NGX_HAVE_POLL)
#include <poll.h>
#endif


#if (NGX_HAVE_KQUEUE)
#include <sys/event.h>
#endif


#if (NGX_HAVE_DEVPOLL) && !(NGX_TEST_BUILD_DEVPOLL)
#include <sys/ioctl.h>
#include <sys/devpoll.h>
#endif


#if (NGX_HAVE_FILE_AIO)
#include <aio.h>
typedef struct aiocb  ngx_aiocb_t;
#endif


#define NGX_LISTEN_BACKLOG  511

#define ngx_debug_init()


//extern char **environ;





struct hostent {
               char  *h_name;            /* official name of host */
               char **h_aliases;         /* alias list */
               int    h_addrtype;        /* host address type */
               int    h_length;          /* size of address */
               char **h_addr_list;       /* list of addresses */
           };

#define AF_UNIX 0
#define SO_SNDLOWAT 0
#define SO_TYPE (0)

#define F_WRLCK 1
#define F_UNLCK 2
struct flock {
	int l_type;
	int l_whence;
};
// mmap https://github.com/hermit-os/kernel/blob/226515be603df9c05b374db1e0f422d95955825a/src/syscalls/mmap.rs#L12
#define PROT_READ (1 << 0)
#define PROT_WRITE (1 << 1)
#define MAP_SHARED (0) // i think you can ignore this
#define MAP_FAILED (-1) // if not 0 then there is an error https://github.com/hermit-os/kernel/blob/226515be603df9c05b374db1e0f422d95955825a/src/syscalls/mmap.rs#L51 


#define FIOASYNC 0 // make socket async and listen on sigio Martin fragen ob es das gibt
#define F_SETOWN 0


//sys/socket.h


/* Ancillary data object manipulation macros.  */
#if __glibc_c99_flexarr_available
# define CMSG_DATA(cmsg) ((cmsg)->__cmsg_data)
#else
# define CMSG_DATA(cmsg) ((unsigned char *) ((struct cmsghdr *) (cmsg) + 1))
#endif
#define CMSG_NXTHDR(mhdr, cmsg) __cmsg_nxthdr (mhdr, cmsg)
#define CMSG_FIRSTHDR(mhdr) \
  ((size_t) (mhdr)->msg_controllen >= sizeof (struct cmsghdr)		      \
   ? (struct cmsghdr *) (mhdr)->msg_control : (struct cmsghdr *) 0)
#define CMSG_ALIGN(len) (((len) + sizeof (size_t) - 1) \
			 & (size_t) ~(sizeof (size_t) - 1))
#define CMSG_SPACE(len) (CMSG_ALIGN (len) \
			 + CMSG_ALIGN (sizeof (struct cmsghdr)))
#define CMSG_LEN(len)   (CMSG_ALIGN (sizeof (struct cmsghdr)) + (len))

/* Given a length, return the additional padding necessary such that
   len + __CMSG_PADDING(len) == CMSG_ALIGN (len).  */
#define __CMSG_PADDING(len) ((sizeof (size_t) \
                              - ((len) & (sizeof (size_t) - 1))) \
                             & (sizeof (size_t) - 1))


struct cmsghdr {
               size_t cmsg_len;    /* Data byte count, including header
                                      (type is socklen_t in POSIX) */
               int    cmsg_level;  /* Originating protocol */
               int    cmsg_type;   /* Protocol-specific type */
           /* followed by
               unsigned char cmsg_data[]; */
           };

#define	SCM_RIGHTS	0x01		/* access rights (array of int) */
#define	MSG_TRUNC	0x10		/* data discarded before delivery */
#define	MSG_CTRUNC	0x20		/* control data lost before delivery */




/* Bits in `sa_flags'.  */
//#define	SA_NOCLDSTOP  1		 /* Don't send SIGCHLD when children stop.  */
//#define SA_NOCLDWAIT  2		 /* Don't create zombie on child death.  */
//#define SA_SIGINFO    4		 /* Invoke signal-catching function with */
//# define SA_RESTART   0x10000000 /* Restart syscall on signal return.  */
//# define SA_NODEFER   0x40000000 /* Don't automatically block the signal when */
//# define SA_RESETHAND 0x80000000 /* Reset to SIG_DFL on entry to handler.  */
//# define SA_INTERRUPT 0x20000000 /* Historical no-op.  */

/* Some aliases for the SA_ constants.  */
# define SA_NOMASK    SA_NODEFER
# define SA_ONESHOT   SA_RESETHAND
# define SA_STACK     SA_ONSTACK


#define setpriority(x,y,z) (0)
//#define setpriority(x,y,z) (0)

#define socketpair(x,y, z,a) (-1)

//#define setrlimit(x,y) (-1)

#define sigsuspend(x) (-1)


#define mmap(addr, e, d, c,b, a) (NULL)
#define munmap(addr, length) (-1)

#define utimes(x, y) (-1)

#define getppid(x) (-1)


#define getpwnam(x) (NULL)
#define getgrnam(x) (NULL)
#define chown(x, y, z) (1)

#define gethostbyname(x) (NULL)
#define gethostname(x, y) (NULL)
#define initgroups(x, y) (-1)
#define chdir(x) (-1)
#define crypt(x, y) (NULL)
#define setsid() (-1)
#define ftruncate(flides, len) (-1)
//#define posix_memalign(ptr, align, size) (-1)

#define SO_SNDLOWAT 0
#define IP_BIND_ADDRESS_NO_PORT 0


#undef O_APPEND
#define O_APPEND 0
#endif /* _NGX_POSIX_CONFIG_H_INCLUDED_ */
