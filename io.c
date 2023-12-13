#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

  char buffer[512];
  
  size_t count = countRowInFile(file);
  if(target > count - 1) {
    puts("Targeted row to be updated is out of bounds");
    exit(3);
  }

  setCursorToStartOfFile(file);

  for (size_t i = 0; i < count - 1; i++)
  {
    fscanf(file, "%[^\n] ", buffer);
    printf("%lu", i);
    puts(buffer);


    if(i == target) {
      fputs(msg, ftemptable);
      puts("Linha alterada");
    } else {
      fputs(buffer, ftemptable);
      puts("Linha nao alterada");
    }

    fputs("\n", ftemptable);
  }

  fclose(file);
  fclose(ftemptable);

  rename("tempfile", filepath);
}

void enviromentSetup() {

  createDirectory();
  createMainTablesFile();

}