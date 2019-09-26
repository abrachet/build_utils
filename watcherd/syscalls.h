/**
 * Alex Brachet (abrachet@cs.purdue.edu)
 */

#include <sys/inotify.h> // for IN_CLOEXEC and sturct inotify_event
#include <signal.h> // for SIGKILL
#include <stdint.h>

typedef uint64_t size_t;
typedef int64_t ssize_t;

extern int syscall(int, ...);

static inline ssize_t read(int fd, void *buf, size_t len) {
    return syscall(0, fd, buf, len);
}

static pid_t fork(void) {
    return syscall(57);
}

static int execve(const char *filename, char *const argv[], char *const envp[]) {
    return syscall(59, filename, argv, envp);
}

static inline void _exit(int status) {
    syscall(60, status);
}

#define kill(pid, sig) syscall(62, pid, sig)
#define inotify_init1(flags) syscall(294, flags)
#define inotify_add_watch(fd, path, mask) syscall(254, fd, path, mask)
