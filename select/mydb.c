
#include "mydb.h"

#define FILEPATH_MAX (80)

int InitDb()//创建数据库
{
	/*db name start*/
    char *pPath = NULL;
    char dbName[] = "/mysqlite.db";
    pPath = (char *) malloc (FILEPATH_MAX + 10);
    if (NULL == pPath)
    {
        return DB_ERR;
    }
    memset(pPath, 0, FILEPATH_MAX + 10);
    getcwd(pPath, FILEPATH_MAX);
    printf("%s %d\n",pPath,strlen(pPath));
    memcpy(pPath + strlen(pPath), dbName, strlen(dbName));
    printf("%s %d\n",pPath,strlen(pPath));
    /*db name end*/
	if (DB_ERROR_OK != InitConnection())
    {
        printf("invoke InitConnection function error!\n");
        return DB_ERR;
    }
    //open or create db
    if (DB_ERROR_OK != OpenConnection(pPath))
    {
        printf("invoke OpenConnection function error!\n");
        return DB_ERR;
    }

	CreateTable();
	
    return DB_OK;
}
int CreateTable()//创建管理员表，普通用户表，粮仓环境参数
{	
	//管理员,id作为主键，自增
	char admintable[]="create table admin (id integer primary key autoincrement,"
							" admin_name text not null,"
							" admin_password text not null )";
	if(DB_ERROR_OK != ExecSQL(admintable))
	{
		perror("creat admin table error");
		return DB_ERR;
	}
	//用户
	char usertable[]="create table user (id integer primary key autoincrement,"
                                                    "user_name text not null,"
                                                    "user_password text not null)";
	if(DB_ERROR_OK != ExecSQL(usertable))
	{
		perror("creat user table error");
		return DB_ERR;
	}
	//粮仓环境参数
	char Environment[]="create table environment (id integer primary key autoincrement,"
                                                    "temperature text not null,"
						    "humidity text not null,"
						    "illumination text not null,"
                                                    "nowtime text not null)";
	if (DB_ERROR_OK != ExecSQL(Environment))
    	{
		perror("create Environment table error\n");
        	return DB_ERR;
    	}

	printf("create table ok!!!\n"); 
	//创建一个超级管理员账户
	char AdminInsert[]="insert into admin (id, admin_name, admin_password) values (1,'admin','admin')";
	if (DB_ERROR_OK != ExecSQL(AdminInsert))
    	{
        	printf("insert table sql error\n");
		return DB_ERR;
    	}
		
	char UserInsert[]="insert into user (id, user_name, user_password) values (1,'user','user')";
	if (DB_ERROR_OK != ExecSQL(UserInsert))
    	{
        	printf("insert table sql error\n");
		return DB_ERR;
    	}
		
	InitBbash("20","15","28");//将各个环境变量加入表中
	return DB_OK;
}
int InitBbash(char *temp,char *humidity,char *illumination)//插入环境变量
{
	char EnvironmentInsert[200]="insert into environment (temperature, humidity ,illumination,nowtime) values ('";
	//char *temp = "20'c";
	//char *humidity = "15";
	//char *illumination = "28照度";
	//时间库函数，调出当前时间
	time_t timer;
    	struct tm *tblock;
    	timer = time(NULL);
    	tblock = localtime(&timer);
	char *nowtime = asctime(tblock);
	strcat(EnvironmentInsert,temp);
	strcat(EnvironmentInsert,"','");
	strcat(EnvironmentInsert,humidity);
	strcat(EnvironmentInsert,"','");
	strcat(EnvironmentInsert,illumination);
	strcat(EnvironmentInsert,"','");
	strcat(EnvironmentInsert,nowtime);
	strcat(EnvironmentInsert,"')");
	printf("%s\n",EnvironmentInsert);
	if (DB_ERROR_OK != ExecSQL(EnvironmentInsert))
    	{
        	printf("insert table sql error\n");
		return DB_ERR;
    	}
	return DB_OK;
	printf("环境变量插入成功!\n");
}
int zhuce(char *buff[])//user 注册
{
	if(DB_OK ==findtable(buff[1]))
	{
		printf("用户名已经存在\n");
		return DB_ERR;
	}
	printf("zhuce jinru\n");
	printf("%s\t %s\t\n",buff[1],buff[2]);
	char UserInsert[200]="insert into user (user_name, user_password) values ('";
	strcat(UserInsert,buff[1]);
	strcat(UserInsert,"','");
	strcat(UserInsert,buff[2]);
	strcat(UserInsert,"')");
	if (DB_ERROR_OK != ExecSQL(UserInsert))
    	{
        	printf("注册失败!\n");
		return DB_ERR;
    	}
	printf("注册成功!\n");
	return DB_OK;
}

