#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inventory.h"
#include "gui.h"
#include "DistanceMatrixAPI.h"

int main(int argc, char *argv[]){
    readCSV(); //read the csv and copy all items into Inventory array struct
    mainGUI(argc, argv);
    
    cleanupInventory();//keep this function as the last thing to execute as it frees up the memory of the inventory names
    return 0;
}