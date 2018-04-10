/*************************************************************************
	> File Name: mylcd.c
	> Author: lxy
	> Mail: 455488438@qq.com 
	> Created Time: Sun 01 Apr 2018 09:59:52 AM CST
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/mman.h>
#include "mylcd.h"

#define RED		0xFF0000;
#define GREEN	0x00FF00;
#define BLUE	0x0000FF;
#define LCD_SIZE 800*480*4



//屏幕描述符
static int fd_lcd;
//屏幕映射的地址
static int *plcd = NULL;


void error_handle(const char *errmsg)
{
	perror(errmsg);
	exit(EXIT_FAILURE);
}

void lcd_init()
{
	//打开屏幕设备
	fd_lcd = open("/dev/fb0",O_RDWR);
	if(fd_lcd == -1)
		error_handle("open fb0 error");
	
	printf("open fb ok!\n");
	//映射到内存
	plcd = mmap(
			0,LCD_SIZE,
			PROT_READ|PROT_WRITE,
			MAP_SHARED,fd_lcd,0
			);
	if(plcd == MAP_FAILED)
		error_handle("map error");
	
	printf("lcd init success!!\n");
}

void lcd_clear_screen(int color)
{
	int i,j;
	for(i = 0; i < 480; ++i)
	{
		for(j = 0; j < 800; ++j)
		{
			lcd_draw_dot(j,i,color);
		}
	}
}

void lcd_draw_dot(int x,int y,int color)
{
	*(plcd + 800*y + x) = color ;
}

void lcd_free()
{
	munmap(plcd,LCD_SIZE);
	close(fd_lcd);
}

void lcd_draw_bmp(const char *filename,int x,int y)
{
	BMP_INFO info;
	load_bmp(filename,&info);
	char * pbmp = info.data;
	int i,j;
	int color;
	char r,g,b;
	int pad = (info.width*3 % 4) ? (4-info.width*3 % 4):0;
	for(i = 0; i < info.height; ++i)
	{
		for(j = 0; j < info.width; ++j)
		{
			r = *pbmp++;
			g = *pbmp++;
			b = *pbmp++;
			color = b<<16 | g<<8 | r;
			lcd_draw_dot(j+x,info.height-1-i+y,color);
		}
			pbmp += pad;
	}
	free(info.data);
}

void load_bmp(const char *filename,BMP_INFO *info)
{
	//打开BMP
	int fd = open(filename,O_RDONLY);
	if(fd == -1)
		error_handle(filename);
	//读取BMP图的各种属性
	//读文件大小
	lseek(fd,0x2,SEEK_SET);
	read(fd,&info->size,sizeof(int));
	//读文件的宽和高
	lseek(fd,0x12,SEEK_SET);
	read(fd,&info->width,sizeof(int));
	read(fd,&info->height,sizeof(int));
	
	//printf("%d %d %d \n",info->size,info->width,info->height);
	
	//分配空间，用于保存像素数组
	info->data = (char*)malloc(info->size - 54);
	//读像素数据
	lseek(fd,0x36,SEEK_SET);
	read(fd,info->data,info->size-54);
	//printf("load bmp ok!\n");
}


void lcd_draw_ui()
{
	lcd_draw_bmp("./bmp/bg.bmp",0,0);
	lcd_draw_bmp("./bmp/title.bmp",0,0);
	int i;
	for (i = 0; i < 12; ++i)
	{
		lcd_draw_bmp("./bmp/key_release.bmp",10+65*i,47);
	}
	lcd_draw_bmp("./bmp/logo.bmp",229,350);
}
