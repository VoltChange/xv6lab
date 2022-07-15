#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"


	//将路径格式化为文件名,抄自ls
char* fmt_name(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  memmove(buf, p, strlen(p)+1);
  return buf;
}

	//路径最后的文件名与要查找的文件名，若一致，打印完整路径
void eq_print(char *path, char *name){
	if(strcmp(fmt_name(path), name) == 0){
		printf("%s\n", path);
	}
}
	//在某路径中查找某文件
void find(char *path, char *findName){
	int fd;
	struct stat st;	
    //打开path
	if((fd = open(path, O_RDONLY)) < 0){
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}
	if(fstat(fd, &st) < 0){
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}
	char buf[512], *p;	
	struct dirent de;

    //判断类型
	switch(st.type){	
		case T_FILE://文件情况
			eq_print(path, findName);			
			break;
		case T_DIR://目录情况
			if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
				printf("find: path too long\n");
				break;
			}
			strcpy(buf, path);
			p = buf+strlen(buf);
			*p++ = '/';

			while(read(fd, &de, sizeof(de)) == sizeof(de)){//遍历目录下的东西
				
				if(strcmp(de.name, ".")==0 || strcmp(de.name, "..")==0)//不递归.和..
					continue;				
				memmove(p, de.name, strlen(de.name));
				p[strlen(de.name)] = 0;

				find(buf, findName);//递归目录
			}
			break;
	}
	close(fd);	
}

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("wrong\n");
		exit(0);
	}
	find(argv[1], argv[2]);
	exit(0);
}