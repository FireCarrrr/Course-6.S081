#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    // 子进程到父进程的 pipe
    int c_to_f[2];
    pipe(c_to_f);
    // 父进程到子进程的管道
    int f_to_c[2];
    int n;
    pipe(f_to_c);

    char c;

    if(fork() == 0) { // child
       // 子进程会复制父进程的 file table
       // 也就是说子进程也会打开父进程打开的文件
       // 当没有数据可以读的时候，read 函数会阻塞等待新数据到来或者所有指向 pipe 读 fd 关闭
       close(f_to_c[1]);
       close(c_to_f[0]);
       // 读
       n = read(f_to_c[0], &c, 1);
       if(n != 1) {
           fprintf(2, "child read error");
       }
       printf("%d: received ping\n", getpid());
       write(c_to_f[1], &c, 1);
       close(c_to_f[1]);
       close(f_to_c[0]);
       exit(0);
    } else {          // father
       close(c_to_f[1]);
       close(f_to_c[0]);
       write(f_to_c[1], &c, 1);
       n = read(c_to_f[0], &c, 1);
       if(n != 1) {
           fprintf(2, "parent read error");
       }
       printf("%d: received pong\n", getpid());
       close(c_to_f[0]);
       close(f_to_c[1]);
       wait(0);
       exit(0);
    }
}