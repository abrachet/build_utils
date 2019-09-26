#include <sys/file.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int fd = open(".lock", 0);
    if (fd < 0) {
        perror("");
    }
    if (flock(fd, LOCK_SH) < 0)
        perror("flock");

    for (;;);
}