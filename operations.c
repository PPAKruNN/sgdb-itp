#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "protocols.h"

/*
    If table exists, return the number it occupies on the file (starting on 0).
    If doesn't exist, return -1.
*/
int checkTableExistence(char * tableName) {

    FILE * file;
    file = fopen(TABLES_FILE_PATH, "r");
    if(file == NULL) return 0;

    char line[512];
    int count = 0;

    fpos_t position;

    while(fgets(line, sizeof(line), file)) {
        char table[32];
        sscanf(line, "(%[^:]:", table);

        if(strcmp(table, tableName) == 0) return position.__pos;
        fgetpos(file, &position);

        count++;
    }

    return -1;
}

void printTable(char * tableInfo) {

    char tableName[32];
    char path[64];
    int columnCount;
    char colstring[256];
    char * next = colstring;

    char format[64] = "(%[^:]:%[^:]:%i)%s ";

    sscanf(tableInfo, format, tableName, path, &columnCount, colstring);

    puts("-----------------------------------------");
    printf("| Table name: %-25s |\n", tableName);
    puts("| Column                         | Type |");
    puts("-----------------------------------------");
    
    for (int i = 0; i < columnCount; i++)
    {
        char columnName[32];
        char type;

        sscanf(next, "[%[^:]:%c]", columnName, &type);

        char * endBracket = strchr(next, ']');
        if(endBracket) {
            next = endBracket + 1;
        }

        printf("| %-30s | %-4c |\n", columnName, type);
    }
    puts("-----------------------------------------");

}

void print() {
    char buffer[32];

    puts("Enter table name:");
    scanf("%32s", buffer);

    int pos = checkTableExistence(buffer);
    if(pos == -1) {
        puts("Table not found");
        exit(3);
    }

    FILE * tablesFile;
    tablesFile = fopen(TABLES_FILE_PATH, "r");
    
    char tableInfo[512];

    fseek(tablesFile, pos, SEEK_SET);
    fscanf(tablesFile, "%[^\n] ", tableInfo);

    printTable(tableInfo);

}

void createTable() {

    system("clear");
    puts("Table creation");
    puts("Enter table name:");

    // Check if table is unique.
    char tableName[32];
    scanf("%s", tableName);

    int exists = checkTableExistence(tableName);
    if(exists != -1) {
        printf("A table named \"%s\" already exists", tableName);
        exit(3);
    }

    int columnCount = 1;
    char buffer[256];
    char cols[256];

    char pkname[32];
    puts("First, give a name to the primary key column:");
    scanf("%32s", pkname);
    sprintf(cols, "[%s:P]", pkname);

    system("clear");
    puts("Now, enter the columns and types in the format: \"colum_name:type\":");
    puts("Available types:");
    puts("I - Integer");
    puts("F - Float");
    puts("D - Double");
    puts("C - Char");
    puts("S - String");
    puts("(press Ctrl+D or type \"quit\" to finish)");
    while (scanf("%s", buffer) != EOF)
    {
        system("clear");

        if(strcmp(buffer, "quit") == 0) break;

        char column[32];
        char type;

        int res = sscanf(buffer, "%[^:]:%c", column, &type);
        if(res != 2) {
            puts("Invalid format");
            puts("Format: \"colum_name:type\"");
            puts("Example: \"username:S\"");
            continue;
        } else {

            if(type != 'I' && type != 'F' && type != 'D' && type != 'C' && type != 'S') {
                system("clear");
                puts("Invalid type");
                puts("Available types:");
                puts("I - Integer");
                puts("F - Float");
                puts("D - Double");
                puts("C - Char");
                puts("S - String");
                continue;
            }

            printf("Column %i created! Column name: %s, type: %c\n", columnCount, column, type);

            columnCount++;

            char colbuf[38];
            sprintf(colbuf, "[%s:%c]", column, type);

            strcat(cols, colbuf);

        }
    }

    // Create table file.
    char path[64];
    sprintf(path, "%s/%s", DB_FOLDER_PATH, tableName);
    createTableFile(path);

    // Append table name to tables file.
    char tableInfo[512];

    sprintf(tableInfo, TABLE_INFO_SCHEMA_FORMAT, tableName, path, columnCount);
    strcat(tableInfo, cols);

    appendRowInFile(TABLES_FILE_PATH, tableInfo);

    puts("Table created!");
    printTable(tableInfo);
}

void listTables() {

    FILE * tablesFile;
    tablesFile = fopen(TABLES_FILE_PATH, "r");

    executeForEachLine(printTable, tablesFile);

}

void addRow() {

}