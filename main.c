#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inventory.h"
#include "gui.h"


int main(int argc, char *argv[]){
    readCSV();
    mainGUI(argc, argv);
    printInventory(); //testing to see if I copied everything right


    cleanupInventory();//keep this function as the last thing to execute as it frees up the memory of the inventory names
    return 0;
}