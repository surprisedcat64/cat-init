#include <unistd.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>


void init_agetty() {
    pid_t pid = fork();
    if (pid < 0) _exit(1);
    if (pid == 0) {
        char *argv[] = {
            "agetty",
            "-L",
            "tty1",
            "linux",
            NULL
        };
	extern char **environ;
        execve("/sbin/agetty", argv, environ);
        _exit(1);
    }
}
void mount_filesystems(void)
{
    mkdir("/proc", 0555);
    mkdir("/sys", 0555);
    mkdir("/dev", 0755);
    mkdir("/run", 0755);
    mount("devtmpfs", "/dev", "devtmpfs", 0, "mode=0755"); 
    mkdir("/dev/pts", 0755);
    mkdir("/dev/shm", 1777);
    mkdir("/sys/fs", 0755);
    mkdir("/sys/fs/cgroup", 0755);
    mount("proc", "/proc", "proc", 0, NULL);
    mount("sysfs", "/sys", "sysfs", 0, NULL);
    mount("tmpfs", "/run", "tmpfs", 0, "mode=0755");
    mount("devpts", "/dev/pts", "devpts", 0, "gid=5,mode=620");
    mount("tmpfs", "/dev/shm", "tmpfs", 0, "mode=1777");
    mount("cgroup2", "/sys/fs/cgroup", "cgroup2", 0, NULL);
}

int efi_detect(void)
{
    return access("/sys/firmware/efi", F_OK) == 0;
}



int main () {
	mount_filesystems();
	if (efi_detect()) {
   		 mkdir("/sys/firmware/efi/efivars", 0755);

    		 mount("efivarfs",
         		 "/sys/firmware/efi/efivars",
         		 "efivarfs",
         		 0,
         		 NULL);
	}
    mkdir("/etc/cat.d", 0755);
    system("/bin/sh /etc/cat.d/init.sh");
    // its suggested to do something such as 01-network 02-seatd 03-whatever
    init_agetty();
    while (1) {
        waitpid(-1, NULL, 0);
}
return 0;
}
