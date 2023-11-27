#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "inventory.h"

extern int itemCount;
extern InvItem Inventory[];

void browseGUI();
void cartGUI();
static void on_button_clicked(GtkWidget *widget, gpointer data);
void on_add_to_cart_toggled(GtkCellRendererToggle *cell_renderer, gchar *path_str, GtkListStore *store);
void on_quantity_edited(GtkCellRendererText *cell_renderer, gchar *path_str, gchar *new_text, GtkListStore *store);

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
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkListStore *store;
    GtkTreeIter iter;

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

    // Create a list store and add it to the tree view
    store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT, G_TYPE_BOOLEAN);
    list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    // Create a scrolled window and add the list to it
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), list);

    // Add the scrolled window to the vbox
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    // Add Name column to tree
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    //Price column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Price", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    // Add "Quantity" column
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", TRUE, NULL);
    g_signal_connect(renderer, "edited", G_CALLBACK(on_quantity_edited), store);
    column = gtk_tree_view_column_new_with_attributes("Quantity", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);


    // Add "Add to cart" column
    renderer = gtk_cell_renderer_toggle_new();
    column = gtk_tree_view_column_new_with_attributes("Add to cart", renderer, "active", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    // Connect a signal handler for the "toggled" signal of the "Add to cart" buttons
    g_signal_connect(renderer, "toggled", G_CALLBACK(on_add_to_cart_toggled), store);


    // Add data to the list store
    for (int i = 0; i < itemCount; i++) {
        char price[50];
        sprintf(price, "$%.2f", Inventory[i].price);

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, Inventory[i].name, 1, price, 2, Inventory[i].quantity, -1);
    }
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

void on_quantity_edited(GtkCellRendererText *cell_renderer, gchar *path_str, gchar *new_text, GtkListStore *store)
{
    GtkTreeIter iter;
    GtkTreePath *path = gtk_tree_path_new_from_string(path_str);

    // Get edited iter
    gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);

    // Set new quantity
    gtk_list_store_set(store, &iter, 4, new_text, -1);

    // Do something with the new quantity (new_text)

    gtk_tree_path_free(path);
}

// Signal handler for the "toggled" signal of the "Add to cart" buttons
void on_add_to_cart_toggled(GtkCellRendererToggle *cell_renderer, gchar *path_str, GtkListStore *store)
{
    GtkTreeIter iter;
    GtkTreePath *path = gtk_tree_path_new_from_string(path_str);
    gboolean add_to_cart;

    // Get toggled iter
    gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, path);
    gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 3, &add_to_cart, -1);

    // Toggle "Add to cart" button
    add_to_cart = !add_to_cart;
    gtk_list_store_set(store, &iter, 3, add_to_cart, -1);

    // Do something with the "Add to cart" button state (add_to_cart)

    gtk_tree_path_free(path);
}