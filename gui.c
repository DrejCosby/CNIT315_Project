#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "gui.h"

void browseGUI();
void cartGUI();
static void on_button_clicked(GtkWidget *widget, gpointer data);

int global_argc;
char **global_argv;

void mainGUI(int argc, char *argv[]){
    global_argc = argc;
    global_argv = argv;

    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *vbox;

    //idk what this does lol
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MartMart Grocery");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    //basically changes the color of the background
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "* { background-color: peachpuff; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    //create vbox and add it to the window
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    //add the large title text at the top of page
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span font='40' weight='bold'>MartMart Grocery</span>");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    //add the browse inventory button
    button = gtk_button_new_with_label("Browse Inventory");
    gtk_widget_set_size_request(button, 300, 50);
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), "Browse Inventory");
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(button, FALSE);
    gtk_widget_set_vexpand(button, FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 7);

    //add the cart button
    button = gtk_button_new_with_label("Cart");
    gtk_widget_set_size_request(button, 300, 50);
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), "Cart");
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(button, FALSE);
    gtk_widget_set_vexpand(button, FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 7);

    //add the exit button
    button = gtk_button_new_with_label("Exit");
    gtk_widget_set_size_request(button, 300, 50);
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), "Exit");
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(button, FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 7);

    //add a separator so that the buttons arent at the bottom of the page
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_css_provider_load_from_data(provider, "* { background-color: peachpuff; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(separator), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_box_pack_end(GTK_BOX(vbox), separator, TRUE, TRUE, 0);
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();
}

void browseGUI(){
    GtkWidget *window;
    GtkWidget *list;
    GtkWidget *button;
    GtkWidget *vbox;
    GtkWidget *hbox;

    // Create a new window for the inventory page
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Inventory");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "* { background-color: peachpuff; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

   vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
   gtk_container_add(GTK_CONTAINER(window), vbox);

   hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

   // Create a back button and add it to the left of the hbox
   button = gtk_button_new_with_label("<---");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), window);
   gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

   // Show the window
   gtk_widget_show_all(window);
}

//GUI for displaying the cart and items that the user has
void cartGUI(){
    GtkWidget *window;
    GtkWidget *list;
    GtkWidget *button;
    GtkWidget *vbox;
    GtkWidget *hbox;

    // Create a new window for the inventory page
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Cart");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "* { background-color: peachpuff; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

   vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
   gtk_container_add(GTK_CONTAINER(window), vbox);

   hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

   // Create a back button and add it to the left of the hbox
   button = gtk_button_new_with_label("<---");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), window);
   gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

   // Show the window
   gtk_widget_show_all(window);
}

static void on_button_clicked(GtkWidget *widget, gpointer data)
{
    //handling when each button is clicked
    g_print("%s button clicked\n", (char *)data);
    if (strcmp((char *)data, "Browse Inventory") == 0) {
        browseGUI();
    }
    else if (strcmp((char *)data, "Cart") == 0){
        cartGUI();
    }
    else if (strcmp((char *)data, "Exit") == 0) {
        gtk_main_quit();
    }
    else if (strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "<---") == 0){
        GtkWidget *window = (GtkWidget *)data;

        // Close the browse inventory window
        gtk_widget_destroy(window);

        // Open the main GUI page
        mainGUI(global_argc, global_argv);
    }
}