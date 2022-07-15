#include "kernel/types.h"
#include "user/user.h"
/*
如果数据没有准备好，那么对管道执行的read会一直等待
直到有数据了或者其他绑定在这个管道写端口的描述符都已经关闭了
*/
int 
main(int argc, char* argv[] ){
    int pid;
    //数组下标0为读端、1为写端；
    int pipe_ptoc[2];//父到子管道的文件标识符
    int pipe_ctop[2];//子到父管道的文件标识符
    char buf[20];
    //为父子进程建立管道
    pipe(pipe_ctop); 
    pipe(pipe_ptoc);

    
    write(pipe_ptoc[1], "ping",4);//父向子写

    pid=fork();
    // 子进程
    if(pid== 0){
        close(pipe_ptoc[1]);//关闭父到子的写端
        read(pipe_ptoc[0],buf, 4);//子进程从父读
        close(pipe_ptoc[0]);//关闭父到子的读端
        printf("%d: received %s\n",getpid(), buf);
        write(pipe_ctop[1], "pong", sizeof(buf));//子向父写
        exit(0);
    }
    // 父进程
    else{
        wait(0);
        close(pipe_ctop[1]);//释放子到父的写端
        read(pipe_ctop[0], buf, sizeof(buf));//父从子读
        close(pipe_ctop[0]);//关闭子到父的读端
        printf("%d: received %s\n", getpid(), buf);
        
        exit(0);
    }
    
}