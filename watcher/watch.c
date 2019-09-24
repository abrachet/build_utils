

extern int syscall(int, ...);

static inline int write(int fd, const char* buf, int len) {
    return syscall(3, fd, buf, len);
}

int main() {
    write(1, "Hello\n", 6);
}
