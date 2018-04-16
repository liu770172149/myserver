/*************************************************************************
    > File Name: main.c
    > Author: dt
    > Mail: dtsino@163.com
    > Created Time: 2018年04月16日 星期一 10时35分59秒
 ************************************************************************/

#include "camera.h"
int main()
{
    open_device ();/*非阻塞方式打开视频设备，获得设备描述符fd*/
    init_device ();/*初始化视频设备(type：设置为视频采集类型；c：设置采集窗口大小；设置帧格式),并调用init_mmap()函数进行内存映射(内核空间buf映射到用户空间buffers)*/
    start_capturing ();/*把空的内核视频缓冲区放入视频缓冲区输入队列，并调用xioctl()函数向摄像头设备发送采集命令，启动视频采集*/
    mainloop ();/*读取采集好的视频缓冲区，并转码，YUV->RGB->JPEG*/
    stop_capturing ();/*调用xioctl()函数向摄像头设备发送停止采集命令，停止视频采集*/
    uninit_device ();/*取消内存映射，释放用户空间视频缓冲区内存buffers*/
    close_device ();/*关闭设备描述符fd*/
    exit(EXIT_SUCCESS);
    return 0;
}
