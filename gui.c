#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "gui.h"

static void on_button_clicked(GtkWidget *widget, gpointer data)
{
    g_print("Button %s clicked\n", (char *)data);
}

void mainGUI(int argc, char *argv[]){
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *vbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MartMart Grocery");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span font='20' weight='bold'>MartMart Grocery</span>");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Browse Inventory");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), "Browse Inventory");
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Cart");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), "Cart");
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Exit");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), "Exit");
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();
}