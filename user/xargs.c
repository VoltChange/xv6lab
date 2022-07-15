#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/param.h"

const int bufsize=20;
int main(int argc, char *argv[]){
    sleep(10);
    char buf[bufsize];
    //获取标准化输入
    //按照惯例，进程从文件描述符0读取（标准输入），将输出写入文件描述符1（标准输出）
    read(0,buf,bufsize);

    //获取命令行参数
    char* newargv[MAXARG];
    int newargc=0;
    for(int i=1;i<argc;i++)
    {
        newargv[newargc++]=argv[i];//将原来的命令行参数复制到新的中
    }

    //把标准化输入的参数放到新命令行参数中
    char* p=buf;
    for(int i=0;i<bufsize;i++)
    {
        if(buf[i]=='\n')
        {
            int pid=fork();
            if(pid==0)//子
            {
                //分割
                buf[i]=0;
                newargv[newargc++]=p;
                newargv[newargc++]=0;
                
                //执行
                exec(newargv[0],newargv);
                exit(0);
            }else//父
            {
                p=&buf[i+1];
                wait(0);
            }
        }
    }
    wait(0);
    exit(0);
}