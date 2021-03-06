#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"

#include <sys/types.h>
#include <pthread.h>

#include "gui_main.h"
#include "uart.h"

GtkWidget *window_main;
GtkWidget *main_check;
GtkWidget *statusbar;
GtkWidget *channel_value_lable[10];
GtkWidget *channel_count_lable[10];
GtkWidget *channel_switch_check[10];
int channelcount[10];

gint tegpipeio;
//int runtime;
//pthread_t handlerthread;

pid_t uart_pid;
int pipe_fd[2];
int pipe_fd_n[2];
char pipe_gui_cache[PIPE_LEN];


void handler_thread(gpointer          data,
                     gint              source, 
                     GdkInputCondition condition){
	int n;
		n = read(pipe_fd_n[0],pipe_gui_cache,PIPE_GUI_LEN);
		if(n < 0){
			printf("read filed..\n");
		}
		if ( n>0 ){
			if(pipe_gui_cache[0] == 'F'){
				char strcache[15];
				int channel = pipe_gui_cache[1]-'0';
				if(channel >= 0 && channel <=9 ){
					gtk_label_set_label(GTK_LABEL (channel_value_lable[channel]),&pipe_gui_cache[3]);
					channelcount[channel]++;
					sprintf(strcache,"%d",channelcount[channel]);
					gtk_label_set_label(GTK_LABEL (channel_count_lable[channel]),strcache);
				}
			}
		}
}

void show_info(GtkWidget *widget, gpointer window){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(window,
	GTK_DIALOG_DESTROY_WITH_PARENT,
	GTK_MESSAGE_INFO,
	GTK_BUTTONS_OK,
	"Download Completed");
	gtk_window_set_title(GTK_WINDOW(dialog), "Information");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
void show_status(gchar *str){
	gtk_statusbar_push(GTK_STATUSBAR(statusbar),gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), ""), str);
}

void maincheckwitch_callback (GtkWidget *widget, gpointer data)
{
    if (gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON(widget))) 
    {
	
	show_status("开始接收");
	if ( pipe(pipe_fd_n) < 0 )
		perror("pipe_n");	
	if ( pipe(pipe_fd) < 0 ){
		perror("pipe");	
	}else{
		uart_pid = fork();
		if(uart_pid == 0){
			close(pipe_fd[1]);
			close(pipe_fd_n[0]);
			uart_thread(&SerPort,pipe_fd[0],pipe_fd_n[1]);	
		}else{
			close(pipe_fd[0]);
			close(pipe_fd_n[1]);
			//res = pthread_create (&handlerthread, NULL, handler_thread, NULL);
			//res = g_thread_create (&handlerthread, NULL, handler_thread, NULL);
			tegpipeio = gdk_input_add(pipe_fd_n[0], GDK_INPUT_READ, handler_thread, NULL);
		}
	}
        /* If control reaches here, the toggle button is down */
    
    } else {
    	show_status("停止接收");
	write(pipe_fd[1],"S",2);
	gdk_input_remove(tegpipeio);
        /* If control reaches here, the toggle button is up */
    }
}



int main_win_init(int argc, char *argv[]){
	GtkWidget *vbox;
	GtkWidget *menubar;
	GtkWidget *uart_port;
	GtkWidget *setupmenu;
	GtkWidget *setup;
	GtkWidget *fqmtable;
	GtkWidget *labletmp;
	gint i;

	//GtkWidget *labletmp;
	//GtkWidget *lable2;	
	gtk_init(&argc, &argv);
	window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window_main), GTK_WIN_POS_CENTER);
	//gtk_window_set_default_size(GTK_WINDOW(window_main), WIN_MAIN_W, WIN_MAIN_H);
	gtk_window_set_title(GTK_WINDOW(window_main), APP_NAME);
	//menubar
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window_main), vbox);
	menubar = gtk_menu_bar_new();  
	setupmenu =  gtk_menu_new();  //分级的菜单
	setup = gtk_menu_item_new_with_label(MENUBAR_SETUP_LABLE);  //这个是为了设置名字等参数
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(setup), setupmenu);

	uart_port = gtk_menu_item_new_with_label(MENUBAR_UART_PORT_SETUP_LABLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), setup);
	gtk_menu_shell_append(GTK_MENU_SHELL(setupmenu), uart_port);
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 3);
	fqmtable = gtk_table_new (TABLE_ROW_N, TABLE_COL_N, FALSE);
	gtk_box_pack_start(GTK_BOX(vbox), fqmtable, FALSE, FALSE, 3);
	gtk_table_set_row_spacings(GTK_TABLE (fqmtable),10);
	gtk_table_set_col_spacings(GTK_TABLE (fqmtable),10);
	//left lable
	for(i = 0; i<TABLE_COL_N ; i++){
		char num[15];
		//char *channel_lable = TABLE_CHANNEL_NAME;
		if(i == 0)
			sprintf(num,"%s",TABLE_CHANNEL_NAME);
		else
			sprintf(num,"%s%d",TABLE_CHANNEL_APP,i);
		labletmp = gtk_label_new (num);
		gtk_table_attach_defaults (GTK_TABLE (fqmtable), labletmp, 0, 1, i, i+1);
	}
	//gtk_widget_show (lable2);
	for(i = 0; i<TABLE_COL_N ; i++){
		char num[15];
		if(i == 0){
			labletmp = gtk_label_new (TABLE_CHANNEL_VALUE_LABLE);
			gtk_table_attach_defaults (GTK_TABLE (fqmtable), labletmp, 1, 2, i, i+1);
		}else{
			sprintf(num,"%d",8898734+i);
			channel_value_lable[i-1] = gtk_label_new (num);
			gtk_table_attach_defaults (GTK_TABLE (fqmtable), channel_value_lable[i-1],
							 1, 2, i, i+1);
		}
	}
	for(i = 0; i<TABLE_COL_N ; i++){
		//char num[15];
		if(i == 0){
			labletmp = gtk_label_new (TABLE_CHANNEL_COUNT_LABLE);
			gtk_table_attach_defaults (GTK_TABLE (fqmtable), labletmp, 2, 3, i, i+1);
		}else{
			//sprintf(num,"%d",i);
			channel_count_lable[i-1] = gtk_label_new ("0");
			gtk_table_attach_defaults (GTK_TABLE (fqmtable), channel_count_lable[i-1],
							 2, 3, i, i+1);
		}
	}
	//toogle and checkbox
	for(i = 0; i<TABLE_COL_N ; i++){
		//char num[15];
		if(i == 0){
			main_check = gtk_check_button_new();
			gtk_table_attach_defaults (GTK_TABLE (fqmtable), main_check, 3, 4, i, i+1);
			g_signal_connect(main_check, "clicked", G_CALLBACK(maincheckwitch_callback), (gpointer)window_main);
		}else{
			channel_switch_check[i-1] = gtk_check_button_new();
			gtk_table_attach_defaults (GTK_TABLE (fqmtable), channel_switch_check[i-1], 3, 4, i, i+1);
			/*sprintf(num,"%d",i);
			channel_count_lable[i-1] = gtk_label_new (num);
			gtk_table_attach_defaults (GTK_TABLE (fqmtable), channel_count_lable[i-1],
							 2, 3, i, i+1);*/
		}
	}
	//statusbar
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE,1);

	//主菜单

	gtk_widget_show_all(window_main);
	g_signal_connect_swapped(G_OBJECT(window_main), "destroy",
				 G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(uart_port, "activate", G_CALLBACK(OnUartPotrSet), (gpointer)window_main);

	gtk_main();

	return 0;
}



