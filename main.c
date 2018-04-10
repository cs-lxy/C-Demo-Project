/*************************************************************************
	> File Name: main.c
	> Author: lxy
	> Mail: 455488438@qq.com 
	> Created Time: Sun 01 Apr 2018 09:59:52 AM CST
 ************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <strings.h>
#include <pthread.h>
#include <stdbool.h>

#include "myts.h"
#include "mylcd.h"

//程序退出标志，为真则退出 
bool terminate = false;
/*信号处理函数*/
void signal_handle(int signo);
/*线程处理函数*/
void *play_key_sound(void *arg);


int main(int argc,char **argv)
{
	if(argc !=2 )
	{
		fprintf(stderr," Usage: %s <tsName> \n",argv[0]);
		return -1;
	}
	//坐标点
	COORDINATE coord = {0,0};
	//线程ID
	pthread_t tid;
	//信号注册
	signal(SIGINT,signal_handle);

	//1.屏幕初始化
	lcd_init();
	//2.绘制界面
	lcd_draw_ui();
	//3.打开触屏
	ts_open(argv[1]);
	
	while(!terminate)
	{
		ts_press(&coord);
		if(coord.x < 10 || coord.x >= 790 || coord.y < 47 || coord.y > 327)
			continue;
		int index = (coord.x-10)/65;
		lcd_draw_bmp("./bmp/key_press.bmp",10+65*index,47);

		pthread_create(&tid,NULL,play_key_sound,(void*)&index);
		pthread_detach(tid);

		ts_release();
		lcd_draw_bmp("./bmp/key_release.bmp",10+65*index,47);

		bzero(&coord,sizeof(COORDINATE));
	}
	
	//.资源回收
	lcd_free();
	ts_close();
	printf("\n game over! \n");  
	return 0;
}

void signal_handle(int signo)
{
	if(signo == SIGINT)
		terminate = 1;
}

void *play_key_sound(void *arg)
{
	int index = *(int*)arg;
	char cmd[100] = {0};

	sprintf(cmd,"madplay -Q ./mp3/d%d.mp3 ",index+1);
	system("ps | grep 'madplay' | awk '!/grep/{print $1}' | xargs  kill -9 2> /dev/null");
	system(cmd);

	//killall madplay
	//ps | grep 'madplay' | awk '$0!~/grep/{print $1}' | xargs  kill -9

	return NULL;
}