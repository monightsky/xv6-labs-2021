#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    
    if (argc < 2) {
        fprintf(2, "Usage: xargs <command> [arg]...");
        exit(1);
    }
    
    
    char *commands[argc + 1];
    int i, cnt = 0;
    for (i = 0; i < argc - 1; i++) {
        commands[i] = argv[i + 1];
    }
    commands[argc] = 0;

    char buf[512];
    char c;
    i = 0;
    while (read(0, &c, 1) == 1) {
        if (c == '\n') {
            buf[i] = 0;
            cnt += 1;
            i = 0;
            commands[argc - 1] = buf;
            if (!fork()) {
                exec(commands[0], commands);
            }
        } else {
            buf[i++] = c;
        }
    }
    buf[i] = 0;
    cnt += 1;
    commands[argc - 1] = buf;
    if (strlen(buf)) {
        if (!fork()) {
            exec(commands[0], commands);
         }
    }
    while (cnt--) {
        wait(0);
    }

    exit(0);
}