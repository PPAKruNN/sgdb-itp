#include <stdio.h>

#include "io.h"

int main() {
    enviromentSetup();
    char * str = "oia gente";
    appendRowMainTablesFile(str);
    return 0;
}