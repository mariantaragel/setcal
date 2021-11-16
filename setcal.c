#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char **elements;
    int size;
    int capacity;    
} Set;

void set_ctor(Set *set)
{
    set->elements = NULL;
    set->size = 0;
    set->capacity = 0;
}

int read_set(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        fprintf(stderr, "Chyba!\n");
        return 0;
    }

    char c;
    while (c != '\n'){
        char element[31];
        int i;
        for (i = 0; (c = getc(file)) != (char)32; i++){
            if (i >= 30){
                fprintf(stderr, "Chyba!\n");
                return 0;
            }

            element[i] = c;
            if(c == '\n'){
                break;
            }
        }
        element[i] = '\0';

        printf("%s", element);
    }

    fclose(file);

    return 1;
}

int main(int argc, char **argv)
{
    if (argc != 2){
        fprintf(stderr, "Chyba 1!\n");
        return EXIT_FAILURE;
    }

    Set universe;
    set_ctor(&universe);
    if (!(read_set(argv[1]))){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
