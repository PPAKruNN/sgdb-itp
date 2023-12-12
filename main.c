#include <stdio.h>

#include "io.h"
#include "operations.h"
#include "protocols.h"

int main() {
    enviromentSetup();

    printTable("(aaaaaaa:db/aaaaaaa:3)[PK:P][aaaa:b][bbbb:a]");

    return 0;
}