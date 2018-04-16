/*************************************************************************
    > File Name: camera.c
    > Author: dt
    > Mail: dtsino@163.com
    > Created Time: 2018年04月16日 星期一 10时35分41秒
 ************************************************************************/
#include "camera.h"
#include "jpeglib.h"

    FILE *save_yuv_fd;//存放YUV格式图片的文件描述符
    FILE *save_rgb_fd;//存放RGB格式图片的文件描述符
    FILE *save_jpeg_fd;//存放JPEG格式图片的文件描述符
    int fd;//打开摄像头设备的文件描述符
    char *dev_name = NULL;;//摄像头设备的路径
/*出错时打印自定义字符串S、打印错误号以及错误原因，调用exit()函数退出程序*/
void errno_exit(const char *s)
{
    fprintf (stderr, "%s error %d, %s\n", s, errno, strerror (errno));
    exit (EXIT_FAILURE);
}

/*自定义函数xioctl()调用ioctl()函数向摄像头设备发送控制命令*/
int xioctl(int fd, int request, void *arg)
{
    int r;
	/*
	设置视频设备属性通过 ioctl 来进行设置， ioctl 有三个参数，分别是 fd, cmd,和
	parameter,表示设备描述符，控制命令和控制命令参数
	*/
    do r = ioctl (fd, request, arg);
    while (-1 == r && EINTR == errno);
    return r;
}

/*保存p中的yuv图片到yuv.yuv文件中*/
void save_yuv_file(const unsigned char *p, size_t length)
{
    save_yuv_fd = NULL;
    save_yuv_fd = fopen("yuv.yuv","w");
    fputc ('1', stdout);
	/*
		写数据块函数调用的一般形式为： fwrite(buffer,size,count,fp); 
		其中buffer是一个指针,它表示存放输出数据的首地址。 
		size 表示数据块的字节数。
		count 表示要读写的数据块块数。
		fp 表示文件指针。
	*/
    fwrite(p,sizeof(char),length,save_yuv_fd);
    fflush (stdout);
}

/*保存p中的yuv图片到rgb.rgb文件中*/
void save_rgb_file(const unsigned char *p, size_t length)
{
    save_rgb_fd = NULL;
    save_rgb_fd = fopen("rgb.rgb","w");
    fputc ('2', stdout);
	/*
		写数据块函数调用的一般形式为： fwrite(buffer,size,count,fp); 
		其中buffer是一个指针,它表示存放输出数据的首地址。 
		size 表示数据块的字节数。
		count 表示要读写的数据块块数。
		fp 表示文件指针。
	*/
    fwrite(p,sizeof(char),length,save_rgb_fd);
    fflush (stdout);
}

/*保存p中的yuv图片到jpeg.jpeg文件中*/
void save_jpeg_file(const unsigned char *p,size_t length)
{
    save_jpeg_fd = NULL;
    save_jpeg_fd = fopen("jpeg.jpeg","w");
    fputc('3',stdout);
    fwrite(p,sizeof(char),length,save_jpeg_fd);
    fflush(stdout);
}

/*RGB转JPEG函数（参数：RGB内存，图片压缩质量）*/
int write_rgb_to_jpeg_buff(unsigned char *rgb_buffer,int quality)
{
    unsigned long outsize = 0;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];
    int row_stride;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    memset(jpeg_buffer,0,sizeof(*jpeg_buffer));
    jpeg_mem_dest(&cinfo,&jpeg_buffer,&outsize);
    cinfo.image_width = imag_width;
    cinfo.image_height = imag_height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo,imag_quality,TRUE);
    jpeg_start_compress(&cinfo,TRUE);
    row_stride = imag_width * 3;
    while(cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &rgb_buffer[cinfo.next_scanline * row_stride];
        (void)jpeg_write_scanlines(&cinfo,row_pointer,1);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    return (int)outsize;
}

