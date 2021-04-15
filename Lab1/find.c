#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *dirName, char *fileName) {

    int fd;
    char buf[512], *p;
    struct stat st;
    // Data structure for a directory entry
    struct dirent de;

    // open dir
    if((fd = open(dirName, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", dirName);
        return;
    }

    // the fstat system call retrieves infomation from inode that
    // a file descriptor refers to
    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dirName);
        close(fd);
        return;
    }

    // 如果是 file
    if(st.type == T_FILE) {
        if(strcmp(fileName, dirName)) {
            printf("%s\n", dirName);
        }
        close(fd);
        return;
    }

    // 如果是 directory
    if(st.type == T_DIR) {

        if(strlen(dirName) + 1 + DIRSIZ + 1 > sizeof buf) {
            fprintf(2, "find: path too lang\n");
            close(fd);
            return;
        }
        strcpy(buf, dirName);
        p = buf + strlen(buf);
        // dirName/
        *p++ = '/';

        // Directory is a file containing a sequence of dirent srtuctures
        while(read(fd, &de, sizeof(de)) == sizeof(de)) {

            if(de.inum == 0) {
                continue;
            }
            if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) {
                continue;
            }

            // copy memory area
            memmove(p, de.name, DIRSIZ);
            // end of a string
            p[DIRSIZ] = 0;

            if(stat(buf, &st) < 0) {
                fprintf(2, "find: cannot stat %s\n", buf);
                close(fd);
                return;
            }

            if(st.type == T_DIR) {
                find(buf, fileName);
            }

            if(st.type == T_FILE) {
                if(strcmp(de.name, fileName) == 0) {
                    printf("%s\n", buf);
                }
            }
        }
        close(fd);
    }
}

int main(int argc, char *argv[]) {
    
    if(argc != 3) {
        fprintf(2, "usage: find dirName fileName\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}

