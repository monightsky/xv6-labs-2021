#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
do_prime(int p[])
{
    close(p[1]);
    int prime, n;
    if (read(p[0], &prime, 4) != 4) {
        close(p[0]);
        exit(1);
    }
    fprintf(1, "prime %d\n", prime);
    int np[2];
    pipe(np);
    if (fork()) {
        close(np[0]);
        while (read(p[0], &n, 4) == 4) {
            if (n % prime) {
                if (write(np[1], &n, 4) != 4) {
                    fprintf(2, "%d write fail\n", getpid());
                    exit(1);
                }
            }
        }
        close(p[0]);
        close(np[1]);
        wait(0);
    } else {
        do_prime(np);
        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if (fork()) {
        close(p[0]);
        int i;
        for (i = 2; i <= 35; i++) {
            if (write(p[1], &i, 4) != 4) {
                fprintf(2, "%d write fail\n", getpid());
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
        exit(0);
    } else {
        do_prime(p);
        exit(0);
    }
}