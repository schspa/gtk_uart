
target = test
GTK_FLAG = `pkg-config --libs --cflags gtk+-2.0`
$(target):gui_main.h gui_main.c uart.c uart.h gui_portset.h gui_portset.c combopro.h combopro.c package.h
	cc -D_REENTRANT -Wall -o test uart.c main.c gui_main.c gui_portset.c combopro.c $(GTK_FLAG)
.PHONY : clean
clean :
	-rm -rf $(target)


