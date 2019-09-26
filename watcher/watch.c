/**
 * Alex Brachet (abrachet@cs.purdue.edu) 
 */

#include "syscalls.h"

static char buf[1024];

void runfile(char *const argv[]) {
    pid_t pid = fork();

    if (pid < 0)
        goto die;
    if (pid > 0)
       return;

    execve(*argv, argv, 0);
die:
    kill(0, SIGKILL);
}

int main(int argc, const char **argv) {
    int inotifyfd = inotify_init1(IN_CLOEXEC);
    if(inotify_add_watch(inotifyfd, argv[1], IN_MODIFY) < 0)
        _exit(127);

    for (;;) {
        (void) read(inotifyfd, buf, 1024);
        struct inotify_event* event = (struct inotify_event*) buf;
        runfile((char *const *) argv + 1); 
    }
}
