#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ncurses.h>

#include "protocols.h"

void createDirectory() {
  struct stat st = {0};

  if(stat(DB_FOLDER_PATH, &st) == -1) {
    mkdir(DB_FOLDER_PATH, 0700);
  };

}

void createMainTablesFile() {

  createDirectory();

  FILE * ftables;
  ftables = fopen(TABLES_FILE_PATH, "r");
  if(ftables == NULL) {
    ftables = fopen(TABLES_FILE_PATH, "w");
  }

  fclose(ftables);

}

/**
 If the file exists, return 1.
 Else, return 0.
*/
int checkIfFileExist(char * path) {

  FILE * file;
  file = fopen(path, "r");

  if(file == NULL) {
    return 0;
  }

  return 1;

}

void executeForEachLine(void (*callback)(char *), FILE * file) {

  char buffer[1024];

  while(fscanf(file, "%[^\n] ", buffer) != EOF) {
    callback(buffer);
  }

}

void createTableFile(char * path) {

  FILE * file;
  file = fopen(path, "r");

  if(file == NULL) {
    file = fopen(path, "w");
  }

  fclose(file);

}

void appendRowInFile(char * path, char * str) {

  FILE * ftables;
  ftables = fopen(path, "a");

  if(ftables == NULL) {
    puts("Error when trying to open the file!");
    exit(3);
  }

  fputs(str, ftables);
  fputs("\n", ftables);

  fclose(ftables);

}

void setCursorToStartOfFile(FILE * file) {

  fseek(file, 0, SEEK_SET);

}

size_t countRowInFile(FILE * file) {

  size_t count = 1;

  char c;
  do {
    c = fgetc(file);
    if(c == '\n') count++;
  } while (c != EOF);

  return count;

}

/*
  Receives a filepath string and update target row using the msg string.
*/ 
void updateRowInFile(char * filepath, size_t target, char * msg) {

  FILE * file;
  FILE * ftemptable;
  file = fopen(filepath, "r");
  ftemptable = fopen("tempfile", "w");

  char buffer[1024];
  
  size_t count = countRowInFile(file);
  if(target > count - 1) {
    puts("Targeted row to be updated is out of bounds");
    exit(3);
  }

  setCursorToStartOfFile(file);

  size_t i = 0;
  while(fscanf(file, " %[^\n] ", buffer) != EOF)
  {

    if(i == target) {
      if(strcmp(msg, "skip") == 0){
        puts("skip");
        i++;
        continue;
      };
      fputs(msg, ftemptable);
    } else {
      fputs(buffer, ftemptable);
    }

    fputs("\n", ftemptable);
    i++;
  }

  fclose(file);
  fclose(ftemptable);

  if (rename("tempfile", filepath) != 0) {
    perror("Error renaming file");
    exit(EXIT_FAILURE);
}
}

int checkTableExistence(char * tableName) {

    FILE * file;
    file = fopen(TABLES_FILE_PATH, "r");
    if(file == NULL) return -1;

    char line[512];
    int count = 0;

    int position = 0;

    while(fgets(line, sizeof(line), file)) {
        char table[32];
        sscanf(line, "(%[^:]:", table);

        if(strcmp(table, tableName) == 0) {
          return position;
        }
        position = ftell(file);

        count++;
    }

    return -1;
}

Table * loadTableInfo() {

    FILE * ftable;
    FILE * tablesFile;
    char tableName[32];
    char path[64];
    char tableInfo[512];

    // input
    system("clear");
    puts("Enter table name:");
    scanf(" %s", tableName);

    // verification
    system("clear");
    int pos = checkTableExistence(tableName);
    if(pos == -1) {
        puts("Table not found");
        exit(3);
    }

    // loading table schema
    sprintf(path, "%s/%s", DB_FOLDER_PATH, tableName);
    tablesFile = fopen(TABLES_FILE_PATH, "r");
    fseek(tablesFile, pos, SEEK_SET);
    fscanf(tablesFile, "%[^\n] ", tableInfo);


    // loading table
    char fpath[64];
    int columnCount;
    char cols[256];
    char * next = cols;
    sscanf(tableInfo, "(%*[^:]:%[^:]:%d)%s", fpath, &columnCount, cols);

    ftable = fopen(fpath, "r");

    // placing collumns into array;
    Column * columns = malloc(sizeof(Column) * columnCount);

    for (int i = 0; i < columnCount; i++)
    {
        columns[i].name = malloc(sizeof(char) * 32);

        sscanf(next, "[%[^:]:%c]", columns[i].name, &columns[i].type);

        char * new = strchr(next, ']');
        if(new != NULL) {
            next = new + 1;
        }

    }

    Table * table = malloc(sizeof(Table));
    table->name = tableName;
    table->path = path;
    table->columnCount = columnCount;
    table->rawInfo = tableInfo;
    table->rawColumns = cols;
    table->columns = columns;
    table->file = ftable;

    fclose(tablesFile);

    return table;
}

Table * loadTableInfoByRawInfo(char * tableInfo) {

    FILE * ftable;
    Table * table = malloc(sizeof(Table));
    table->rawInfo = tableInfo;
    table->rawColumns = malloc(sizeof(char) * 256);
    table->path = malloc(sizeof(char) * 64);
    table->name = malloc(sizeof(char) * 32);

    sscanf(tableInfo, "(%[^:]:%[^:]:%d)%s", table->name, table->path, &table->columnCount, table->rawColumns);
    char * next = table->rawColumns;

    ftable = fopen(table->path, "r");

    Column * columns = malloc(sizeof(Column) * table->columnCount);
    for (int i = 0; i < table->columnCount; i++)
    {
        columns[i].name = malloc(sizeof(char) * 32);

        sscanf(next, "[%[^:]:%c]", columns[i].name, &columns[i].type);

        char * new = strchr(next, ']');
        if(new != NULL) {
            next = new + 1;
        }

    }

    table->columns = columns;
    table->file = ftable;

    return table;
}

// Get a array of options, display a menu to user select using arrow keys.
// Return the index of selected option.
int interactiveMenu(char ** options, int optionsCount, char * title) {

    initscr();
    noecho();
    start_color();
    use_default_colors();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    int currOption = 0;

    while(1) {
      attron(A_BOLD);
      printw("%s: \n", title);
      attroff(A_BOLD);

      for (int i = 0; i < optionsCount; i++)
      {
          if(i == currOption) {
              attron(COLOR_PAIR(1));
              attron(A_BOLD);
              printw(" - %s\n", options[i]);
              attroff(COLOR_PAIR(1));
              attroff(A_BOLD);
          } else {
              printw(" - %s\n", options[i]);
          }

      }
      printw("\nUse arrow keys to navigate and ENTER to select\n");

      refresh();

      // Get the input
      char c = getch();

      if(c == '\033') {
          getch();
          clear();
          switch(getch()) {
              case 'A':
                  if(currOption > 0) currOption--;
                  break;
              case 'B':
                  if(currOption < optionsCount - 1) currOption++;
                  break;
          }
      }
      else if(c == '\n') {
          clear();
          endwin();
          return currOption;
      }
      else {
          clear();
          refresh();
          continue;
      }

    }

}

void enviromentSetup() {

  createDirectory();
  createMainTablesFile();

}