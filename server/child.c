#include "func.h"
//创建子进程
void make_child(pData p,int n)
{
	int i;
	pid_t pid;
	int fds[2];
	for(i=0;i<n;i++)
	{
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid=fork();
		if(!pid)
		{
			close(fds[0]);
			child_handle(fds[1]);
		}
		close(fds[1]);
		p[i].pid=pid;
		p[i].fd=fds[0];
		p[i].busy=0;
		printf("pid=%d,fds[0]=%d\n",pid,fds[0]);
	}
}

void child_handle(int fd)
{
	int sfd;
	int len;
	char unbusy='o';
	int exit_flag;
	char buf[100];
	memset(buf,0,sizeof(buf));//清空命令缓存，不然下一个命令无法接收
	recv_fd(fd,&sfd,&exit_flag);//接收任务
	if(exit_flag)
	{
		printf("I am child,I will exit\n");
		exit(0);
	}
	printf("I will work %d\n",sfd);
	while(1)
	{
		int ret;
		memset(buf,0,sizeof(buf));//清空数组buf
		recv_n(sfd,(char*)&len,sizeof(len));
		recv_n(sfd,buf,len);
		printf("客户端的命令请求是：%s\n",buf);
		ret=cond_test(buf,sfd);
		if(-1==ret)
		{
			printf("test fail!\n");
			return;
		}
		printf("\n");
	}
	write(fd,&unbusy,sizeof(char));//告诉父进程子进程非忙碌
	close(sfd);
}
