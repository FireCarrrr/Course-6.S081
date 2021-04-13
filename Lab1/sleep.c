#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // 验证参数个数
    if(argc < 2) {
        // 向标准出错输出
        fprintf(2, "usages: sleep [ticks]...\n");
        exit(1);
    }
    // 把参数转化成数字
    int ticks = atoi(argv[1]);
    // system call sleep
    sleep(ticks);
    exit(0);
}