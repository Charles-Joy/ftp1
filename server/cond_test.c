#include "func.h"

int cond_test(char *buf,int sfd)
{
	train t;
	bzero(&t,sizeof(t));
	char dirpath[100]={0};
	memset(dirpath,0,sizeof(dirpath));

	if(0==strncmp(buf,"pwd",3))
	{
		char path[128]={0};
		if(NULL==getcwd(path,sizeof(path)))
		{
			perror("getcwd");
			return 0;
		}
		puts(path);
		strcpy(t.buf,path);
		t.len=strlen(t.buf);
		send_n(sfd,(char*)&t,4+t.len);
	}

	if(0==strncmp(buf,"cd",2))
	{//返回当前目录出错
		memset(dirpath,0,sizeof(dirpath));
		strcpy(dirpath,buf+3);
		chdir(dirpath);//调用系统命令
		if(NULL==getcwd(dirpath,sizeof(dirpath)))
		{
			perror("getcwd");
			return 0;
		}
		puts(dirpath);
		strcpy(t.buf,dirpath);
		t.len=strlen(t.buf);
		send_n(sfd,(char*)&t,4+t.len);
	}

	if(0==strncmp(buf,"ls",2))
	{
		char path[128]={0};
		memset(path,0,sizeof(path));
		if(NULL==getcwd(path,sizeof(path)))
		{
			perror("getcwd");
			return 0;
		}
		DIR* dir=opendir(path);
		if(NULL==dir)
		{
			perror("opendir");
			return -1;
		}
		struct dirent* tp;
		char *sumpath;
		sumpath=(char*)malloc(500);
		memset(sumpath,0,sizeof(sumpath));
		while(tp=readdir(dir))
		{
			if(strcmp(".",tp->d_name)&&strcmp("..",tp->d_name))
			{
				bzero(dirpath,sizeof(dirpath));
				sprintf(dirpath,"%s\n",tp->d_name);
				strcat(sumpath,dirpath);
			}
		}
		puts(path);
		puts(sumpath);
		strcpy(t.buf,sumpath);
		t.len=strlen(t.buf);
		send_n(sfd,(char*)&t,4+t.len);
		closedir(dir);

	}

	if(0==strncmp(buf,"remove",6))
	{
		bzero(&dirpath,sizeof(dirpath));
    	strcpy(dirpath,buf+7);//buf的第7位开始
    	remove(dirpath);
		printf("%s 已经被移除\n",dirpath);
	}

	if(0==strncmp(buf,"gets",4))
	{
		char fname[30]={0};
		strcpy(fname,buf+5);
		printf("%s将下载至客户端\n",fname);
		tran_file(sfd,fname);
		printf("下载完成！\n");
	}

	if(0==strncmp(buf,"puts",4))
	{
		char fname[30]={0};
		strcpy(fname,buf+5);
		printf("%s将上传至服务器\n",fname);
		prin_file(sfd);
		printf("上传完成！\n");
	}
	
}
