#include "combopro.h"


void gtk_combopro_new(ComboPro *combopro,int n){
	int i;
	GtkWidget *lable;
	//printf("%s:%d\r\n",combopro->name,n);
	combopro->combobox = gtk_combo_box_new_text();
	for(i=0 ; i<n ; i++){
		if(combopro->combotext[i]!=NULL)
		gtk_combo_box_append_text(GTK_COMBO_BOX(combopro->combobox), 
						combopro->combotext[i]);
	}
	lable = gtk_label_new(combopro->name);
	combopro->hbox = gtk_hbox_new(TRUE, 1);
	gtk_box_pack_start(GTK_BOX(combopro->hbox), lable, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(combopro->hbox), combopro->combobox, TRUE, TRUE, 0);	
	
}
