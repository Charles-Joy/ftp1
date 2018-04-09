#include "func.h"

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("./client IP PORT\n");
		return -1;
	}
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	bzero(&ser,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));//将端口转换为网络字节序
	ser.sin_addr.s_addr=inet_addr(argv[1]);//将点分十进制的ip地址转为32位的网络字节序
	int ret;
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("connect");
		return -1;
	}


	int len;
	char buf[1000000]={0};


	int epfd;
	epfd=epoll_create(1);
	struct epoll_event event,evs[2];
	bzero(&event,sizeof(event));
	event.data.fd=sfd;
	event.events=EPOLLIN;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
	if(ret==-1)
	{
		perror("epoll_ctl1");
		return -1;
	}
	event.data.fd=0;
	event.events=EPOLLIN;
	ret=epoll_ctl(epfd,EPOLL_CTL_ADD,0,&event);
	if(ret==-1)
	{
		perror("epoll_ctl2");
		return -1;
	}

	//	train t;
	//	bzero(&t,sizeof(t));
	char S_cond[35];
	bzero(&S_cond,sizeof(S_cond));
	int ret1;
	int i;
	while(1)
	{
		bzero(evs,sizeof(evs));
		ret1=epoll_wait(epfd,evs,3,-1);
		if(-1==ret1)
		{
			perror("epoll_wait");
			return -1;
		}
		for(i=0;i<ret1;i++)
		{
			if(0==evs[i].data.fd)
			{
				memset(S_cond,0,sizeof(S_cond));
				ret=read(0,S_cond,sizeof(S_cond));
				if(ret>0)
				{
					printf("读取命令成功!\n");
					ret=cond_tran(S_cond,sfd);
					if(-1==ret)
					{
						perror("cond_tran");
						return -1;
					}
				}else{
					perror("read");
					return -1;
				}
			}

			if(sfd==evs[i].data.fd)
			{
				memset(buf,0,sizeof(buf));
				ret=recv(sfd,&len,sizeof(len),0);
				if(0==ret)
				{
					printf("server close\n");
					goto end;
				}
				ret=recv(sfd,buf,len,0);
				if(0==ret)
				{
					printf("server close\n");
					goto end;
				}//服务器断开，客户端退出

				printf("%s\n",buf);
				if(0==strncmp(buf,"getsdown",8))
				{
					prin_file(sfd);
				}
				if(0==strncmp(buf,"putsdown",8))
		    	{
					char fname[35]={0};
					char name[35]={0};
					strcpy(name,(char*)&S_cond+5);
					strncpy(fname,name,strlen(name)-1);
					tran_file(sfd,fname);
					printf("上传成功！\n");

				}
			}		
		}
	}
end:
	close(sfd);
}