/*YUV转换成RGB32*/
int convert_yuv_to_rgb_pixel(int y,int u,int v)
{
	unsigned int pixel32 = 0;
	unsigned char *pixel = (unsigned char*)&pixel32;
	int r,g,b;
	r = y + (1.370705 * (v - 128));
	g = y - (0.698001 * (v - 128)) - (0.337633 * (u - 128));
	b = y + (1.732446 * (u - 128));
	if(r > 255)
	{
		r = 255;
	}
	if(g > 255)
	{
		g = 255;
	}
	if(b > 255)
	{
		b = 255;
	}
	if(r < 0)
	{
		r = 0;
	}
	if(g < 0)
	{
		g = 0;
	}
	if(b < 0)
	{
		b = 0;
	}
	pixel[0] = r;
	pixel[1] = g;
	pixel[2] = b;
	return pixel32;
}

/*YUV转RGB函数(YUV转RGB32再转RGB24)*/
int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb,unsigned int width,unsigned int height)
{
	unsigned int in,out,length;
	in = 0;
	out = 0;
    length = width * height * 2;
	unsigned int pixel_16;
	unsigned char pixel_24[3];
    unsigned int pixel32;
    int y0,u,y1,v;
    for(in = 0;in < length;in += 4)
    {
        pixel_16 = yuv[in + 3] << 24 | yuv[in + 2] << 16 | yuv[in + 1] << 8 | yuv[in + 0];
        y0 = (pixel_16 & 0x000000ff);
        u = (pixel_16 & 0x0000ff00) >> 8;
        y1 = (pixel_16 & 0x00ff0000) >> 16;
        v = (pixel_16 & 0xff000000) >> 24;
        pixel32 = convert_yuv_to_rgb_pixel(y0,u,v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
        pixel32 = convert_yuv_to_rgb_pixel(y1,u,v);
        pixel_24[0] = (pixel32 & 0x000000ff);
        pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
        pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
        rgb[out++] = pixel_24[0];
        rgb[out++] = pixel_24[1];
        rgb[out++] = pixel_24[2];
    }
    return 0;
}

/*从视频缓冲区的输出队列中取得一个已经准备好一帧视频数据的视频缓冲区，调用转换函数：YUV转RGB，RGB转JPEG*/
int read_frame(void)
{
    struct v4l2_buffer buf;
    CLEAR (buf);
    int ret = 0;
	rgb_buffer_size = imag_width * imag_height * 3;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf)) 
	{
        switch (errno) 
		{
			case EAGAIN:
				return 0;
			case EIO:
				/* Could ignore EIO, see spec. */
				/* fall through */
			default:
				errno_exit ("VIDIOC_DQBUF");
        }
    }
    assert (buf.index < n_buffers);
    printf("yuv_size::%d\r\n",buffers[buf.index].length);
	save_yuv_file(buffers[buf.index].start, buffers[buf.index].length);
    fclose(save_yuv_fd);
	printf("\r\n");
	printf("-------------------save_yuv_file ok!!!-------------\r\n");
	printf("\r\n");
	//clean the rgb_buffer
	memset(rgb_buffer,0,rgb_buffer_size);
	//convert yuv to rgb
	ret = convert_yuv_to_rgb_buffer(buffers[buf.index].start,rgb_buffer,imag_width,imag_height);
    printf("rgb_size:%d\r\n",rgb_buffer_size);
	save_rgb_file(rgb_buffer,rgb_buffer_size);
    fclose(save_rgb_fd);
	printf("\r\n");
	printf("********************save_rgb_file ok!!!**********************\r\n");
	printf("\r\n");
    #if 1
	jpeg_size = write_rgb_to_jpeg_buff(rgb_buffer,imag_quality);
    printf("jpeg_size:%d\r\n",jpeg_size);
	save_jpeg_file(jpeg_buffer,jpeg_size);
	fclose(save_jpeg_fd);
	printf("\r\n");
	printf("~~~~~~~~~~~~~~~~~~~~save_jpeg_fd ok!!!~~~~~~~~~~~~~~~~~~~~~~~\r\n");
	printf("\r\n");
    #endif
	if(ret >= 0)
	{
		ret = xioctl(fd,VIDIOC_QBUF,&buf);
		if( -1 == ret )
		{
			errno_exit ("VIDIOC_QBUF");
		}
		else
		{
			return 1;
		}
	}
}

