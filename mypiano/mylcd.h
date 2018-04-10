#ifndef MYLCD_H
#define MYLCD_H


//BMP图信息结构
typedef struct BMP_INFO
{
	int width;
	int height;
	int size;
	unsigned char *data;
}BMP_INFO;

/*LCD初始化，打开、映射*/
void lcd_init();
/*资源回收：解除映射，关闭LCD*/
void lcd_free();
/*把屏幕显示成指定的颜色*/
void lcd_clear_screen(int color);
/*绘制界面*/
void lcd_draw_ui();
/*错误处理，打印指定的错误信息并退出程序*/
void error_hand(const char*errmsg);

/**
@函数名 	 lcd_draw_dot 
@功能 	 	 在屏幕的指 定位置 画 指定的 颜色 
@参数 x 	 表示纵坐标，即例数
@参数 y 	 表示横坐标，即行数
@参数 color  整型颜色值 
*/
inline void lcd_draw_dot(int x,int y,int color);

/**
@函数名：lcd_draw_bmp
@功能：在屏幕的指定位置画指定的BMP图
@参数：filename	BMP图名
@参数：x,y 		横坐标，纵坐标
*/
void lcd_draw_bmp(const char *filename,int x,int y);
/**
@函数名：load_bmp
@功能：加载BMP相关信息
@参数：filename	BMP图名
@参数：info	BMP图像信息(宽、高、大小、像素数据)
*/
void load_bmp(const char *filename,BMP_INFO *info);

#endif

