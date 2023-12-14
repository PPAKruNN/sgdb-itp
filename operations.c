#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "io.h"
#include "protocols.h"
#include "utils.h"

/*
    If table exists, return the number it occupies on the file (starting on 0).
    If doesn't exist, return -1.
*/
int tempCheckTableExistence(char * tableName) {

    FILE * file;
    file = fopen(TABLES_FILE_PATH, "r");
    if(file == NULL) return -1;

    char line[512];
    int count = 0;

    int position = 0;

    while(fgets(line, sizeof(line), file)) {
        char table[32];
        sscanf(line, "(%[^:]:", table);

        if(strcmp(table, tableName) == 0) return position; 
        ftell(file);

        count++;
    }

    return -1;
}

void printTableUsingTable(Table * table) {

    puts("-----------------------------------------");
    printf("| Table name: %-25s |\n", table->name);
    puts("-----------------------------------------");
    puts("| Column                         | Type |");
    puts("-----------------------------------------");
    
    for (int i = 0; i < table->columnCount; i++)
    {
        printf("| %-30s | %-4c |\n", table->columns[i].name, table->columns[i].type);
    }
    puts("-----------------------------------------");


}

void printTable(char * tableInfo) {

    Table * table = loadTableInfoByRawInfo(tableInfo);

    puts("-----------------------------------------");
    printf("| Table name: %-25s |\n", table->name);
    puts("-----------------------------------------");
    puts("| Column                         | Type |");
    puts("-----------------------------------------");
    
    for (int i = 0; i < table->columnCount; i++)
    {
        printf("| %-30s | %-4c |\n", table->columns[i].name, table->columns[i].type);
    }
    puts("-----------------------------------------");

}

void printTableValues(char * row) {

    char * next = row + 1;
    char value[32];
    
    while (sscanf(next, "%[^:]", value) == 1)
    {
        char * endBracket = strchr(next, ':');
        if (endBracket != NULL) {
            next = endBracket + 1;
        } else {
            sscanf(next, "%[^)]", value); 
            printf("| %-30s ", value);
            break;  
        }

        printf("| %-30s ", value);
    }

    puts("|");

}

void print() {

    Table * table = loadTableInfo();

    printTableUsingTable(table);

    fclose(table->file);
    free(table);

}

void createTable() {

    system("clear");
    puts("Table creation");
    puts("Enter table name:");

    // Check if table is unique.
    char tableName[32];
    scanf("%s", tableName);

    int exists = tempCheckTableExistence(tableName);
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

int searchColumn(Table * table, int columnIndex, Comparison action, char * target) {

    char * format = malloc(sizeof(char) * 256); 
    strcpy(format, "("); 

    char type = tolower(table->columns[columnIndex].type);

    if(type != 's' && type != 'c' && action == LIKE) {
        puts("Like comparison only works on string or char columns");
        exit(3);
    };

    for (int i = 0; i <= columnIndex; i++)
    {
        if(i == columnIndex) {
            if(columnIndex == table->columnCount - 1) {
                strcat(format, "%[^)]");
            } else {
                strcat(format, "%[^:]:");
            }
        } else {
            strcat(format, "%*[^:]:");
        }
    }

    system("clear");
    printTableUsingTable(table);

    char buffer[1024];

    puts("Found values: \n");
    int found = 0;

    while(fscanf(table->file, "%[^\n] ", buffer) != EOF) {

        char value[32];
        sscanf(buffer, format, value);

        switch (action)
        {
        case EQUALS:
            if(strcmp(value, target) == 0) {
                printTableValues(buffer);
                found++;
            }
            break;
        case GREATER:
            if(strcmp(value, target) > 0) {
                printTableValues(buffer);
                found++;
            }
            break;
        case GREATER_EQ:
            if(strcmp(value, target) >= 0) {
                printTableValues(buffer);
                found++;
            }
            break;
        case LESSER:
            if(strcmp(value, target) < 0) {
                printTableValues(buffer);
                found++;
            }
            break;
        case LESSER_EQ:
            if(strcmp(value, target) <= 0) {
                printTableValues(buffer);
                found++;
            }
            break;
        case LIKE:
            if(strstr(value, target) != NULL) {
                printTableValues(buffer);
                found++;
            }
            break;
        default:
            puts("Invalid action");
            break;
        }

    }

    printf("\nFound %i values\n", found);

    return found;
    free(format);

}

void addRowToTable() {

    Table * table = loadTableInfo();

    char row[1024] = "(";

    for (int i = 0; i < table->columnCount; i++)
    {
        system("clear");
        char value[32];
        char formated[33];

        if(table->columns[i].type == 'P') {
            puts("Enter value for Primary key \n(Column Type: Unique Unsigned Integer)");
            __u_int pk = 0;

            int qtd = scanf(" %u", &pk);
            if(qtd == 0) {
                puts("Invalid input");
                exit(3);
            }

            char pkstring[32];
            sprintf(pkstring, "%u", pk);

            int count = searchColumn(table, i, EQUALS, pkstring);
            if(count > 0) {
                system("clear");
                puts("Primary key already exists, cannot create row!");
                exit(3);
            }

            sprintf(value, "%u", pk);

            strcat(row, value);

            continue;
        };

        printf("Enter value for column %s: \n(Column Type: %c) \n", table->columns[i].name, table->columns[i].type);

        typedScanf(table->columns[i].type, value);

        sprintf(formated, ":%s", value);
        strcat(row, formated);

    }

    strcat(row, ")");

    appendRowInFile(table->path, row);

    fclose(table->file);

}

void readTable() {

    Table * table = loadTableInfo();

    printTableUsingTable(table);

    puts("\nTable Values: ");

    for (int i = 0; i < table->columnCount; i++)
    {
        printf("| %-30s ", table->columns[i].name);
    }
    puts("|");
    
    executeForEachLine(printTableValues, table->file);
}

void deleteRowFromTable() {

    Table * table = loadTableInfo();
    __u_int pk = 0;

    puts("Enter the primary key of the row you want to delete: ");
    int qtd = scanf("%u", &pk);
    if(qtd != 1) {
        puts("Invalid input");
        exit(3);
    }

    char pkstring[32];
    sprintf(pkstring, "%u", pk);

    int found = searchColumn(table, 0, EQUALS, pkstring);
    if(found == 0) {
        puts("Doesn't exist a row with this primary key");
        exit(3);
    }

    char * buffer = malloc(sizeof(char) * 1024);
    size_t count = 0;

    setCursorToStartOfFile(table->file);

    while(fscanf(table->file, "%[^\n] ", buffer) != EOF) {
        
        char value[32];
        sscanf(buffer, "(%[^:]", value);

        if(strcmp(value, pkstring) == 0) {
            fclose(table->file);
            updateRowInFile(table->path, count, "skip");
            break;
        }

        count++;
    }
}

void readTableColumn() {
    
    Table * table = loadTableInfo();

    printTableUsingTable(table);

    char ** columnsNames = malloc(sizeof(char *) * table->columnCount);
    for (int i = 0; i < table->columnCount; i++)
    {
        columnsNames[i] = table->columns[i].name;
    }
    
    char * title = "Choose one of the columns below to search on";
    int selected = interactiveMenu(columnsNames, table->columnCount, title);

    char * actions[] = {
        "Equals to",
        "Greater than",
        "Greater or equals than",
        "Lesser than",
        "Lesser or equals than",
        "Like (string only)"
    };

    int action = interactiveMenu(actions, 6, "Filter by: ");

    char * target = malloc(sizeof(char) * 32);
    puts("Enter target value: ");
    scanf("%s", target);

    searchColumn(table, selected, action, target);

}