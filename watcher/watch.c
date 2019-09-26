
#include <fcntl.h> // for oflags

extern int syscall(int, ...);


#define open(path, oflag) syscall(2, path, oflag)

static inline int write(int fd, const char* buf, int len) {
    return syscall(3, fd, buf, len);
}

int main(int argc, const char **argv) {
    // argv[1] is file to watch and then execute
    int fd = open(argv[1], O_RDONLY);
    write(1, "Hello\n", 6);
    for (;;);
}