void mainloop(void)
{
    //unsigned int count;
    //count = 100;
    while(1) 
    {
        //for(;;)
        //{
            fd_set fds;
            struct timeval tv;
            int r;
            FD_ZERO (&fds);
            FD_SET (fd, &fds);
            /* Timeout. */
            tv.tv_sec = 2;
            tv.tv_usec = 0;
            r = select (fd + 1, &fds, NULL, NULL, &tv);
            if (-1 == r) 
            {
                if (EINTR == errno)
                continue;
                errno_exit ("select");
            }
            else if (0 == r) 
            {
                fprintf (stderr, "select timeout\n");
                exit (EXIT_FAILURE);
            }
            else
            {
                if(rgb_buffer == NULL)
                {
                    printf("rgb_buffer == NULL\n");
                    //close the camera
                    break;
                }
                if(read_frame())
                    break;
            }
        //}
    }
}

/*调用xioctl()函数向摄像头设备发送停止采集命令，停止视频采集*/
void stop_capturing(void)
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
        errno_exit ("VIDIOC_STREAMOFF");
}

/*把空的内核视频缓冲区放入视频缓冲区输入队列，并调用xioctl()函数向摄像头设备发送采集命令，启动视频采集*/
void start_capturing(void)
{
    unsigned int i;
    enum v4l2_buf_type type;
    for (i = 0; i < n_buffers; ++i) 
	{
        struct v4l2_buffer buf;
		CLEAR (buf);
		buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory  = V4L2_MEMORY_MMAP;
        buf.index   = i;
        if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))//把一个空的视频缓冲区放到视频缓冲区队列
            errno_exit ("VIDIOC_QBUF");
    }
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //apply for an area for the rgb
    rgb_buffer = (unsigned char *)malloc(imag_width * imag_height * 4);
    //apply for an area for the jpeg
    jpeg_buffer = (unsigned char *)malloc(imag_width * imag_height * 4);
	if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))//设置数据流类型,启动视频采集
        errno_exit ("VIDIOC_STREAMON");
}

/*取消内存映射，释放用户空间视频缓冲区内存buffers*/
void uninit_device(void)
{
    unsigned int i;
    for (i = 0; i < n_buffers; ++i)
	{
		if (-1 == munmap (buffers[i].start, buffers[i].length))
			errno_exit ("munmap");
	}
    free (buffers);
}

/*内存映射，内核buf映射到用户空间视频缓冲区内存buffers*/
void init_mmap(void)
{
	n_buffers = 0;
	buffers = NULL;
    struct v4l2_requestbuffers req;
    CLEAR (req);
    req.count  = 4;//申请缓冲区数量
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req)) 
	{/*执行成功时，函数返回值为 0；V4L2驱动层分配好了视频缓冲区。reg返回实际申请缓冲区数量，*/
        if (EINVAL == errno) 
		{
            fprintf (stderr, "%s does not support memory mapping\n", dev_name);
            exit (EXIT_FAILURE);
        } 
		else 
		{
            errno_exit ("VIDIOC_REQBUFS");
        }
    }
    if (req.count < 2) 
	{
        fprintf (stderr, "Insufficient buffer memory on %s\n",dev_name);
        exit (EXIT_FAILURE);
    }
    buffers = calloc(req.count, sizeof (*buffers));//用户空间申请缓冲区和内核空间缓冲区大小一致
    if (!buffers) 
	{
        fprintf (stderr, "Out of memory\n");
        exit (EXIT_FAILURE);
    }
    for (n_buffers = 0; n_buffers < req.count; ++n_buffers)//n_buffers = req.count = 4 时退出循环体
	{
        struct v4l2_buffer buf;
        CLEAR (buf);
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = n_buffers;
		/*     
			控制命令 VIDIOC_QUERYBUF
			功能： 查询已经分配的 V4L2 的视频缓冲区的相关信息，包括视频缓冲区的使
			用状态、在内核空间的偏移地址、缓冲区长度等。存放到buf结构体中。
		*/
        if (-1 == xioctl (fd, VIDIOC_QUERYBUF, &buf))
            errno_exit ("VIDIOC_QUERYBUF");
        buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start =  (unsigned char *)mmap (NULL /* start anywhere */,
									buf.length,
									PROT_READ | PROT_WRITE /* required */,
									MAP_SHARED /* recommended */,
									fd, buf.m.offset);
        if (MAP_FAILED == buffers[n_buffers].start)//若映射错误，退出
            errno_exit ("mmap");
    }
}

