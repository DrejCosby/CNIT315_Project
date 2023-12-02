#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>


/* Compile with this command: gcc -o DistanceMatrixAPI DistanceMatrixAPI.c -lcurl -ljson-c */

/* Takes a city as user input and return estimated drive time between it and West Lafayette, IN
Works for most cities I've tried so far but there have been a few exceptions. */

/* Also I had a main in this file initially and it just contained what DestinationRequest()
function contains which has caused an error that I haven't solved yet. */


// Struct to store duration information
struct DurationInfo {
    char text[50];
    int value;
};

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
        // Construct the API request URL
        char url[256];
        const char *api_key = "AIzaSyBEKS3cVPrNLTYYX6fUwtYbq0IgtEh1Svo"; // Replace with your actual API key
        const char *origin = "West+Lafayette,IN";

        sprintf(url, "https://maps.googleapis.com/maps/api/distancematrix/json?origins=%s&destinations=%s&key=%s",
                origin, destination, api_key);

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

void DestinationRequest() { //Needs to be GUI
    char destination[100];

    // Get user input for the destination
    printf("Enter the destination location: ");
    fgets(destination, sizeof(destination), stdin);

    // Remove the newline character from the input
    destination[strcspn(destination, "\n")] = 0;

    // Call the function to get travel time
    get_travel_time(destination);
}
