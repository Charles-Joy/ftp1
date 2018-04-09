#include "func.h"

int cond_tran(char* cond,int sfd)
{
	train t;
	bzero(&t,sizeof(t));
	if(0==strncmp(cond,"pwd",3))
	{
		strncpy(t.buf,cond,strlen(cond));
		t.len=strlen(t.buf)-1;
		printf("输入的命令是：%s",t.buf);
		send_n(sfd,(char*)&t,4+t.len);
	}else if(0==strncmp(cond,"cd",2))
	{
		strncpy(t.buf,cond,strlen(cond));
		t.len=strlen(t.buf)-1;
		printf("输入的命令是：%s",t.buf);
		send_n(sfd,(char*)&t,4+t.len);
	}else if(0==strncmp(cond,"ls",2))
	{
		strncpy(t.buf,cond,strlen(cond));
		t.len=strlen(t.buf)-1;
		printf("输入的命令是：%s",t.buf);
		send_n(sfd,(char*)&t,4+t.len);
	}else if(0==strncmp(cond,"remove",6))
	{
		strncpy(t.buf,cond,strlen(cond));
		t.len=strlen(t.buf)-1;
		printf("输入的命令是：%s",t.buf);
		send_n(sfd,(char*)&t,4+t.len);
		printf("remove finish!\n");
	}else if(0==strncmp(cond,"gets",4))
	{
		strncpy(t.buf,cond,strlen(cond));
		t.len=strlen(t.buf)-1;
		printf("输入的命令是：%s",t.buf);
		send_n(sfd,(char*)&t,4+t.len);
	}else if(0==strncmp(cond,"puts",4))
	{
		strncpy(t.buf,cond,strlen(cond));
		t.len=strlen(t.buf)-1;
		printf("输入的命令是：%s",t.buf);
		send_n(sfd,(char*)&t,4+t.len);
	}else{
		printf("命令错误\n");
		printf("请重新输入：\n");
	}
	return 0;
}
