#ifndef CART_CHECKOUT_H
#define CART_CHECKOUT_H

#include <stdbool.h>

#define MAX_ITEMS 1000 // Maximum number of items

//Inventory Item struct
typedef struct CartItem{
    char* name;
    float price;
    int quantity;
} CartItem;

extern int CartitemCount; // Counter for number of items
extern CartItem Cart[MAX_ITEMS]; // Array to store items



void addToCart(char* name, float price, int quantity);



#endif // CART_CHECKOUT_H