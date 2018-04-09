#include "func.h"

int tran_file(int new_fd,char* fname)
{
	train d;
	strcpy(d.buf,fname);
	d.len=strlen(d.buf);
	//把文件名传输给服务器
	send(new_fd,&d,4+d.len,0);
	int fd;
	fd=open(fname,O_RDONLY);
	if(-1==fd)
	{
		perror("open");
		return -1;
	}
	struct stat buf;
	int ret=fstat(fd,&buf);
	if(-1==ret)
	{
		perror("fstat");
		return -1;
	}
	d.len=sizeof(off_t);//发送文件长度
	memcpy(d.buf,&buf.st_size,d.len);
	send_n(new_fd,(char*)&d,4+d.len);
	while((d.len=read(fd,d.buf,sizeof(d.buf)))>0)//发送文件内容
	{
		ret=send_n(new_fd,(char*)&d,4+d.len);
		if(-1==ret)
		{
			perror("send_n");
			return -1;
		}
	}
	printf("文件大小为：%ld 字节\n",buf.st_size);
	d.len=0;
	send_n(new_fd,(char*)&d,4);
	close(fd);
}
