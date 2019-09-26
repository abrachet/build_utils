# watcherd

This is the daemon created by `buildutil` which watches for a change to a file and then runs it.

## Basic Usage

`watcherd` watches a file for modifications using the inotify interface, upon a modification event `watcherd` runs the file passing the given arguments. Often it makes sense to use a bash script, or can be used with `buildrs(1)`.

## Build

```bash
make
# mv watcherd to desired location
```

## Notes

Command line arguments should be passed in the form `watcherd <path> [args]`. Path need not be a full path to the file to watch. Unlike most daemons which prefer to have their working directory at the root directory this makes no difference in this case. For the use cases that `watcherd` and other build utils are created they are highly susceptible to having their working directories be unmounted from the filesystem. Specifically it was created for running on school servers to run tests for large builds. This is unavoidable in this case because the file it is watching will exist in these filesystems making it busy regardless of `watcherd`'s current directory. `[args]` are what get passed.

`watcherd` is pedantic about resource usage it doesn't even link to the C library and its text and data segments are both extremely small. Moreover, it makes a concerted effor to use as little stack space as possible, 1kb is plenty of space to give. Although `watcherd` itself will never write to the 0, 1 and 2 file descriptors it expects these to exist for the process it creates, which will almost definitely be expecting them. Although this requires more kernel memory than possible alternatives, it is too clunky to ask `watcherd` create these files for the child process while keeping command line arguments simple.

Because `watcherd` is not linked to the C standard library as such syscalls are not made with the libc wrappers. Right now there is no compatability with other systems the `_start` function expects a Linux x86 abi and the syscall wrappers expect the same. Compatibility with other operating systems is near impossible given `watcherd` relies heavily on the inotify api, however other architectures is very possible. To use with another architecutre one would need to change start.s, syscalls.s and syscalls.h to change how command line arguments are retrieved, how syscalls are made and the syscall numbers respectively.