int Login(int socketID,char *buffer[])//admin and user login
{
	if(0 ==strcmp("admin",buffer[1]))
	{
		char Admin[100] = "select * from admin where admin_password = '";
		strcat(Admin,buffer[2]);
		strcat(Admin,"'");
		char** ppTable = NULL;
    		int iRow = 0;
    		int iColumn = 0;
		if (DB_ERROR_OK != GetTable(Admin, &ppTable, &iRow, &iColumn))
    		{
			return DB_ERR;
    		}
		if (NULL != ppTable)
    		{
			if(0 == strcmp(buffer[1],ppTable[4]) && 0 == strcmp(buffer[2],ppTable[5]))
			{
				printf("管理员登录成功!\n");
				return DB_OK;		
			}
			printf("管理员帐号密码错误!\n");
			return DB_ERR;
		}
	}
	else
	{
		char User[100] = "select * from user where user_name = '";
		strcat(User,buffer[1]);
		strcat(User,"'");
		strcat(User,"and user_password = '");
		strcat(User,buffer[2]);
		strcat(User,"'");
		char** ppTable = NULL;
    		int iRow = 0;
    		int iColumn = 0;
		if (DB_ERROR_OK != GetTable(User, &ppTable, &iRow, &iColumn))
    		{
			return DB_ERR;
    		}
		if (NULL != ppTable)
    		{
			if(0 == strcmp(buffer[1],ppTable[4]) && 0 == strcmp(buffer[2],ppTable[5]))
			{
				printf("用户登录成功!\n");
				return DB_OK;		
			}
			printf("用户帐号密码错误!\n");
			return DB_ERR;
		}
	}
}

int findtable(const char *buff)//查看是否该姓名已近注册
{
	if(NULL == buff)
	{
		printf("find error\n");
	}
	char findName[100]="select * from user where user_name = '";
	strcat(findName,buff);
	strcat(findName,"'");
	char** ppTable = NULL;
    	int iRow = 0;
    	int iColumn = 0;
	if (DB_ERROR_OK != GetTable(findName, &ppTable, &iRow, &iColumn))
    	{
		return DB_ERR;
    	}
	if (NULL != ppTable)
    	{
		if(0 == strcmp(buff,ppTable[4]))
		{
			return DB_OK;		
		}
		return DB_ERR;
	}
    	ReleaseTableResult(ppTable);
	return DB_OK;
}


int findbbash(char *bufff)
{
	//temperature,humidity,illumination
	char sqlSelect[] = "select temperature,humidity,illumination from environment";
		char** ppTable = NULL;
		//char buff[1024]="";
		int iRow = 0;//hang
		int iColumn = 0;
		if (DB_ERROR_OK != GetTable(sqlSelect, &ppTable, &iRow, &iColumn))
		{
			return DB_ERR;
			printf("select sql error\n");
		}
         printf("select sql row %d column %d\n", iRow, iColumn);
		if(NULL != ppTable)
		{
			//printf("jin ru pptable\n");
			int i = iRow;
			for (; i < iRow+1; i++)
			{
				int j = 0;
				for (;j<iColumn; j++)
				{
					if (NULL != ppTable[i * iColumn + j])
					{
						printf("%s    ",ppTable[i * iColumn + j]);
						strcat(bufff,ppTable[i* iColumn + j]);
						strcat(bufff,"#");
					}
					
				}
				printf("\n");
			}
			/*
			int k=0;								
			for(k=0;k<iRow;k++)
			{									
				int m=0;
				for(m=1;m<iColumn;m++)
				{					                
					if (NULL != ppTable[k * iColumn + m])
                    {                                          
						printf("%s\n",ppTable[k * iColumn + m]);
						strcat(bufff,ppTable[k* iColumn+m]);
						strcat(bufff,"#");
							//printf("%s  ",bufff);
					}
				}
		    }
			*/
        }
        // printf("%s\n",bufff);
		 
		 ReleaseTableResult(ppTable);
	return DB_OK;
}









