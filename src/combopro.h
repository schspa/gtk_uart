#ifndef __COMBOPRO_H__
#define __COMBOPRO_H__
#include <gtk/gtk.h>
typedef struct{
	char *name;
	char **combotext;
	GtkWidget *combobox;
	GtkWidget *hbox;
}ComboPro; 

void gtk_combopro_new(ComboPro *combopro,int n);

#endif //__COMBOPRO_H__

