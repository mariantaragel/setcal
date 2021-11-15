#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char **elements;
    int size;
    int capacity;    
} Set;


int main(int argc, char **argv)
{
    if (argc != 2){
        fprintf(stderr, "Chyba!\n");
        return EXIT_FAILURE;
    }

    Set universe;

    return EXIT_SUCCESS;
}