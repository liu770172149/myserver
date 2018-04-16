#ifndef _SELECT_H_
#define _SELECT_H_

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "mydb.h"

#define IP "192.168.0.113"
#define PORT 10086
#define SIZE 1024

enum SEL_ERR_TYPE
{
	SEL_ERR = -1,
	SEL_OK
};
int fenge(char *buff,const char *cha,char *pp[]);
int mysocket();//开启服务器

#endif //_MYDB_H_
