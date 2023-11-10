#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdbool.h>

#define MAX_ITEMS 1000 // Maximum number of items

//Inventory Item struct
typedef struct InvItem{
    char* name;
    float price;
    int quantity;
} InvItem;

extern InvItem items[MAX_ITEMS]; // Array to store items
extern int itemCount; // Counter for number of items

void printInventory();
void readCSV();
void cleanupInventory();


#endif // INVENTORY_H