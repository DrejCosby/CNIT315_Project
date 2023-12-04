#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <gtk/gtk.h>

void confirmGUI();

//address variables
char name[100];
char street_address[100];
char city[50];
char state[2];
char zip[10];

extern double total_price;

/* Compile with this command: gcc -o DistanceMatrixAPI DistanceMatrixAPI.c -lcurl -ljson-c */

/* Takes a city as user input and return estimated drive time between it and West Lafayette, IN
Works for most cities I've tried so far but there have been a few exceptions. */


// Struct to store duration information
struct DurationInfo {
    char text[50];
    int value;
};

void on_entry_activate(GtkEntry *entry, gpointer user_data) {
    gchar **destination = (gchar **)user_data;
    g_free(*destination);  // Free previous entry if any
    *destination = g_strdup(gtk_entry_get_text(entry));

    // Store the entered text in the address global variables
    if (strcmp(user_data, "street_address") == 0) {
        strcpy(street_address, *destination);
    } else if (strcmp(user_data, "city") == 0) {
        strcpy(city, *destination);
    } else if (strcmp(user_data, "zip") == 0) {
        strcpy(zip, *destination);
    } else if (strcmp(user_data, "name") == 0){
        strcpy(name, *destination);
    }
}


void on_button_clicked_(GtkButton *widget, gpointer data) {
    if (strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "<---") == 0){
        GtkWidget *window = (GtkWidget *)data;
        // Close the current window
        gtk_widget_destroy(window);
    }
    else if (strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Order") == 0){
        confirmGUI();
    }
}

void DestinationGUI(gchar **destination) {
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *box;  
    GtkWidget *button;
    GtkWidget *hbox;

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Checkout");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "* { background-color: peachpuff; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);


    // Create a back button and add it to the left of the hbox
    button = gtk_button_new_with_label("<---");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked_), window);
    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);

    //section header
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span font='20' weight='bold'>Personal Details:</span>");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    label = gtk_label_new("Full Name:");
    entry = gtk_entry_new();
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_entry_activate), (gpointer)&name);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    //section header
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span font='20' weight='bold'>Payment Information:</span>");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    label = gtk_label_new("Card Number:");
    entry = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    label = gtk_label_new("Expiration Date:");
    entry = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    label = gtk_label_new("CVV:");
    entry = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    //section header
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span font='20' weight='bold'>Shipping:</span>");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    // For Street Address
    label = gtk_label_new("Street Address:");
    entry = gtk_entry_new();
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_entry_activate), (gpointer)&street_address);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // For City
    label = gtk_label_new("City:");
    entry = gtk_entry_new();
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_entry_activate), (gpointer)&city);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // For State
    label = gtk_label_new("State: (XX)");
    entry = gtk_entry_new();
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_entry_activate), (gpointer)&state); // Assuming you have a 'state' variable
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // For Zip
    label = gtk_label_new("Zip:");
    entry = gtk_entry_new();
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_entry_activate), (gpointer)&zip);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Order");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked_), window);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);
}

/* Start of API stuff*/
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    struct DurationInfo *duration_info = (struct DurationInfo *)userp;

    // Parse the JSON response dynamically
    json_object *json_root = json_tokener_parse((char *)contents);

    // Check if the parsing was successful
    if (json_root == NULL) {
        fprintf(stderr, "Error parsing JSON response\n");
        return total_size;
    }

    // Extract duration information
    json_object *rows_array, *elements_array, *duration_object;
    if (json_object_object_get_ex(json_root, "rows", &rows_array) &&
        json_object_array_length(rows_array) > 0 &&
        json_object_object_get_ex(json_object_array_get_idx(rows_array, 0), "elements", &elements_array) &&
        json_object_array_length(elements_array) > 0 &&
        json_object_object_get_ex(json_object_array_get_idx(elements_array, 0), "duration", &duration_object)) {

        strncpy(duration_info->text, json_object_get_string(duration_object), sizeof(duration_info->text) - 1);
        duration_info->value = json_object_get_int(duration_object);
    } else {
        fprintf(stderr, "Error extracting duration information from JSON response\n");
    }

    // Cleanup
    json_object_put(json_root);

    return total_size;
}

void extract_time_unit(const char *duration_text) {
    // Extract the portion of duration_text that represents the time unit
    const char *time_unit = strstr(duration_text, " ");
    
    if (time_unit != NULL) {
        printf("Time Unit: %s\n", time_unit + 1); // +1 to skip the space
    } else {
        fprintf(stderr, "Error extracting time unit from duration text\n");
    }
}

void get_travel_time(const char *destination) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        char address[160];
        // Construct the API request URL
        char url[256];
        const char *api_key = "AIzaSyBEKS3cVPrNLTYYX6fUwtYbq0IgtEh1Svo"; // Replace with your actual API key
        const char *origin = "West+Lafayette,IN";

        // Construct the address from the global variables
        sprintf(address, "%s, %s, %s", street_address, city, zip);
        sprintf(url, "https://maps.googleapis.com/maps/api/distancematrix/json?origins=%s&destinations=%s&key=%s",
                origin, address, api_key);

        // Set the API request URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the write callback function to process the response
        struct DurationInfo duration_info;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &duration_info);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Print the dynamically parsed duration information
            printf("Estimated Arrival Time: %s\n", duration_info.text);
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void confirmGUI() {
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *box;  
    GtkWidget *button;
    GtkWidget *hbox;

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Confimation");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "* { background-color: peachpuff; }", -1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    // header
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span font='30' weight='bold'>Thank you for your order!</span>");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    label = gtk_label_new(NULL);
    char total_price_str[50];
    sprintf(total_price_str, "Total Price: $%.2f", total_price);
    gtk_label_set_text(GTK_LABEL(label), total_price_str);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window);
}
