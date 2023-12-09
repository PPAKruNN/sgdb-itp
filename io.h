#ifndef IO_H
#define IO_H

void createDirectory();
void enviromentSetup();
void createMainTablesFile();
void createTableFile(char path[256]);
void appendRowInFile(char path[256], char * str);
void updateRowInFile(char filepath[256], size_t target, char msg[256]);


#endif