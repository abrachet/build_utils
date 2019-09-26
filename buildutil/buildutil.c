/**
 * Alex Brachet (abrachet@cs.purdue.edu)
 */

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <unistd.h>

static inline int file_exists(const char *name) { return !access(name, F_OK); }

static int file_is_locked(const char *name) {
  int fd = open(name, O_EVTONLY);
  if (fd < 0) {
    return 0;
  }

  int locked = flock(fd, LOCK_EX | LOCK_NB);
  int errnoval = errno;
  (void)close(fd);
  if (locked == -1) {
    assert(errnoval == EWOULDBLOCK);
    (void)errnoval;
    errno = 0;
    return 1;
  }

  return 0;
}

static inline int report_file_error(const char *filename) {
  int errval = errno;
  assert(errval);
  fprintf(stderr, "Couldn't open '%s': %s", filename, sys_errlist[errval]);
  errno = 0;
  return 1;
}

static pid_t create_daemon(const char *argv[]) {
  pid_t pid = fork();

  if (pid < 0) {
    perror("Couldn't create child process.");
    return 0;
  }

  if (!pid) {
    struct rlimit rl;
    int res = getrlimit(RLIMIT_STACK, &rl);
    assert(!res);
    (void)res;
    rl.rlim_cur = 3 * 1024;
    res = setrlimit(RLIMIT_STACK, &rl);
    assert(!res);
    (void)res;

    // TODO: play around with RSS size.

    close(0);
    if (open("/dev/null", 0) < 0)
        _exit(127);
    close(1);
    if (open(".stdout", O_CREAT | O_WRONLY, 0664) < 0)
        _exit(127);
    close(2);
    if (open(".stderr", O_CREAT | O_WRONLY, 0664) < 0)
        _exit(127);

    assert(argv[0] && "invalid argv");

    execvp(*argv, (char *const *)argv);
    perror("Couldn't start daemon");
    _exit(127);
  }

  return pid;
}

static char *create_full_path(const char *filename) {
  char *cwd = getcwd(0, 0);
  size_t len = strlen(cwd);
  char *re;
  if ((re = realloc(cwd, len + strlen(filename))))
    cwd = re;
  else {
    perror("Allocation failure");
    (void)free(cwd);
    return NULL;
  }
  strcpy(cwd + len, filename);
  return cwd;
}

static void kill_process(int pidfd) {
  FILE *pidfile = fdopen(pidfd, "r+");
  pid_t pid;
  fscanf(pidfile, "%d", &pid);
  int i = kill(pid, SIGKILL);
  assert(!i);
  (void)i;
  (void)fclose(pidfile);
}

static int init_restart(int argc, const char **argv) {
  assert(!strcmp(argv[0], "init") || !strcmp(argv[0], "restart"));
  bool init = !strcmp(argv[0], "init");
  const char *watchfile = argc > 1 ? argv[1] : ".watchfile";
  const char *lockfile = argc > 2 ? argv[2] : "pid.lock";

  if (file_exists(lockfile) && init) {
    if (file_is_locked(lockfile)) {
      fprintf(stderr,
              "'%s' already exists and is owned by a running "
              "process.\nTo kill it and start a new watcher use:"
              " 'buildutil restart' as a privileged user\n",
              lockfile);
      return 1;
    }
    fprintf(stderr, "Warning: '%s' already exists.\n", lockfile);
  } else if (!file_exists(lockfile) && !init) {
    fprintf(stderr,
            "'%s' doesn't exist.\nDid you mean to use "
            "'buildutil init'?\n",
            lockfile);
    return 1;
  }

  int wfd = open(watchfile, O_CREAT, 0754);
  if (wfd < 0)
    return report_file_error(watchfile);
  // In case it already existed and wasn't created by open.
  (void)fchmod(wfd, 0754);
  (void)close(wfd);

  int flags = O_CREAT | O_EXLOCK | O_NONBLOCK;
  flags |= init ? O_WRONLY : O_RDWR;
  int lfd = open(lockfile, flags, 0644);
  if (lfd < 0) {
    if (init)
      return report_file_error(lockfile);
    int errval = errno;
    if (errval == EACCES) {
      fputs("Do not have permissions to open locked file for"
            "reading/writing. Run as root\n",
            stderr);
      return 1;
    }
    errno = errval;
    return report_file_error(lockfile);
  }

  char *watchfile_path = create_full_path(watchfile);
  if (!watchfile_path)
    return 1;

  if (!init) {
    if (file_is_locked(lockfile))
      kill_process(lfd);
    (void)ftruncate(lfd, 0);
  }

  const char *args[] = {"watcherd", watchfile_path, NULL};
  pid_t pid = create_daemon(args);
  if (!pid)
    return 1;

  dprintf(lfd, "%d", pid);
  close(lfd);

  return 0;
}

int check(int argc, const char **argv) {
  assert(!strcmp(argv[0], "check"));
  const char *lockfile = argc > 1 ? argv[1] : "pid.lock";

  if (file_exists(lockfile)) {
    if (file_is_locked(lockfile))
      fputs("daemon is properly running", stderr);
    fputs("daemon has been killed", stderr);
  }

  return 0;
}

int main(int argc, const char **argv) {
  if (argc < 2) {
    fputs("Not enough arguments\n", stderr);
    return 1;
  }

  if (!strcmp(argv[1], "init") || !strcmp(argv[1], "restart"))
    return init_restart(argc - 1, argv + 1);
  if (!strcmp(argv[1], "check"))
    return check(argc - 1, argv + 1);

  fprintf(stderr, "Unkown argument '%s'\n", argv[1]);
  return 1;
}
