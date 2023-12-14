#ifndef IO_H
#define IO_H

#include "protocols.h"

void createDirectory();
void enviromentSetup();
void createMainTablesFile();
void createTableFile(char * path);
void appendRowInFile(char * path, char * str);
void updateRowInFile(char * filepath, size_t target, char * msg);
void executeForEachLine(void (*callback)(char *), FILE * file);
Table * loadTableInfo();
Table * loadTableInfoByRawInfo(char * tableInfo);
int interactiveMenu(char ** options, int optionsCount, char * title);


#endif