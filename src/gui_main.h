#ifndef __GUI_MAIN_H__
#define __GUI_MAIN_H__
#include <gtk/gtk.h>
#include <string.h>  //strcat 
#include "gui_portset.h"
#define WIN_MAIN_H  600
#define WIN_MAIN_W  900
#define APP_NAME "GTK_UART"

//menubar
#define MENUBAR_SETUP_LABLE "设置"
#define MENUBAR_UART_PORT_SETUP_LABLE "串口"


#define TABLE_ROW_N 4
#define TABLE_COL_N 11
#define TABLE_CHANNEL_APP "Channel:"
#define TABLE_CHANNEL_NAME "通道"

#define TABLE_CHANNEL_VALUE_LABLE "数值"
#define TABLE_CHANNEL_COUNT_LABLE "计数"

#define TANLE_MAIN_SWITCH_LABLE "连接"

#define PIPE_GUI_LEN 64


extern GtkWidget *window_main;
extern GtkWidget *statusbar;
extern GtkWidget *channel_value_lable[10];
extern GtkWidget *channel_count_lable[10];

int main_win_init(int argc, char *argv[]); //GUI初始化
void show_status(gchar *str);
#endif //__GUI_MAIN_H__

