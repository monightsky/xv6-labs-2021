#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
  return buf;
}

void
find(char *path, char *f)
{
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find open %s fail\n", path);
        close(fd);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find fstat fail\n");
        exit(1);
    }

    switch (st.type) {
        case T_FILE:
            if (strcmp(f, fmtname(path)) == 0) {
                printf("%s\n", path);
                return;
            }
            break;
        case T_DIR:
            char buf[512], *p;
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';

            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0 || strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, f);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(2, "Usage: find <target_file> <target_path>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}