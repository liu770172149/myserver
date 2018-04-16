//串口相关的头文件    
#include "uart.h"
     
#define FALSE  -1    
#define TRUE   0 

  
#define SBUF_SIZE 36
#define OBUF_SIZE 36
#define TMPE_LIMIT  25
#define LIGHT_LIMIT  280
#define HUM_LIMIT  25

//unsigned char sbuf[SBUF_SIZE]={0};   //

unsigned char obuf[OBUF_SIZE]={0};   //温度、湿度、光照存放数组
int fd; 



int myuart_init()//整个串口初始化,及进入自动采集模式   //记得在main函数内部结尾添加close(fd)
{    
                               //文件描述符    
		int err;                           //返回调用函数的状态    
		                            
		int i;    
		             
		unsigned char send_buf[36]={0};	
		fd = UART0_Open(); //打开串口，返回文件描述符    
		//设置串口数据帧格式 
		printf("打开串口成功并返回main函数\n");
		err ==UART0_Init(fd,115200,0,8,1,'N'); 
		printf("串口初始化成功，并返回main函数\n");
		if (FALSE == err || FALSE == fd)
			return 0;
		printf("判断完成，无错误\n");
	//	if(*argc=="hand")    
	//	{   
	//		scanf("%d\n",&order);
	//		UART0_Send(fd,order);//控制命令	
	//	}
		printf("是否自动判断完成，无错误\n");
		
		//close(fd);
		return 0;
	 
}
/*******************************************************************  
* 名称：                  UART0_Open  
* 功能：                打开串口并返回串口设备文件描述  
* 入口参数：        fd    :文件描述符     port :串口号(ttyS0,ttyS1,ttyS2)  
* 出口参数：        正确返回为1，错误返回为0  
*******************************************************************/    
int UART0_Open()    
{    
    char *uart3 = "/dev/ttyUSB0";   
    int fd = open( uart3, O_RDWR|O_NOCTTY|O_NDELAY);    
    if (FALSE == fd)    
    {    
        perror("打开设备文件失败\n");    
        return(FALSE);    
    }    
    //恢复串口为阻塞状态                                   
    if(fcntl(fd, F_SETFL, 0) < 0)    
    {    
        printf("fcntl failed!\n");    
        return(FALSE);    
    }         
    else    
    {    
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));    
    }    
    //测试是否为终端设备        
    if(0 == isatty(STDIN_FILENO))    
    {    
        printf("standard input is not a terminal device\n");    
        return(FALSE);    
    }    
    else    
    {    
        printf("isatty success!\n");    
    }                  
    printf("fd->open=%d\n",fd);    
    return fd;    
}    
/*******************************************************************  
* 名称：                UART0_Close  
* 功能：                关闭串口并返回串口设备文件描述  
* 入口参数：        fd    :文件描述符     port :串口号(ttyS0,ttyS1,ttyS2)  
* 出口参数：        void  
*******************************************************************/    
     
void UART0_Close(int fd)    
{    
    close(fd);    
}    
     
