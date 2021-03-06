#include "gui_portset.h"
#include "gui_main.h"
#include  <errno.h>
ComboPro cp_device,cp_bound,cp_stopbit,cp_numbit;



char *numbit[] = {"5","6","7","8"};
char *stopbit[] = {"1bit","1.5bit","2bit"};
char *bound[] = {"115200","9600"};
char *dev_file[] = {"/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3"};

void port_modify(GtkWidget *widget, gpointer window){
	gchar *tty;
	//gchar *bound;
	int bound;
	//speed_t speed;
	tty = gtk_combo_box_get_active_text(GTK_COMBO_BOX(cp_device.combobox));
	bound = atoi(gtk_combo_box_get_active_text(GTK_COMBO_BOX(cp_bound.combobox)));
	SerPort.dev = (char *)tty;
	switch(bound){
		case 9600:SerPort.speed = B9600;break;
		case 115200:SerPort.speed = B115200;break;
		default:SerPort.speed = B9600;	
	}
	if(SerPort.fd != 0){
		set_options(&SerPort);
		printf("modify options\n");
		show_status(strerror(errno));	
	}else{
		if( open_uart(&SerPort) != 0)
			show_status(strerror(errno));
		else
			show_status("open tty success..");
	}
	gtk_widget_destroy(window);
}
void PortSetGUI(void){
	GtkWidget *windows;
	GtkWidget *vbox;
	GtkWidget *hboxbt;
	GtkWidget *btok;
	GtkWidget *btcancel;
	//printf("PortSetGUI");
	windows = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(windows), GTK_WIN_POS_CENTER);
	//gtk_window_set_default_size(GTK_WINDOW(windows), 320, 240);
	gtk_window_set_title(GTK_WINDOW(windows), LABLE_UART_SETUP);
	vbox = gtk_vbox_new(TRUE, 1);
	gtk_container_add(GTK_CONTAINER(windows), vbox);

	cp_device.name = "设备文件";
	cp_device.combotext = dev_file;
	gtk_combopro_new(&cp_device, 4);

	cp_bound.name = "波特率";
	cp_bound.combotext = bound;	
	gtk_combopro_new(&cp_bound,2);
	
	cp_stopbit.name = "停止位";
	cp_stopbit.combotext = stopbit;
	gtk_combopro_new(&cp_stopbit,3);

	cp_numbit.name = "数据位";
	cp_numbit.combotext = numbit;
	gtk_combopro_new(&cp_numbit,4);

	hboxbt = gtk_hbox_new(TRUE, 0);
	btok =  gtk_button_new_with_label("确定");
	btcancel =  gtk_button_new_with_label("取消");
	gtk_container_add(GTK_CONTAINER(hboxbt), btok);	
	gtk_container_add(GTK_CONTAINER(hboxbt), btcancel);
	
	gtk_container_add(GTK_CONTAINER(vbox), cp_device.hbox);	
	gtk_container_add(GTK_CONTAINER(vbox), cp_bound.hbox);
	gtk_container_add(GTK_CONTAINER(vbox), cp_numbit.hbox);
	gtk_container_add(GTK_CONTAINER(vbox), cp_stopbit.hbox);
	gtk_container_add(GTK_CONTAINER(vbox), hboxbt);

	gtk_widget_show_all(windows);
	//
	g_signal_connect_swapped(btcancel, "clicked", G_CALLBACK(gtk_widget_destroy), windows);  //cancel响应
	g_signal_connect(btok, "clicked", G_CALLBACK(port_modify), windows);  //cancel响应
} 
void OnUartPotrSet(GtkWidget *widget, gpointer label){
	//printf("OnUartPotrInit\n");
	//write(0,"OnUartPotrInit\n",20);
	//show_info(widget,label);
	PortSetGUI();
}
