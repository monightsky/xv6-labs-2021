#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    if (fork()) {
        char buf[2], info[2];
        buf[1] = 0;
        info[0] = 'a';
        info[1] = 0;
        if (write(p[1], info, 1) != 1) {
            fprintf(2, "parent write fail\n");
            exit(1);
        }
        close(p[1]);
        if (read(p[0], buf, 1) <= 0) {
            fprintf(2, "parent read fail\n");
            exit(1);
        }
        close(p[0]);
        fprintf(1, "%d: received pong\n", getpid());
        exit(0);
    } else {
        char buf[2];
        buf[1] = 0;
        if (read(p[0], buf, 1) <= 0) {
            fprintf(2, "child read fail\n");
            exit(1);
        }
        fprintf(1, "%d: received ping\n", getpid());
        close(p[0]);
        if (write(p[1], buf, 1) <= 0) {
            fprintf(2, "child write fail\n");
            exit(1);
        }
        close(p[1]);
        exit(0);
    }
}