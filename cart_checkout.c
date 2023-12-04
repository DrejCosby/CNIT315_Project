#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "gtk/gtk.h"
#include "gui.h"
#include "inventory.h"
#include "cart_checkout.h"

int CartitemCount = 0;
CartItem Cart[MAX_ITEMS]; //array to store the cart items

void addToCart(char* name, float price, int quantity){
    // Check if the cart is full
    if (CartitemCount >= MAX_ITEMS) {
        printf("Cart is full\n");
        return;
    }

    // Create a new cart item
    CartItem newItem;
    newItem.name = strdup(name); // Duplicate the string to avoid memory issues
    newItem.price = price;
    newItem.quantity = quantity;

    // Add the item to the cart
    Cart[CartitemCount++] = newItem;
}