
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inventory.h"

int itemCount = 0; // Initialize itemCount to 0
InvItem Inventory[MAX_ITEMS]; //array to store the inventory

//reads the inventory items csv and populates the InvItem struct with that data
void readCSV(){
    int successfulCopies = 0;
    char line[100];
    FILE *file = fopen("InventoryItems.csv", "r");
    if (file == NULL) {
        printf("Could not open file\n");
        return;
    }
    // Read and ignore the header line
    char header[100];
    fgets(header, sizeof(header), file);
    //read each line and create an inventory item for its data
    while (fgets(line, sizeof(line), file)){
        char* token = strtok(line, ",");
        char* name = malloc(50 * sizeof(char));

        if (name == NULL) {
            printf("Memory allocation failed\n");
            return;
        }
        
        strncpy(name, token, 50);
        name[49] = '\0';
        float price = atof(strtok(NULL,","));
        int quantity = atoi(strtok(NULL, ","));
        InvItem newItem = {name, price, quantity};
        Inventory[itemCount++] = newItem;
        successfulCopies++;
    }
    fclose(file);
    printf("Successfully copied %d records to Inventory from csv\n", successfulCopies);
}

//free the memory that was allocated for each of the names for the items
void cleanupInventory() {
    for(int i = 0; i < itemCount; i++){
        free(Inventory[i].name);
    }
}