/*******************************************************************  
* 名称：                UART0_Init()    
* 功能：                设置串口数据位，停止位和效验位  
* 入口参数：        fd        串口文件描述符  
*                              speed     串口速度  
*                              flow_ctrl   数据流控制  
*                           databits   数据位   取值为 7 或者8  
*                           stopbits   停止位   取值为 1 或者2  
*                           parity     效验类型 取值为N,E,O,,S  
*出口参数：          正确返回为1，错误返回为0  
*******************************************************************/    
int UART0_Init(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)    
{    
       
    int   i;    
    int   status;    
    int   speed_arr[] = { B115200, B19200, B9600, B4800, B2400, B1200, B300};    
    int   name_arr[] = {115200,  19200,  9600,  4800,  2400,  1200,  300};    
             
    struct termios options;    
	printf("进入串口初始化成功\n");
    /*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.  
    */    
    if( tcgetattr( fd,&options)  !=  0)    
    {    
        perror("SetupSerial 1");        
        return(FALSE);     
    }    
      
    //设置串口输入波特率和输出波特率    
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)    
    {    
        if  (speed == name_arr[i])    
        {                 
            cfsetispeed(&options, speed_arr[i]);     
            cfsetospeed(&options, speed_arr[i]);      
        }    
    }         
       
    //修改控制模式，保证程序不会占用串口    
    options.c_cflag |= CLOCAL;    
    //修改控制模式，使得能够从串口中读取输入数据    
    options.c_cflag |= CREAD;    
      
    //设置数据流控制    
    switch(flow_ctrl)    
    {    
          
        case 0 ://不使用流控制    
              options.c_cflag &= ~CRTSCTS;    
              break;       
          
        case 1 ://使用硬件流控制    
              options.c_cflag |= CRTSCTS;    
              break;    
        case 2 ://使用软件流控制    
              options.c_cflag |= IXON | IXOFF | IXANY;    
              break;    
    }    
    //设置数据位    
    //屏蔽其他标志位    
    options.c_cflag &= ~CSIZE;    
    switch (databits)    
    {      
        case 5    :    
                     options.c_cflag |= CS5;    
                     break;    
        case 6    :    
                     options.c_cflag |= CS6;    
                     break;    
        case 7    :        
                 options.c_cflag |= CS7;    
                 break;    
        case 8:        
                 options.c_cflag |= CS8;    
                 break;      
        default:       
                 fprintf(stderr,"Unsupported data size\n");    
                 return (FALSE);     
    }    
    //设置校验位    
    switch (parity)    
    {      
        case 'n':    
        case 'N': //无奇偶校验位。    
                 options.c_cflag &= ~PARENB;     
                 options.c_iflag &= ~INPCK;        
                 break;     
        case 'o':      
        case 'O'://设置为奇校验        
                 options.c_cflag |= (PARODD | PARENB);     
                 options.c_iflag |= INPCK;                 
                 break;     
        case 'e':     
        case 'E'://设置为偶校验      
                 options.c_cflag |= PARENB;           
                 options.c_cflag &= ~PARODD;           
                 options.c_iflag |= INPCK;          
                 break;    
        case 's':    
        case 'S': //设置为空格     
                 options.c_cflag &= ~PARENB;    
                 options.c_cflag &= ~CSTOPB;    
                 break;     
        default:      
                 fprintf(stderr,"Unsupported parity\n");        
                 return (FALSE);     
    }     
    // 设置停止位     
    switch (stopbits)    
    {      
        case 1:       
                 options.c_cflag &= ~CSTOPB; break;     
        case 2:       
                 options.c_cflag |= CSTOPB; break;    
        default:       
                       fprintf(stderr,"Unsupported stop bits\n");     
                       return (FALSE);    
    }    
       
    //修改输出模式，原始数据输出    
    options.c_oflag &= ~OPOST;    
      
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);    
    //options.c_lflag &= ~(ISIG | ICANON);    
       
    //设置等待时间和最小接收字符    
    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */      
    options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */    
       
    //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读    
    tcflush(fd,TCIFLUSH);    
       
    //激活配置 (将修改后的termios数据设置到串口中）    
    if (tcsetattr(fd,TCSANOW,&options) != 0)      
    {    
        perror("com set error!\n");      
        return (FALSE);     
    }    
	printf("串口初始化成功，即将返回main函数\n");
    return (TRUE);     
}    
/*******************************************************************  
* 名称：                  UART0_Recv  
* 功能：                接收串口数据  
* 入口参数：        fd                  :文件描述符      
*                              rcv_buf     :接收串口中数据存入rcv_buf缓冲区中  
*                              data_len    :一帧数据的长度  
* 出口参数：        正确返回为1，错误返回为0  
*******************************************************************/    
int UART0_Recv(int fd, unsigned char *rcv_buf,int data_len)    
{    
	int len; 
	int i = 0;
	int fs_sel;
	fd_set fs_read;
	struct timeval time;
	FD_ZERO(&fs_read);
	FD_SET(fd,&fs_read);
	time.tv_sec = 10;
	time.tv_usec = 0;
	fs_sel = select(fd+1,&fs_read,NULL,NULL,&time);
 
	len = read(fd,rcv_buf,data_len);
	printf("I am right!(version1.2) len = %d \n",len); 
	return len;  
}   
int ChangeNum(int temp,int hum,int light1,int light2){
	light1 = light1+light2*256;
	//阈值判断
			if(temp > TMPE_LIMIT)
			{
				UART0_Send(fd,4); //m0:fan_on
				//printf("m0:fan_on,temp=%d,i=%d\n",tmp_tmpe,i);
			}
			else
			{
				UART0_Send(fd,8);//m0:fan_off 
				//printf("m0:fan_off,temp=%d,i=%d\n",tmp_tmpe,i);

			}
			if(light1 < LIGHT_LIMIT)
			{
				UART0_Send(fd,0); //"m0:led on  SEVEN_ON 
				UART0_Send(fd,9);
				//printf("m0:led on,light=%d,i=%d\n",tmp_light,i);
			}
			else
			{
				UART0_Send(fd,1); //"m0:led off  SEVEN_Off 
				UART0_Send(fd,10);
				//printf("m0:led off,light=%d,i=%d\n",tmp_light,i);

			}
			if(hum > HUM_LIMIT)
			{
				UART0_Send(fd,2); //"m0:buzzer on
				//printf("m0:buzzer on,hum=%d,i=%d\n",tmp_hum,i);
			}
			else
			{
				UART0_Send(fd,3);//BUZZER_OFF
				//printf("m0:buzzer off,hum=%d,i=%d\n",tmp_hum,i);

			}
		
	snprintf(sbuf,SBUF_SIZE,"t:%dh:%dl:%d",temp,hum,light1);
	printf("%s\r\n",sbuf);
	
	return 0;
} 
/********************************************************************  
* 名称：                  UART0_Send  
* 功能：                发送数据  
* 入口参数：        fd                  :文件描述符      
*                              send_buf    :存放串口发送数据  
*                              data_len    :一帧数据的个数  
* 出口参数：        正确返回为1，错误返回为0  
*******************************************************************/    
       
