
#include <fcntl.h> // for oflags
#include <sys/inotify.h> // for IN_CLOEXEC
#include <stdint.h>

extern int syscall(int, ...);

static inline ssize_t read(int fd, void *buf, size_t len) {
    return syscall(0, fd, buf, len);
}

static inline ssize_t write(int fd, const void *buf, size_t len) {
    return syscall(1, fd, buf, len);
}

// #define this because it conflicts with open in fcntl.h
#define open(path, oflag) syscall(2, path, oflag)

static inline int _exit(int status) {
    syscall(60, status);
}

#define inotify_init1(flags) syscall(294, flags)
#define inotify_add_watch(fd, path, mask) syscall(254, fd, path, mask)

int main(int argc, const char **argv) {
    // argv[1] is file to watch and then execute
    int watchfile = open(argv[1], O_RDONLY);
    int inotifyfd = inotify_init1(IN_CLOEXEC);
    int wd = inotify_add_watch(inotifyfd, argv[1], IN_ALL_EVENTS);
    if (wd < 0)
        _exit(127);


    char buf[1024];
    for (;;) {
        ssize_t amount_read = read(wd, buf, 1024);
        struct inotify_event* event = buf;
        write(1, "event!\n", 8);
    }

}
