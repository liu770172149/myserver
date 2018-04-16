#ifndef _MYDB_H_
#define _MYDB_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "sqlite3.h"
#include "sqliteinterface.h"

enum DB_ERR_TYPE
{
	DB_ERR = -1,
	DB_OK 
};
/*对外部的接口为：
InitDb，创建数据库
zhucu, 注册普通用户
Login, 管理员和用户的登录
*/
int InitDb();//创建数据库并初始化
int CreateTable();//创建管理员表，普通用户表，粮仓环境参数
int InitBbash(char *temp,char *humidity,char *illumination);//插入环境变量
int zhuce(char *buff[]);//user zhucu
int Login(int socketID,char *buffer[]);//admin and user login
int findtable(const char *buff);//查看是否该姓名已近注册
int findbbash(char *buff);//查询环境变量


#endif //_MYDB_H_
