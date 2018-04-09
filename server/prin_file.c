#include "func.h"

int prin_file(int sfd)
{

	int fd;
	int ret;
	int len;
	train d;
	char buf[10000]={0};
	strcpy(d.buf,"putsdown");
	d.len=strlen(d.buf);
	send_n(sfd,(char*)&d,4+d.len);//通知客户端准备发送文件
	recv_n(sfd,(char*)&len,sizeof(len));
	recv_n(sfd,buf,len);
	fd=open(buf,O_RDWR|O_CREAT,0666);
	off_t f_size;
	ret=recv_n(sfd,(char*)&len,sizeof(len));
	ret=recv_n(sfd,(char*)&f_size,len);//接文件长度
	printf("文件大小为：%ld 字节\n",f_size);
	float f=0;
	int j=1;
	time_t now,last;
	now=time(NULL);
	last=now;
	//以时间打印百分比
	while(1)
	{
		ret=recv_n(sfd,(char*)&len,sizeof(len));
		if(-1==ret)
		{
			printf("recv_n len\n");
			return -1;
			
		}
		if(len>0)
		{
			f=f+len;
			time(&now);
			if(now-last>=1)
			{
				printf("%5.2f%s\r",f/f_size*100,"%");
				fflush(stdout);
				last=now;
			}
			ret=recv_n(sfd,buf,len);
			write(fd,buf,len);
			if(-1==ret)
			{
				printf("write：%5.2f%s\n",f/f_size*100,"%");
				break;
			}
		}else{
			printf("          \r");
			printf("%d%s\n",100,"%");
			break;
		}
	}
	close(fd);
}
