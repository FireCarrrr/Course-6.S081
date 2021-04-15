#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 每个进程都从标准输入中读，往标准输出中写
#define R 0
#define W 1

// 每一个进程都从标准输入读，从标准输出写
void redirect(int rw, int p[]) {
    close(rw);
    dup(p[rw]);
    close(p[0]);
    close(p[1]);
}

void waitForNum() {

    int pp[2];
    int n;
    int num;
    n = read(R, &num, sizeof(num));
    if(n != 0) {
        printf("prime: %d\n", num);
        pipe(pp);
        if(fork() == 0) {
            redirect(R, pp);
            waitForNum();
        } else {
            redirect(W, pp);
            int tmp;
            for(;;) {
                n = read(R, &tmp, sizeof(tmp));
                if(n == 0) {
                    break;
                }
                if(tmp % num != 0) {
                    write(W, &tmp, sizeof(tmp));
                }
            }
            close(W);
            wait(0);
            exit(0);
        }
    }
    exit(0);
}

int main(int argc, char *argv[]) {

    int p[2];
    pipe(p);

    if(fork() == 0) {  // 子进程
       redirect(R, p);
       waitForNum();
    } else {
        redirect(W, p);
        for(int i = 2; i <= 35; i++) {
            write(W, &i, sizeof(i));
        }
        close(W);
        wait(0);
        exit(0);
    }
    return 0;
}




