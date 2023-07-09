/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define to the type of elements in the array set by `getgroups'.
   Usually this is either `int' or `gid_t'.  */
#define GETGROUPS_T gid_t

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define if system calls automatically restart after interruption
   by a signal.  */
/* #undef HAVE_RESTARTABLE_SYSCALLS */

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define if the `setpgrp' function takes no argument.  */
#define SETPGRP_VOID 1

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* Define to a suitable ssize_t. */
/* #undef ssize_t */

/* Define to a suitable sig_atomic_t. */
/* #undef sig_atomic_t */

/* Define if you want rc to hand off exec errors to (e.g.) /bin/sh. */
#define DEFAULTINTERP "/bin/sh"

/* Define to the default path used if $PATH is empty when rc starts. */
#define DEFAULTPATH "/usr/local/bin","/usr/bin","/usr/ucb","/bin","."

/* Define if your kernel has SysV special SIGCLD semantics. */
#define HAVE_SYSV_SIGCLD 1

/* Define if your kernel supports `#!' magic numbers. */
#define HASH_BANG 1

/* Define if you have /dev/fd. */
#define HAVE_DEV_FD 1

/* Define if you have /proc/self/fd. */
/* #undef HAVE_PROC_SELF_FD */

/* Define if you have named pipes. */
/* #undef HAVE_FIFO */

/* Define if quad_t is a native type. */
/* #undef HAVE_QUAD_T */

/* Define if you have rlim_t. */
#define HAVE_RLIM_T 1

/* Define if you have sigsetjmp(). */
#define HAVE_SIGSETJMP 1

/* Define if you want rc to encode strange characters in the environment. */
#define PROTECT_ENV 1

/* Define if you have extra builtins. */
/* #undef RC_ADDON */

/* Define if you want echo as a builtin. */
#define RC_ECHO 1

/* Define if you want rc to support broken apps, like a job control shell. */
#define RC_JOB 1

/* Define if you want to use editline. */
/* #undef EDITLINE */

/* Define if you want to use GNU readline. */
/* #undef READLINE */

/* Define if you have the older readline, with rl_deprep_terminal. */
/* #undef READLINE_OLD */

/* Define if RLIMIT_foo defines need _KERNEL. */
/* #undef RLIMIT_NEEDS_KERNEL */

/* Define if rlim_t is quad_t. */
/* #undef RLIM_T_IS_QUAD_T */

/* Define to release date. */
#define RELDATE "1999-05-28"

/* Define if you have SA_INTERRUPT (and sigaction()). */
/* #undef HAVE_SA_INTERRUPT */

/* Define if you have POSIX getgroups(). */
#define HAVE_POSIX_GETGROUPS 1

/* Define if you have va_copy(). */
#define HAVE_VA_COPY 1

/* Define if you have the getgroups function.  */
#define HAVE_GETGROUPS 1

/* Define if you have the mkfifo function.  */
/* #undef HAVE_MKFIFO */

/* Define if you have the setpgrp function.  */
#define HAVE_SETPGRP 1

/* Define if you have the setrlimit function.  */
#define HAVE_SETRLIMIT 1

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/resource.h> header file.  */
#define HAVE_SYS_RESOURCE_H 1

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/types.h> header file.  */
#define HAVE_SYS_TYPES_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the readline library (-lreadline).  */
/* #undef HAVE_LIBREADLINE */

/* Name of package */
#define PACKAGE "rc"

/* Version number of package */
#define VERSION "1.6"