/*初始化视频设备(type：设置为视频采集类型；c：设置采集窗口大小；设置帧格式)，并调用init_mmap()函数进行内存映射(内核空间buf映射到用户空间buffer)*/
void init_device(void)
{
    struct v4l2_capability cap;//V4L2 的能力描述类型 struct v4l2_capability
    struct v4l2_cropcap cropcap;//此结构体定义了窗口默认大小、图片宽高比
    struct v4l2_crop crop;//描述视频采集窗口的type和size
    struct v4l2_format fmt;//设置一帧视频数据的格式（帧格式，top、bottom场，帧高、帧宽）
    unsigned int min;
	/*
			控制命令 VIDIOC_QUERYCAP
		功能： 查询视频设备的功能 ；
		参数说明：参数类型为 V4L2 的能力描述类型 struct v4l2_capability ；
		返回值说明： 执行成功时，函数返回值为 0；函数执行成功后， 
		struct v4l2_capability 结构体变量中的返回当前视频设备所支持的功能；
		例如支持视频捕获功能 V4L2_CAP_VIDEO_CAPTURE、V4L2_CAP_STREAMING 等。
	*/
    if (-1 == xioctl (fd, VIDIOC_QUERYCAP, &cap)) 
	{
        if (EINVAL == errno) 
		{
            fprintf (stderr, "%s is no V4L2 device\n",dev_name);
            exit (EXIT_FAILURE);
        } else 
		{
            errno_exit ("VIDIOC_QUERYCAP");
        }
    }
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) 
	{
        fprintf (stderr, "%s is no video capture device\n",dev_name);
        exit (EXIT_FAILURE);
    }
    /* 在这里选择视频输入、视频标准和调整 */
    CLEAR (cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	/*获得窗口默认大小、图片宽高比等信息放入cropcap中*/
    if (0 == xioctl (fd, VIDIOC_CROPCAP, &cropcap)) 
	{
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; /* 设置窗口大小为默认大小 */
        if (-1 == xioctl (fd, VIDIOC_S_CROP, &crop)) 
		{
            switch (errno) 
			{
				case EINVAL:
					/* Cropping not supported. */
					break;
				default:
					/* Errors ignored. */
					break;
            }
        }
    } 
	else 
	{    
        /* Errors ignored. */
    }
    CLEAR (fmt);
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = imag_width;
    fmt.fmt.pix.height      = imag_height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    if (-1 == xioctl (fd, VIDIOC_S_FMT, &fmt))
        errno_exit ("VIDIOC_S_FMT");
    /* Note VIDIOC_S_FMT may change width and height. */
    /* Buggy driver paranoia. */
    min = fmt.fmt.pix.width * 2;//一个像素占两个字节，计算一行的总字节数
    if (fmt.fmt.pix.bytesperline < min)//设置每行的总字节数为min
        fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;//计算一张图片的总大小（每行总字节数*总行数）
    if (fmt.fmt.pix.sizeimage < min)//设置一张图片的总大小
        fmt.fmt.pix.sizeimage = min;
   	init_mmap ();//初始化内存映射 
}

/*关闭设备描述符fd*/
void close_device(void)
{
    if (-1 == close (fd))
        errno_exit ("close");
    fd = -1;
}

/*非阻塞方式打开视频设备，获得设备描述符fd*/
void open_device(void)
{
    fd = -1;
    dev_name = "/dev/video0";
    struct stat st; //struct stat 结构体保存文件信息
    if (-1 == stat (dev_name, &st)) //按照dev_name路径找到目标设备文件，读取设备文件信息保存到st中
	{
        fprintf (stderr, "Cannot identify '%s': %d, %s\n",dev_name,errno,strerror(errno));
        exit (EXIT_FAILURE);
    }
    if (!S_ISCHR (st.st_mode)) 
	{
        fprintf (stderr, "%s is no device\n", dev_name);
        exit (EXIT_FAILURE);
    }
    fd = open (dev_name,O_RDWR | O_NONBLOCK,0);
    if (-1 == fd) 
	{
        fprintf (stderr, "Cannot open '%s': %d, %s\n",dev_name,errno,strerror(errno));
        exit (EXIT_FAILURE);
    }
}

