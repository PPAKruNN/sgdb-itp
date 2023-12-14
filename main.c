#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "operations.h"
#include "protocols.h"

void menu() {
    int option = 0;


    char * title = "Main menu";
    int count = 9;
    char ** options = (char **) malloc(sizeof(char *) * count);

    options[0] = "Create table";
    options[1] = "List all tables schema";
    options[2] = "Print one table schema";
    options[3] = "Add row to table";
    options[4] = "Read Table";
    options[5] = "Search on table";
    options[6] = "Delete row of table";
    options[7] = "Delete table";
    options[8] = "Exit";

    option = interactiveMenu(options, count, title);

    switch (option)
    {
    case 0:
        createTable();
        break;
    case 1:
        listTables();
        break;
    case 2:
        print();
        break;
    case 3:
        addRowToTable();
        break;
    case 4:
        readTable();
        break;
    case 5:
        readTableColumn();
        break;
    case 6:
        deleteRowFromTable();
        break;
    case 8:
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