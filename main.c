#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "operations.h"
#include "protocols.h"

void menu() {
    int option = 0;

    puts("1. Create table");
    puts("2. List tables");
    puts("3. Print table");
    puts("4. Add row to table");
    puts("5. Exit");

    scanf("%i", &option);

    switch (option)
    {
    case 1:
        createTable();
        break;
    case 2:
        listTables();
        break;
    case 3:
        print();
        break;
    case 4:
        addRowToTable();
        break;
    case 5:
        exit(0);
        break;
    default:
        puts("Invalid option");
        break;
    }
}


int main() {
    enviromentSetup();

    menu();

    return 0;
}