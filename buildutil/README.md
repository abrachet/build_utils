# buildutil

`buildutil` is used to start and manage `watcherd`

## Usage

```bash
buildutil init [watchfile] [lockfile] # starts `watcherd` to be watching watchfile
```
By default watchfile will be ".watchfile" and the lockfile will be "pid.lock". The watchfile is what is watched for an event and run by watcherd. The lockfile is the pidfile for the watcher daemon, to create a new instance of `watcherd` give buildutil a different lockfile name. The lockfile is referred to as such because it will be locked by `watcherd` until it exits.

```bash
buildutil restart [watchfile] [lockfile] # kills a currently running watcherd to restart it assuming proper privileges
```

```bash
buildutil check [lockfile] # checks if a process currently owns the 
```
