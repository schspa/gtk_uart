#ifndef __GUI_PORT_SET_H__
#define __GUI_PORT_SET_H__
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "combopro.h"
#include "uart.h"

#define LABLE_UART_SETUP "串口设置"

extern ComboPro cp_device,cp_bound,cp_stopbit,cp_numbit;

void OnUartPotrSet(GtkWidget *widget, gpointer label);
void PortSetGUI(void);

#endif //__GUI_PORT_SET_H__