int UART0_Send(int fd,int order) 
{   
		sleep(1);
		char obuf0[36]={0xdd,0x01,0x24,0x00,0x00 ,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00  };//开灯
		char obuf1[36]={0xdd,0x01,0x24,0x00,0x01 ,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//关灯
		char obuf2[36]={0xdd,0x01,0x24,0x00,0x02,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//开蜂鸣器
		char obuf3[36]={0xdd,0x01,0x24,0x00,0x03,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//关蜂鸣器
		unsigned char obuf4[36]={0xdd,0x01,0x24,0x00,0x04,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//开风扇
		char obuf5[36]={0xdd,0x01,0x24,0x00,0x05,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//风扇低速
		char obuf6[36]={0xdd,0x01,0x24,0x00,0x06,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//风扇中速
		char obuf7[36]={0xdd,0x01,0x24,0x00,0x07,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//风扇高速
		char obuf8[36]={0xdd,0x01,0x24,0x00,0x08,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//关风扇
		char obuf9[36]={0xdd,0x01,0x24,0x00,0x09,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//开数码管
		char obuf10[36]={0xdd,0x01,0x24,0x00,0x0a,0x00,0x00, 0x00,0x00 ,0x00,0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00, 0x00,0x00, 0x00,0x00 ,0x00,0x00 };//关数码管
		switch(order)
		{
			case 0:{
						write(fd,obuf0,36);			
				   }
				   break;
			case 1:{
						write(fd,obuf1,36);			
				   }
				   break;
			case 2:{
						write(fd,obuf2,36);			
				   }
				   break;
			case 3:{
						write(fd,obuf3,36);			
				   }
				   break;
			case 4:{
						write(fd,obuf4,36);			
				   }
				   break;
			case 5:{
						write(fd,obuf5,36);			
				   }
				   break;
			case 6:{
						write(fd,obuf6,36);			
				   }
				   break;
			case 7:{
						write(fd,obuf7,36);			
				   }
				   break;
			case 8:{
						write(fd,obuf8,36);			
				   }
				   break;
			case 9:{
						write(fd,obuf9,36);			
				   }
				   break;
			case 10:{
						write(fd,obuf10,36);
					}
					break;
			default :
					break;
		}
	return 0;
}

int chamge(char arr[50])//将数据采集给服务器
{
	arr[0]=obuf[0];
	arr[1]=obuf[1];
	arr[2]=obuf[2];
	if(arr[0]=='0'&&arr[1]=='0'&&arr[2]=='0')
	{
		printf("采集停止\n");
		return -1;
	}
	
	return 0;
}

int loopctl(char *choose, int order)//循环控制变量/
{
	unsigned char rcv_buf[36]={0};
	int len;
	while (1) //循环读取数据    
        {      
			printf("进入while1 循环\n");
			if(*choose=="hand")    
			{   
				scanf("%d\n",&order);
				UART0_Send(fd,order);//控制命令	
			}
			len = UART0_Recv(fd, rcv_buf,36); 
			printf("len = %d\n",len);
			if(0xbb == rcv_buf[0])
			{
				
				printf("%x \n",rcv_buf[0]);
				read(fd,rcv_buf,35);
				for(i = 0;i<36;i++)
				{
					printf("%x ",rcv_buf[i]);
				}
				printf("\r\n");
				//ChangeNum(rcv_buf[5],rcv_buf[7],rcv_buf[20],rcv_buf[21]);//数据转化到全局SBUF中
				obuf[0]=rcv_buf[5];
				obuf[1]=rcv_buf[7];
				obuf[2]=rcv_buf[20];
                printf("温度，湿度，光照为%d\t %d\t %d\n",obuf[0],obuf[1],obuf[2]);
			}
		}
		return 0;
}