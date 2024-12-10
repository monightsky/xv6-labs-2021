#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
    int t;
    if (argc != 2 || (t = atoi(argv[1])) < 0)
    {
        fprintf(2, "Usage: sleep <postive_num>\n");
        exit(1);
    }
    sleep(t);
    exit(0);
}