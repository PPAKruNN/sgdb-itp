#include <stdio.h>
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

  FILE * ftables;
  ftables = fopen(TABLES_FILE_PATH, "r");
  if(ftables == NULL) {
    ftables = fopen(TABLES_FILE_PATH, "w");
  }

  fclose(ftables);

}

void appendRowMainTablesFile(char * str) {

  FILE * ftables;
  ftables = fopen(TABLES_FILE_PATH, "a");

  if(ftables == NULL) {
    puts("Erro ao abrir o arquivo de configuração de tabelas.");
    return;
  }

  fputs(str, ftables);
  fputs("\n", ftables);

  fclose(ftables);

}

void enviromentSetup() {

  createDirectory();
  createMainTablesFile();

}