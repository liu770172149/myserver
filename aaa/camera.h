/*************************************************************************
    > File Name: camera.h
    > Author: dt
    > Mail: dtsino@163.com
    > Created Time: 2018年04月16日 星期一 10时35分48秒
 ************************************************************************/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <linux/fb.h>
#include <setjmp.h>
#define CLEAR(x) memset(&(x),0,sizeof(x))
#define imag_width 640//图片宽度
#define imag_height 480//图片高度
#define imag_quality 95//压缩图片质量比

/*定义用户空间视频缓冲区buffer结构体，有起始地址start、地址长度length*/
typedef struct buffer
{
    unsigned char *start;
    size_t length;
}buffer_user;

buffer_user *buffers;/*定义用户空间buffers*/
unsigned int n_buffers;//用户空间buffers的数目
unsigned char *rgb_buffer;//存放RGB图像的数组，发送给LCD
unsigned char *jpeg_buffer;//存放JPEG图像的数组
unsigned int rgb_buffer_size;//存放RGB图像的数组大小
unsigned int jpeg_size;//存放JPEG图像的数组大小

/*出错时打印自定义字符串S、打印错误号以及错误原因，调用exit()函数退出程序*/
void errno_exit(const char *s);

/*自定义函数xioctl()调用ioctl()函数向摄像头设备发送控制命令*/
int xioctl(int fd,int request,void *arg);

/*非阻塞方式打开视频设备，获得设备描述符fd*/
void open_device(void);

/*初始化视频设备(type：设置为视频采集类型；c：设置采集窗口大小；设置帧格式)，并调用init_mmap()函数进行内存映射(内核空间buf映射到用户空间buffer)*/
void init_device(void);

/*把空的内核视频缓冲区放入视频缓冲区输入队列，并调用xioctl()函数向摄像头设备发送采集命令，启动视频采集*/
void start_capturing(void);

/*select循环监控视频缓冲区，调用read_fream函数读数据，调用转换函数：YUV转RGB，RGB转JPEG*/
void mainloop(void);

/*调用xioctl()函数向摄像头设备发送停止采集命令，停止视频采集*/
void stop_capture(void);

/*取消内存映射，释放用户空间视频缓冲区内存buffers*/
void uninit_device(void);

/*关闭设备描述符fd*/
void close_device(void);

#endif



