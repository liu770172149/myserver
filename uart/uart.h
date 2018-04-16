#ifndef _UART_H_
#define _UART_H_

#include<stdio.h>      /*标准输入输出定义*/    
#include<stdlib.h>     /*标准函数库定义*/    
#include<unistd.h>     /*Unix 标准函数定义*/    
#include<sys/types.h>     
#include<sys/stat.h>       
#include<fcntl.h>      /*文件控制定义*/    
#include<termios.h>    /*PPSIX 终端控制定义*/    
#include<errno.h>      /*错误号定义*/    
#include<string.h>    


int UART0_Open();    
int UART0_Init(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity); 
int UART0_Send(int fd,int order);
int UART0_Recv(int fd, unsigned char *rcv_buf,int data_len); 
void UART0_Close(int fd);
int ChangeNum(int temp,int hum,int light1,int light2);
int myuart_init();//整个串口初始化,及进入自动采集模式  
int chamge(char arr[50]);//将数据采集给服务器
int loopctl(char *choose, int order);//循环控制变量/


#endif