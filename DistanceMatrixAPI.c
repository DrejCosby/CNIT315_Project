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
char address[160];


extern double total_price;
struct DistanceInfo distance_info;
struct DurationInfo duration_info;

/* Compile with this command: gcc -o DistanceMatrixAPI DistanceMatrixAPI.c -lcurl -ljson-c */

/* Takes a city as user input and return estimated drive time between it and West Lafayette, IN
Works for most cities I've tried so far but there have been a few exceptions. */


// Struct to store duration information
struct DurationInfo {
    char text[50];
    int value;
};

struct DistanceInfo {
    char text[50];
    int value;
};

void on_entry_activate(GtkEntry *entry, gpointer user_data) {
    gchar *text = g_strdup(gtk_entry_get_text(entry));

    printf("on_entry_activate called with user_data: %s\n", (char *)user_data);
    printf("Entered text: %s\n", text);

    // Store the entered text in the address global variables
    if (strcmp(user_data, "street_address") == 0) {
        strcpy(street_address, text);
    } else if (strcmp(user_data, "city") == 0) {
        strcpy(city, text);
    } else if (strcmp(user_data, "zip") == 0) {
        strcpy(zip, text);
    } else if (strcmp(user_data, "name") == 0){
        strcpy(name, text);
    }
    g_free(text);  // Free the duplicated string
}
//function to help with the processing of infomation for URL creation (generated by chatGPT)
void url_encode(char *source, char *target) {
    char *s = source;
    char *t = target;

    while (*s) {
        if (*s == ' ') {
            *t++ = '+';
        } else {
            *t++ = *s;
        }
        s++;
    }

    *t = '\0';  // Null-terminate the target string
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

    else if (strcmp(gtk_button_get_label(GTK_BUTTON(widget)), "Done") == 0){
        gtk_main_quit();
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
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_entry_activate), "street_address");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // For City
    label = gtk_label_new("City:");
    entry = gtk_entry_new();
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_entry_activate), "city");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // For State
    label = gtk_label_new("State: (XX)");
    entry = gtk_entry_new();
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_entry_activate), "state");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);

    // For Zip
    label = gtk_label_new("Zip:");
    entry = gtk_entry_new();
    g_signal_connect(G_OBJECT(entry), "activate", G_CALLBACK(on_entry_activate), "zip");
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

    // Extract distance information
    json_object *distance_object;
    if (json_object_object_get_ex(json_object_array_get_idx(elements_array, 0), "distance", &distance_object)) {
        strncpy(distance_info.text, json_object_get_string(json_object_object_get(distance_object, "text")), sizeof(distance_info.text) - 1);
        distance_info.value = json_object_get_int(json_object_object_get(distance_object, "value"));
    } else {
        fprintf(stderr, "Error extracting distance information from JSON response\n");
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
    // Construct the address from the global variables
    char street_address_encoded[200];
    char city_encoded[100];
    char state_encoded[20];
    char zip_encoded[20];
    // Replace spaces with '+'
    url_encode(street_address, street_address_encoded);
    url_encode(city, city_encoded);
    url_encode(state, state_encoded);
    url_encode(zip, zip_encoded);

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Construct the API request URL
        char url[256];
        const char *api_key = "AIzaSyBEKS3cVPrNLTYYX6fUwtYbq0IgtEh1Svo"; // Replace with your actual API key
        const char *origin = "West+Lafayette,IN";

        // Construct the address from the url encoded variables
        sprintf(address, "%s+%s+%s+%s", street_address_encoded, city_encoded, state_encoded, zip_encoded);
        printf("Constructed address: %s\n", address);
        sprintf(url, "https://maps.googleapis.com/maps/api/distancematrix/json?origins=%s&destinations=%s&units=imperial&key=%s",
            origin, address, api_key);
        printf("Constructed URL: %s\n", url);

        // Set the API request URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the write callback function to process the response
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &distance_info);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
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

    //Cart total price display
    label = gtk_label_new(NULL);
    char total_price_str[50];
    sprintf(total_price_str, "Cart Total Price: $%.2f", total_price);
    gtk_label_set_text(GTK_LABEL(label), total_price_str);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    //shipping rate display
    double shipping_rate = 2.29;
    label = gtk_label_new(NULL);
    char shipping_str[50];
    sprintf(shipping_str, "Standard Shipping Rate: $%.2f per mile", shipping_rate);
    gtk_label_set_text(GTK_LABEL(label), shipping_str);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    // Call get_travel_time function to get the distance information
    get_travel_time(address);

    // Create a new label for the distance
    label = gtk_label_new(NULL);
    char distance_str[50];
    sprintf(distance_str, "Distance: %s", distance_info.text);
    gtk_label_set_text(GTK_LABEL(label), distance_str);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    // Calculate shipping cost
    double miles;
    sscanf(distance_info.text, "%lf", &miles);
    double shipping_cost = miles * shipping_rate;
    
    // Create a new label for the shipping cost
    label = gtk_label_new(NULL);
    char shipping_cost_str[50];
    sprintf(shipping_cost_str, "Shipping Cost: $%.2f", shipping_cost);
    gtk_label_set_text(GTK_LABEL(label), shipping_cost_str);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    // Calculate grand total
    double grand_total = total_price + shipping_cost;

    // Create a new label for the grand total
    label = gtk_label_new(NULL);
    char grand_total_str[100];
    sprintf(grand_total_str, "<span font='15' weight='bold'>Grand Total: $%.2f</span>", grand_total);
    gtk_label_set_markup(GTK_LABEL(label), grand_total_str);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    button = gtk_button_new_with_label("Done");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_button_clicked_), window);
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window);
}
