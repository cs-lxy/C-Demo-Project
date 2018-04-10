/*************************************************************************
	> File Name: myts.c
	> Author: lxy
	> Mail: 455488438@qq.com 
	> Created Time: Sun 01 Apr 2018 09:59:52 AM CST
 ************************************************************************/
#include "myts.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <strings.h>
#include <stdbool.h>
#include <sys/select.h>
#include <linux/input.h>


static int fd_ts;
extern bool terminate;
void ts_open(const char *tsname)
{
	fd_ts = open(tsname,O_RDONLY);
	if(fd_ts == -1)
		error_handle("open ts error");
}

void ts_close()
{
	close(fd_ts);
}
void ts_press(COORDINATE *coord)
{
	fd_set rfds,tmpfds;
	struct timeval tv;
	int ret;
	FD_ZERO(&rfds);
	FD_SET(fd_ts,&rfds);
	int maxfd = fd_ts;
	struct input_event ev;
	while(!terminate)
	{
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		tmpfds = rfds;
		ret = select(maxfd+1,&tmpfds,NULL,NULL,&tv);
		if(ret == -1 && errno != EINTR)
			error_handle("select press error");
		if(ret == 0)
			continue;
		if(ret > 0)
		{
			bzero(&ev,sizeof(ev));
			read(fd_ts,&ev,sizeof(ev));
			if(ev.type == EV_ABS && ev.code == ABS_X)
				coord->x = ev.value;
			if(ev.type == EV_ABS && ev.code == ABS_Y)
				coord->y = 480-ev.value;
			if(coord->x !=0  && coord->y != 0)
				return;
		}//if
	}//while

}//ts_press

void ts_release()
{
	fd_set rfds,tmpfds;
	struct timeval tv;
	int ret;
	FD_ZERO(&rfds);
	FD_SET(fd_ts,&rfds);
	int maxfd = fd_ts;
	struct input_event ev;
	while(!terminate)
	{
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		tmpfds = rfds;
		ret = select(maxfd+1,&tmpfds,NULL,NULL,&tv);
		if(ret < 0)
			error_handle("select release error");
		if(ret == 0)
			continue;
		if(ret > 0)
		{
			bzero(&ev,sizeof(ev));
			read(fd_ts,&ev,sizeof(ev));
			if(ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0)
				return;
		}//if
	}//while

}//ts_release
