#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inventory.h"

int main(){
    readCSV();
    printInventory(); //testing to see if I copied everything right


    cleanupInventory();//keep this function as the last thing to execute as it frees up the memory of the inventory names
    return 0;
}