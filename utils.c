#include <stdio.h>
#include <stdlib.h>

void typedScanf(char type, char * target) {
    int qtd = 0;
    int i = 0;
    float f = 0;
    double d = 0;
    char c;

    switch (type)
    {
    case 'I':
        int qtd = scanf(" %i", &i);
        if(qtd == 0) {
            puts("Invalid input");
            exit(3);
        }
        sprintf(target, "%i", i);
        break;
    case 'F':
        qtd = scanf(" %f", &f);
        if(qtd == 0) {
            puts("Invalid input");
            exit(3);
        }
        sprintf(target, "%f", f);
        break;
    case 'D':
        qtd = scanf(" %lf", &d);
        if(qtd == 0) {
            puts("Invalid input");
            exit(3);
        }
        sprintf(target, "%lf", d);
        break;
    case 'C':
        qtd = scanf(" %c", &c);
        if(qtd == 0) {
            puts("Invalid input");
            exit(3);
        }
        sprintf(target, "%c", c);
        break;
    case 'S':
        qtd = scanf(" %[^\n]", (char *) target);
        if(qtd == 0) {
            puts("Invalid input");
            exit(3);
        }
        break;
    default:
        puts("Invalid type of information");
        exit(3);
        break;
    }
}