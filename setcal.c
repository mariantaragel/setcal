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
 *  Function to add element to set
 *
 * @param[in] set
 * @param[in] elem
 * @param[in] elem_length
 */

int add_element_to_set(Set *set, char* elem, int elem_length)
{
    for (int i = 0; i < set->cardinality; i++){
        if (strcmp(set->elements[i], elem) == 0){
            fprintf(stderr, "Element was already stored!\n");
            return 0;
        }
    }
    
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

    return 1;
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
 * Function prints complement of set
 *
 * @param set_number
 * @param set_list
 * @return 0 error, 1 - given set number is valid
 */

int set_complement(Set_list* set_list, int set_number)
{

    if (set_number > set_list->size){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    /// Universe
    char** universe_elems = set_list->sets[0].elements;
    int universe_size = set_list->sets[0].cardinality;

    /// Given set on line set_number
    char** given_set_elems = set_list->sets[set_number - 1].elements;
    int set_size = set_list->sets[set_number - 1].cardinality;


    int set_idx = 0;

    printf("S");
    for (int i = 0; i < universe_size; ++i) {
        if ( set_idx < set_size && strcmp(universe_elems[i], given_set_elems[set_idx]) == 0){
            set_idx++;
            continue;
        }
        else {
            printf(" %s", universe_elems[i]);
        }
    }
    printf("\n");
    return 1;
}

///  ======================================================================== ///

/**
 * Function print number of elements in set
 * 
 * @param set_list
 * @param set_number
 * @return 0 error, 1 - given set number is valid
 */
int set_card(Set_list *set_list, int set_number)
{
    if (set_number > set_list->size){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    printf("%d\n", set_list->sets[set_number - 1].cardinality);

    return 1;   
}

///  ======================================================================== ///

/**
 * Function print set on stdout
 * 
 * @param set set to print
 */
void print_set(Set_list *set_list, Set set)
{
    if (set_list->size == 1){
        printf("U");
    }
    else {
        printf("S");
    }

    if (set.cardinality == 0){
        printf("\n");
    }
    else {
        for (int i = 0; i < set.cardinality; i++){
            printf(" %s", set.elements[i]);
        }
        printf("\n");
    }
}

///  ======================================================================== ///

/**
 * Function print union of sets
 * 
 * @param set_list
 * @param set_number_1
 * @param set_number_2
 * @return 0 error, 1 - given set number is valid
 */
int union_of_sets(Set_list *set_list, int set_number_1, int set_number_2)
{
    if ((set_number_1 > set_list->size) || (set_number_2 > set_list->size)){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    int size_1 = set_list->sets[set_number_1 - 1].cardinality;
    char** set_1 = set_list->sets[set_number_1 - 1].elements;

    int size_2 = set_list->sets[set_number_2 - 1].cardinality;
    char** set_2 = set_list->sets[set_number_2 - 1].elements;


    printf("S");
    for (int i = 0; i < size_1; i++){
        printf(" %s", set_1[i]);
    }

    for (int i = 0; i < size_2; i++){
        int found = 0;
        for (int j = 0; j < size_1; j++){
            if (strcmp(set_2[i], set_1[j]) == 0){
                found = 1;
            }
        }
        if (!found){
            printf(" %s", set_2[i]);
        }        
    }
    printf("\n");
    
    return 1;
}

///  ======================================================================== ///

/**
 * Function to check syntax of element
 * 
 * @param[in] element - element to check
 * @param[in] set_list
 * @return 0 - element has wrong syntax, 1 in other case
 */
int check_element_syntax(char *element, Set_list set_list)
{
    char *command[] = {"empty", "card", "complement", "union",
    "intersect", "minus", "subseteq", "subset", "equals",
    "reflexive", "symmetric", "antisymmetric", "transitive",
    "function", "domain", "codomain", "injective",
    "surjective", "bijective", "true", "false"};
    
    for (int i = 0; element[i] != '\0'; i++){
        if (!(isalpha(element[i]))){
            fprintf(stderr, "Wrong element syntax!\n");
            return 0;
        }
    }

    for (int i = 0; i < 21; i++){
        if (strcmp(element, command[i]) == 0){
            fprintf(stderr, "Set contains identifier of command!\n");
            return 0;
        }
    }

    int match = 0;
    if (set_list.size > 0){
        for (int i = 0; i < set_list.sets[0].cardinality; i++){
            if (strcmp(element, set_list.sets[0].elements[i]) == 0){
                match = 1;
            }
        }
        if (!match){
            fprintf(stderr, "Error: Element %s isn't in universe\n", element);
            return 0;
        }
    }
    
    return 1;
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

    if (c != ' ' && c != '\n'){
        fprintf(stderr, "Wrong syntax of input file!\n");
        return 0;
    }

    if (c == '\n'){
        add_set_to_list(set_list, &new_set);
        print_set(set_list, new_set);
        return 1;
    }

    /// Skip all white spaces before first element
    while(isblank(c)){
        c = fgetc(file);
    }

    /// Return first non white space char to stream
    ungetc(c, file);

    while ((c = fgetc(file)) != '\n'){

        if (!isblank(c)){

            if (elem_idx >= 30){
                fprintf(stderr, "Wrong universe element!\n");
                return 0;
            }

            element[elem_idx] = c;
            elem_idx++;

        }
        else {
            element[elem_idx] = '\0';
            if (!(check_element_syntax(element, *set_list))){
                return 0;
            }
            if (!(add_element_to_set(&new_set, element, elem_idx))){
                return 0;
            }
            elem_idx = 0;
        }
    }

    /// TODO: repeated code on lines 298-304 and 310-316, make function (?)
    element[elem_idx] = '\0';
    if (!(check_element_syntax(element, *set_list))){
        return 0;
    }
    if (!(add_element_to_set(&new_set, element, elem_idx))){
        return 0;
    }
    add_set_to_list(set_list, &new_set);
    print_set(set_list, new_set);

    return 1;
}

///  ======================================================================== ///
/*
*   is arraz empty
*   return true or false
*/
int empty(char *c)
{   int result;
    reslut = strcmp(*c, '{}');
    if (result == 0)
    {
        return 1;
    }
    return 0;   
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
                if (!read_set(file, &set_list)){
                    return 0;
                }
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