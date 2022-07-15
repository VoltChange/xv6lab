#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc,char* argv[])
{
    if(argc==1){
        printf("no parameter\n");
    }else{
        int duration=atoi(argv[1]);
        sleep(duration);
    }
    exit(0);
}