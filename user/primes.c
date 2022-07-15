#include "kernel/types.h"
#include "user/user.h"
const int max=34;
void primeselect(int p[]);

int main(int argc,char* argv[])
{
    int p[2];
    pipe(p);
    int nums[max];
    //初始化，0对应2
    for(int i=0;i<max;i++)
    {
       nums[i]=1;
    }
    write(p[1],nums,max*sizeof(int));
    primeselect(p);
    exit(0);
}
void primeselect(int p[])
{
    //printf("pid：%d\n",getpid());
    int p_child[2];//子管道的文件标识符
    pipe(p_child);
    int pid;
    int nums[max];
    for(int i=0;i<max;i++)
    {
       nums[i]=0;//初始化
    }

    int prime=-1;
    //从上一管道读取内容
    close(p[1]);
    read(p[0],nums,max*sizeof(int));
    for(int i=0;i<max;i++)
    {

        if(nums[i]==1)
        {
            prime=i+2;
            printf("prime %d\n",prime);
            break;
        }
    }
    for(int i=prime-2;i<max;i++)
    {
        
        if(nums[i]==1&&(i+2)%prime==0)
        {
            nums[i]=0;
        }
    }
    /* 测试
    for(int i=0;i<max;i++)
    {
        printf("%d:%d  ",i+2,nums[i]);
    }
     printf("\n");
    */
    close(p[0]);

    pid=fork();
    if(pid==0)//子进程
    {
        int unselectednum=0;
        for(int i=0;i<max;i++)
        {
            if(nums[i]==1)
                unselectednum++;
        }
        if(unselectednum>0)
        {
            //向子管道写
            write(p_child[1],nums,max*sizeof(int));
            primeselect(p_child);
        }
        exit(0);
    }else//父进程
    {
        wait(0);
        exit(0);
    }
}