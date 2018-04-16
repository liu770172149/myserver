#include "select.h"

int fenge(char *buff,const char *cha,char *pp[])//分割字符串
{
	if((NULL==buff)&&(pp==NULL))
	{
        printf("为空，跳出\n");
        perror("is NULL");
		return -2;
	}
	int j=0;
	char *tmp=strtok(buff,cha);//分割字符串
	while(tmp)
	{
		pp[j]=tmp;
		j++;
		tmp=strtok(NULL,"@");
	}
	return -3;
}
int threadfun(void *arg)
{
    int a=1;
    pthread_exit(&a);
    
}

int mysocket()
{
    pthread_t th;
	int j=0;
	int socketID = 0;
	int newID = 0;
	int ret = 0;
	int addrLength = 0;
	char buf[SIZE] = "";
	struct sockaddr_in addr;
	fd_set readFds;
	fd_set tmpFds;
	int maxFd = 0;
	char *p[50]={""};
	//socket
	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0)
	{
		perror("socket error");
		return SEL_ERR;
	}
	//bind
	addrLength = sizeof(addr);
	memset(&addr, 0, addrLength);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (0 > bind(socketID, (struct sockaddr *)&addr, addrLength))
	{
		perror("bind error");
		close(socketID);
		return SEL_ERR;
	}
	//listen
	listen(socketID, 10);
	//readfds, maxfd, tmpfds
	FD_ZERO(&readFds);
	FD_SET(socketID, &readFds);
	maxFd = socketID;
	tmpFds = readFds;//每次承接总fds表的tmp变量
	while(1)
	{
		readFds = tmpFds;
		printf("socket/bind/listen ok, and call select \r\n");
		ret = select(maxFd + 1, &readFds, NULL, NULL, NULL);//查看多路复用的可读性
		if (ret < 0)
		{
			perror("select error");
			break;
		}
		else if ( 0 == ret )
		{
			printf("select time out \r\n");
			break;
		}
		
		int i = 0;
		for (i = 0; i <= maxFd; i++)
		{
			//判断 哪个文件可读
			if (i == socketID)
			{
				if (FD_ISSET(socketID, &readFds))//查看该socketid是否在readfds内部
				{
					newID = accept(socketID, (struct sockaddr *)&addr, &addrLength);
					if (newID < 0)
					{
						perror("accept error");
						break;
					}
					FD_SET(newID, &tmpFds);
					if (maxFd < newID)
						maxFd = newID;
					printf("new client connect ok, port=%d\r\n", ntohs(addr.sin_port));
				}
			}//if 		
			else if (FD_ISSET(i, &readFds))
			{
				//recv -- 0
				ret = recv(i, buf, SIZE-1, 0);
				printf("recv %d : %s\r\n", i, buf);
				if (ret > 0)
				{
					int ret1 = fenge(buf,"@",p);
					printf("%s\n",p[0]);
					if(ret1 == -3)
					{
						if(*p[0] == 'A')//当前为注册
						{
							printf("zhuce jinru\n");
							//int zhucu(int id,char *buff[])//user 注册函数原型
							int remp = zhuce(p);
							if(remp == DB_ERR)
							{
								printf("注册失败!\r\n");
								char *buff="ERR";
								//发送数据
								int ret11 = send(i, buff, strlen(buff), 0);
								if (ret11 >-1)
								{
									printf("注册失败!\r\n");
								}
							}
							else if (remp == DB_OK)
							{
								printf("注册成功!\r\n");
								char *buff="OK";
								//发送数据
								int ret11 = send(i, buff, strlen(buff), 0);
								if (ret11 > -1)
								{
									printf("注册成功!\r\n");
								}
							}
						}
						else if(*p[0] == 'B')//当前为登录
						{
							// int Login(int socketID,char *buffer[])//用户登录
							int tmp1 = Login(i,p);
							if(tmp1 == DB_ERR)
							{
								char *buff="ERR";
								//发送数据
								int ret22 = send(i, buff, strlen(buff), 0);
								if (ret22 > -1)
								{
									printf("登录失败!\r\n");
								}
							}
							else if (tmp1 == DB_OK)
							{
								char *buff="OK";
								//发送数据
								int ret22 = send(i, buff, strlen(buff), 0);
								if (ret22 > -1)
								{
									printf("登录成功!\r\n");
								}
							}
						}
						else if(*p[0] == 'C')//当前为查询
						{
								printf("进入 C 查询部奏，正在查询。。。。。。\n");
								char bufff[1024]="";
								int ret33=findbbash(bufff);
                                				printf("发送的环境变量为： %s\n",bufff);
								
								if(ret33==DB_ERR)
								{
									printf("查询失败\n");								
								}
								else 
								{
									printf("查询成功，等待发送中....\n");
									int ret333 = send(i, bufff, strlen(bufff), 0);
									if (ret333 > -1)
									{
										printf("查询环境变量且发送成功!\r\n");
									}
								}
						//	}
						}
                        else if(*p[0] == 'D')//当前为传输图片
                        {
                            int ret4=pthread_create(&th,NULL,threadfun,NULL);
                            if(ret4 !=0)
                            {
                                printf("pthread cread error\n");
                                return SEL_ERR;
                            }
                            if(ret4 == 0)
                            {
                                printf("创建线程成功\n");
                            }
                            printf("发送完成，跳出线程、\n");
                        	int *b;
                        	pthread_join(th,(void **)&b);
                        	printf("传输线程结束，状态为%d\n",*b);
                        }
						//printf("recv %d : %s\r\n", i, buf);
					}
				}
				else if ( 0 == ret)
				{
					FD_CLR(i, &tmpFds);
					close(i);				
				}
				else 
				{
					perror("recv error\r\n");
				}
			}//else if 			
		}//for 
	}//while
	//close
	for(j=0;j<maxFd;j++)
	{
		close(j);
	}
	return SEL_OK;
}



