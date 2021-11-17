#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    char **elements;
    int cardinality;
} Set;

typedef struct{
    Set* sets;
    int size;
} Set_list;


/**
 * Constructor for set
 *
 * @param[in] set
 */

void set_ctor(Set *set)
{
    set->elements = NULL;
    set->cardinality = 0;
}

///  ======================================================================== ///

/**
 *  Function free resources for set
 *
 * @param[in] set
 */

void free_set(Set* set)
{
    for (int i = 0; i < set->cardinality; ++i) {
        free(set->elements[i]);
        set->elements[i] = NULL;
    }
    free(set->elements);
    set->elements = NULL;
    set->cardinality = 0;
}

///  ======================================================================== ///

/**
 * Constructor for list of sets
 *
 * @param[in] set_list
 */

void set_list_ctor(Set_list* set_list)
{
    set_list->sets = NULL;
    set_list->size = 0;
}

///  ======================================================================== ///

/**
 *  Function free resources for list of sets
 *
 * @param[in] set_list
 */

void free_set_list(Set_list* set_list)
{
    for (int i = 0; i < set_list->size; ++i) {
        free_set(&set_list->sets[i]);
    }
    free(set_list->sets);
    set_list->sets = NULL;
    set_list->size = 0;
}

///  ======================================================================== ///

/**
 *  Function add set to list
 *
 * @param[in] set_list
 * @param[in] new_set - set, that will be added to list
 */

void add_element_to_set(Set *set, char* elem, int elem_length)
{
    if (set->elements == NULL){
        set->elements = (char**) malloc(sizeof(char*));
        set->cardinality = 1;
        set->elements[0] = (char*) malloc(elem_length + 1);
    }
    else {
        set->cardinality++;
        set->elements = (char**) realloc(set->elements, sizeof(char**) * set->cardinality);
        set->elements[set->cardinality - 1] = (char*) malloc(elem_length + 1);
    }

    memcpy(set->elements[set->cardinality - 1], elem, elem_length + 1);
}

///  ======================================================================== ///

/**
 *  Function add set to list
 *
 * @param[in] set_list
 * @param[in] new_set - set, that will be added to list
 */

void add_set_to_list(Set_list* set_list, Set* new_set){

    if (set_list->sets == NULL){
        set_list->size = 1;
        set_list->sets = (Set*) malloc(sizeof(Set));
    }
    else {
        set_list->size++;
        set_list->sets = (Set*) realloc(set_list->sets, sizeof(Set) * set_list->size);
    }

    set_list->sets[set_list->size - 1] = *new_set;
}

///  ======================================================================== ///

/**
 *  Function parses options given in file
 *
 * @param[in] file - pointer to filestream
 * @param[in] set_list
 * @return    0 in case of error, 1 in other case
 */

int read_set(FILE* file, Set_list* set_list)
{
    char c = fgetc(file);
    char element[31] = {0};
    int elem_idx = 0;

    Set new_set;
    set_ctor(&new_set);

    /// Skip all white spaces before first element
    while(isblank(c)){
        c = fgetc(file);
    }

    /// Return first non white space char to stream
    ungetc(c, file);

    while ((c = fgetc(file)) != '\n'){

        if (!isblank(c)){

            if (elem_idx > 30){
                fprintf(stderr, "Wrong universe element!\n");
                return 0;
            }

            element[elem_idx] = c;
            elem_idx++;

        }
        else {
            element[elem_idx] = '\0';
            add_element_to_set(&new_set, element, elem_idx);
            elem_idx = 0;
            // TODO: make function that checks syntax of element

        }
    }

    /// TODO: repeated code on lines 121-122 and 130-131, make function (?)
    element[elem_idx] = '\0';
    add_element_to_set(&new_set, element, elem_idx);
    add_set_to_list(set_list, &new_set);


    return 1;
}

///  ======================================================================== ///

/**
 *  Function parses options given in file
 *
 * @param[in] filename
 * @return    0 - error, 1 - in other case
 */

int read_option(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL){
        fprintf(stderr, "Can't open %s.\n", filename);
        return 0;
    }

    Set_list set_list;
    set_list_ctor(&set_list);

    char c;
    while ((c = fgetc(file)) != EOF){

        if (isblank(c)){
            continue;
        }

        switch (c) {
            case 'U':{
                if (!read_set(file, &set_list)){
                    return 0;
                }
                break;
            }
            case 'S':{
                read_set(file, &set_list);
                break;
            }
            case 'R':{}
            case 'C':{}
            default:{} // wrong input
        }

    }

    free_set_list(&set_list);
    fclose(file);

    return 1;
}

int main(int argc, char **argv)
{
    if (argc != 2){
        fprintf(stderr, "Wrong number of arguments!\n");
        return EXIT_FAILURE;
    }

    if (!read_option(argv[1])){
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
