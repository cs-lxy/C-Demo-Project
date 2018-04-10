#ifndef MYTS_H
#define MYTS_H

//坐标结构体
typedef struct  COORDINATE
{
	int x;
	int y;
}COORDINATE;

//错误处理
extern void error_handle(const char *errmsg);
/*打开指定的触屏设备*/
void ts_open(const char *tsname);
void ts_close();
/*手指按下，以参数的形式返回x,y坐标*/
void ts_press(COORDINATE *coord);
void ts_release();

#endif
