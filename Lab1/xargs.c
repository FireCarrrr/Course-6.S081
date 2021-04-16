#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LEN 1024

int main(int argc, char *argv[]) {

    if(argc < 2) {
        fprintf(2, "xargs: usage xargs command args");
        exit(1);
    }
    
    // 存放 exec 中运行指令的参数
    char *argv_n[MAXARG];
    // agrv[0] = xagrs,省略
    for(int i = 1; i < argc; i++) {
        argv_n[i - 1] = argv[i];
    }
    char buf[MAX_LEN];
    // left 指向当前参数在 buf 中的起始地址
    char *left, *right;
    left = buf;
    right = buf;
    char c;
    int n;
    // arg_cnt 记录当前参数的个数
    int arg_cnt = argc - 1;
    
    for(;;) {
        // 读一个字符
        n = read(0, &c, 1);
        // 如果没有输入了
        if(n == 0) {
            break;
        }
        // 读到空格或者换行符
        if(c == ' ' || c == '\n') {
            // 如果读到连续的空格
            if(left == right) {
                continue;
            }
            // 字符串的结束标志
            right[0] = 0;
            // 把字符串放到参数列表中
            argv_n[arg_cnt++] = left;
            // 移动 left 指针
            left = ++right;
            // 读完了一行
            if(c == '\n') {
                argv_n[arg_cnt] = 0;
                if(fork() == 0) {
                    exec(argv_n[0], argv_n);
                } else {
                    wait(0);
                    // 清空 buf
                    memset(buf, 0, right - buf);
                    left = buf;
                    right = buf;
                    arg_cnt = argc - 1;
                }
            }
        }
        // 读到一个正常字符 
        else {
            *right++ = c;
        }
    }
    exit(0);
}