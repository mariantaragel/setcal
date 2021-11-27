/**
 * @name setcal.c
 * @details set and relation calculator
 * @authors Marian Taragel, Georgii Troitckii, Tomas Prokop
 * @date 26.11.2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    char **elements;
    int cardinality;
    int capacity;
    int position;
} Set;

typedef struct{
    Set* sets;
    int size;
    int capacity;
} Set_list;

typedef struct{
    char* first;
    char* second;
} Pair;

typedef struct{
    Pair* pairs;
    int number_of_pairs;
    int position;
    int capacity;
} Relation;

typedef struct{
    Relation* relations;
    int size;
    int capacity;
} Relation_list;

/// ======================================================================== ///

int str_comparator(const void* s1, const void* s2)
{
    return strcmp(*(const char**)s1, *(const char**)s2);
}

/// ======================================================================== ///

/**
 * Constructor for set
 *
 * @param[in] set
 */
void set_ctor(Set *set, int current_row)
{
    set->elements = NULL;
    set->cardinality = 0;
    set->capacity = 0;
    set->position = current_row;
}

/// ======================================================================== ///

/**
 *  Function free resources for set
 *
 * @param[in] set
 */
void free_set(Set* set)
{
    for (int i = 0; i < set->cardinality; ++i) {
        if (set->elements[i] != NULL){
            free(set->elements[i]);
            set->elements[i] = NULL;
        }
        set->elements[i] = NULL;
    }
    if (set->elements != NULL){
        free(set->elements);
        set->elements = NULL;
    }
    set->elements = NULL;
    set->cardinality = 0;
    set->capacity = 0;
    set->position = 0;
}

/// ======================================================================== ///

/**
 * Constructor for list of sets
 *
 * @param[in] set_list
 */
void set_list_ctor(Set_list* set_list)
{
    set_list->sets = NULL;
    set_list->capacity = 0;
    set_list->size = 0;
}

/// ======================================================================== ///

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
    if (set_list->sets != NULL) {
        free(set_list->sets);
        set_list->sets = NULL;
    }
    set_list->size = 0;
    set_list->capacity = 0;
}

/// ======================================================================== ///

/**
 * Constructor for pair
 *
 * @param[in] pair
 */
void pair_ctor(Pair *pair)
{
    pair->first = NULL;
    pair->second = NULL;
}

/// ======================================================================== ///

/**
 *  Function free resources for pair
 *
 * @param[in] pair
 */
void free_pair(Pair *pair)
{
    if ((pair->first != NULL) && (pair->second != NULL)){
        free(pair->first);
        pair->first = NULL;
        free(pair->second);
        pair->second = NULL;
    }
}

/// ======================================================================== ///

/**
 * Constructor for relation
 *
 * @param[in] relation
 */
void relation_ctor(Relation *relation, int current_row)
{
    relation->pairs = NULL;
    relation->number_of_pairs = 0;
    relation->capacity = 0;
    relation->position = current_row;
}

/// ======================================================================== ///

/**
 *  Function free resources for relation
 *
 * @param[in] relation
 */
void free_relation(Relation *relation)
{
    for (int i = 0; i < relation->number_of_pairs; i++){
        free_pair(&relation->pairs[i]);
    }

    if (relation->pairs != NULL) {
        free(relation->pairs);
        relation->pairs = NULL;
    }

    relation->pairs = 0;
    relation->position = 0;
}

/// ======================================================================== ///

/**
 * Constructor for list of pairs
 *
 * @param[in] relation_list
 */
void relation_list_ctor(Relation_list *relation_list)
{
    relation_list->relations = NULL;
    relation_list->size = 0;
    relation_list->capacity = 0;
}

/// ======================================================================== ///

/**
 *  Function free resources for relation_list
 *
 * @param[in] relation_list
 */
void free_relation_list(Relation_list *relation_list)
{
    for (int i = 0; i < relation_list->size; i++){
        free_relation(&relation_list->relations[i]);
    }

    if (relation_list->relations != NULL){
        free(relation_list->relations);
        relation_list->relations = NULL;
    }

    relation_list->size = 0;
    relation_list->capacity = 0;
}

/// ======================================================================== ///

/**
 *  Function to add element to set
 *
 * @param[in] set
 * @param[in] elem
 * @param[in] elem_length
 * @return 0 error, 1
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
        set->capacity = 10;
        set->elements = (char**) malloc(sizeof(char*) * set->capacity);
        set->cardinality = 1;
        set->elements[0] = (char*) malloc(elem_length + 1);
    }
    else {
        char** temp = NULL;
        set->cardinality++;

        if ( set->cardinality > set->capacity){
            set->capacity *= 2;
            temp = (char**) realloc(set->elements, sizeof(char*) * set->cardinality);
            if (temp == NULL) {
                free_set(set);
                fprintf(stderr, "Not enough memory!\n");
                return 0;
            }
            set->elements = temp;
        }
        set->elements[set->cardinality - 1] = (char*) malloc(elem_length + 1);
    }

    memcpy(set->elements[set->cardinality - 1], elem, elem_length + 1);

    return 1;
}

/// ======================================================================== ///

/**
 *  Function add set to list
 *
 * @param[in] set_list
 * @param[in] new_set set, that will be added to list
 */
int add_set_to_list(Set_list* set_list, Set* new_set)
{
    if (set_list->sets == NULL){
        set_list->size = 1;
        set_list->capacity = 10;
        set_list->sets = (Set*) malloc(sizeof(Set) * set_list->capacity);
    }
    else {
        Set* temp = NULL;
        set_list->size++;
        if ( set_list->size > set_list->capacity ){
            set_list->capacity *=2;
            temp = (Set*) realloc(set_list->sets, sizeof(Set) * set_list->capacity);
            if (temp == NULL){
                free_set_list(set_list);
                fprintf(stderr, "Not enough memory!\n");
                return 0;
            }
            set_list->sets = temp;
        }
    }
    set_list->sets[set_list->size - 1] = *new_set;
    return 1;
}

/// ======================================================================== ///

/**
 *  Function to add element to set
 *
 * @param[in] set
 * @param[in] elem
 * @param[in] elem_length
 */
int add_elements_to_pair(Pair *pair, char *first, char *second, int first_length, int second_length)
{
    if ((pair->first == NULL) && (pair->second == NULL)){
        pair->first = (char *) malloc(sizeof(char) * (first_length + 1));
        if (pair->first == NULL){
            fprintf(stderr, "Not enough memory!\n");
            return 0;
        }
        strcpy(pair->first, first);

        pair->second = (char *) malloc(sizeof(char) * (second_length + 1));
        if (pair->first == NULL){
            fprintf(stderr, "Not enough memory!\n");
            return 0;
        }
        strcpy(pair->second, second);
    }
    else {
        fprintf(stderr, "Not enough memory!\n");
        return 0;
    }

    return 1;
}

/// ======================================================================== ///

/**
 * Function add pair to relation
 *
 * @param[in] relation
 * @param[in] pair
 */
int add_pair_to_relation(Relation *relation, Pair *pair)
{
    if (relation->capacity == relation->number_of_pairs){
        if (relation->pairs == NULL){
            relation->capacity = 10;
            relation->pairs = (Pair *) malloc(sizeof(Pair) * relation->capacity);
            if (relation->pairs == NULL){
                fprintf(stderr, "Not enough memory!\n");
                return 0;
            }
        }
        else {
            relation->capacity += 5;
            Pair *pointer;
            pointer = (Pair *) realloc(relation->pairs, sizeof(Pair) * relation->number_of_pairs);
            if (pointer == NULL){
                fprintf(stderr, "Not enough memory!\n");
                return 0;
            }
            relation->pairs = pointer;
        }
    }

    relation->number_of_pairs++;
    relation->pairs[relation->number_of_pairs - 1] = *pair;

    return 1;
}

/// ======================================================================== ///

/**
 * Function add relation to list of relations
 * 
 * @param[in] relation_list
 * @param[in] relation relation, that will be added to list
 */
int add_relation_to_list(Relation_list *relation_list, Relation *relation)
{
    if (relation_list->capacity == relation_list->size){
        if (relation_list->relations == NULL){
            relation_list->capacity = 10;
            relation_list->relations = (Relation *) malloc(sizeof(Relation) * relation_list->capacity);
            if (relation_list->relations == NULL){
                fprintf(stderr, "Not enough memory!\n");
                return 0;
            }
        }
        else {
            relation_list->capacity += 5;
            Relation *pointer;
            pointer = (Relation *) realloc(relation_list->relations, sizeof(Relation) * relation_list->capacity);
            if (pointer == NULL){
                fprintf(stderr, "Not enough memory!\n");
                return 0;
            }
            relation_list->relations = pointer;
        }
    }
    relation_list->size++;
    relation_list->relations[relation_list->size - 1] = *relation;

    return 1;
}

/// ======================================================================== ///

/**
 * Function print set on stdout
 *
 * @param[in] set set to print
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

/// ======================================================================== ///

/**
 * Function print relation on stdout
 *
 * @param[in] relation relation to print
 */
void print_relation(Relation relation)
{
    if (relation.number_of_pairs == 0){
        printf("R\n");
    }
    else {
        printf("R");
        for (int i = 0; i < relation.number_of_pairs; i++){
            printf(" (%s %s)", relation.pairs[i].first, relation.pairs[i].second);
        }
        printf("\n");
    }
}

/// ======================================================================== ///

/**
 * Function find index of set on given row
 *
 * @param[in] set_list
 * @param[in] row
 * @return 0 - there isn't set on the row, 1 in other case
 */
int check_set_existence(Set_list* set_list, int* row)
{
    for (int i = 0; i < set_list->size; i++) {
        if (set_list->sets[i].position == *row){
            *row = i;
            return 1;
        }
    }
    return 0;
}

/// ======================================================================== ///

/**
 * Function find index of relation on given row
 *
 * @param[in] rel_list
 * @param[in] row
 * @return 0 - there isn't relation on the row, 1 in other case
 */
int check_relation_existence(Relation_list* rel_list, int* row)
{
    for (int i = 0; i < rel_list->size; ++i) {
        if (rel_list->relations[i].position == *row){
            *row = i;
            return 1;
        }
    }
    return 0;
}

/// ======================================================================== ///

/**
 * Function find index of relation on given row
 *
 * @param[in] relation_list
 * @param[in] row_number
 * @return 0 - there isn't relation on the row, 1 in other case
 */
int domain(Relation_list* relation_list, int row_number)
{
    if ( !check_relation_existence(relation_list, &row_number) ){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    int size = relation_list->relations[row_number].number_of_pairs;

    if (size == 0){
        printf("S\n");
        return 1;
    }

    Pair *pairs = relation_list->relations[row_number].pairs;
    char *elements[size];
    for (int i = 0; i < size; i++){
        elements[i] = pairs[i].first;
    }

    qsort(elements, size, sizeof(char*), str_comparator);

    printf("S");
    for (int i = 0; i < size; ++i) {
        while ((i < size - 1) && (strcmp(elements[i], elements[i + 1]) == 0)){
            i++;
        }
        printf(" %s", elements[i]);
    }
    printf("\n");

    return 1;
}

/// ======================================================================== ///
/**
 * Function compares two pairs in relation
 * 
 * @param[in] pair_1
 * @param[in] pair_2
 * @return 1 - pairs are identical, 0 - in other case
 */
int compare_pairs(Pair pair_1, Pair pair_2)
{
    int match_1 = strcmp(pair_1.first, pair_2.first);
    int match_2 = strcmp(pair_1.second, pair_2.second);
    if (match_1 || match_2){
        return 0;
    }
    
    return 1;
}

/// ======================================================================== ///

/**
 * Function prints:
 * true - relation is symmetric
 * false - in other case
 * 
 * @param[in] relation_list
 * @param[in] row_number
 * @return 0 - there isn't relation on the row, 1 in other case
 */
int is_symmetric(Relation_list *relation_list, int row_number)
{
    if (!check_relation_existence(relation_list, &row_number)){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    int size = relation_list->relations[row_number].number_of_pairs;

    if (size == 0){
        printf("true\n");
        return 1;
    }

    Pair *pairs = relation_list->relations[row_number].pairs;
    int match;
    for (int i = 0; i < size; i++){
        match = 0;
        Pair reverse_pair;
        reverse_pair.first = pairs[i].second;
        reverse_pair.second = pairs[i].first;
        
        for (int j = 0; j < size; j++){
            if (compare_pairs(reverse_pair, pairs[j])){
                match = 1;
                break;
            }
        }

        if (!match){
            printf("false\n");
            break;
        }
        
        if(i + 1 == size){
            printf("true\n");
            break;
        }
    }

    return 1;
}

/// ======================================================================== ///

/**
 * Function prints complement of set
 *
 * @param[in] set_number
 * @param[in] set_list
 * @return 0 - error, 1 - given set number is valid
 */
int set_complement(Set_list* set_list, int set_number)
{
    if ( !check_set_existence(set_list, &set_number) ){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    /// Universe
    char** universe_elems = set_list->sets[0].elements;
    int universe_size = set_list->sets[0].cardinality;

    /// Given set on line set_number
    char** given_set_elems = set_list->sets[set_number].elements;
    int set_size = set_list->sets[set_number].cardinality;

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

/// ======================================================================== ///

/**
 * Function print number of elements in set
 *
 * @param set_list
 * @param set_number
 * @return 0 error, 1 - given set number is valid
 */
int set_card(Set_list *set_list, int set_number)
{
    if ( !check_set_existence(set_list, &set_number) ){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    printf("%d\n", set_list->sets[set_number].cardinality);

    return 1;
}

/// ======================================================================== ///

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
    if (   !check_set_existence(set_list, &set_number_1)
           || !check_set_existence(set_list, &set_number_2)){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    char** first_set = set_list->sets[set_number_1].elements;
    int first_set_size = set_list->sets[set_number_1].cardinality;

    char** second_set = set_list->sets[set_number_2].elements;
    int second_set_size = set_list->sets[set_number_2].cardinality;


    printf("S");
    for (int i = 0; i < first_set_size; i++){
        printf(" %s", first_set[i]);
    }

    for (int i = 0; i < second_set_size; i++){
        int found = 0;
        for (int j = 0; j < first_set_size; j++){
            if (strcmp(second_set[i], first_set[j]) == 0){
                found = 1;
            }
        }
        if (!found){
            printf(" %s", second_set[i]);
        }
    }
    printf("\n");

    return 1;
}

/// ======================================================================== ///

/**
 * Function prints minus of 2 sets
 *
 * @param[in] set_list
 * @param[in] set_number_1
 * @param[in] set_number_2
 * @return 0 error, 1 - given set number is valid
 */
int minus_of_sets(Set_list *set_list, int set_number_1, int set_number_2)
{

    if (   !check_set_existence(set_list, &set_number_1)
           || !check_set_existence(set_list, &set_number_2)){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    char** first_set = set_list->sets[set_number_1].elements;
    int first_set_size = set_list->sets[set_number_1].cardinality;

    char** second_set = set_list->sets[set_number_2].elements;
    int second_set_size = set_list->sets[set_number_2].cardinality;

    int is_found = 0;


    int curr_s2_idx = 0;

    printf("S");

    for (int i = 0; i < first_set_size; ++i) {

        for (int j = curr_s2_idx; j < second_set_size && !is_found; ++j) {
            if ( strcmp(first_set[i], second_set[j]) == 0 ){
                is_found = 1;
                curr_s2_idx = j + 1;
            }
        }
        if (!is_found) {
            printf(" %s", first_set[i]);
        }
        is_found = 0;
    }

    printf("\n");

    return 1;
}


/// ======================================================================== ///

/**
 * Function prints:
 * true - set is subset of other set
 * false - in other case
 *
 * @param[in] set_list
 * @param[in] set_number_1
 * @param[in] set_number_2
 * @return 0 given set numbers are invalid, 1 input is correct
 */
int is_subset(Set_list *set_list, int set_number_1, int set_number_2)
{

    if (   !check_set_existence(set_list, &set_number_1)
           || !check_set_existence(set_list, &set_number_2)){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    char** first_set = set_list->sets[set_number_1].elements;
    int first_set_size = set_list->sets[set_number_1].cardinality;

    char** second_set = set_list->sets[set_number_2].elements;
    int second_set_size = set_list->sets[set_number_2].cardinality;

    /// Subset can't be greater than superset or equal to it
    if (first_set_size >= second_set_size){
        printf("false\n");
        return 1;
    }

    int has_elem = 0;
    int curr_set_idx = 0;

    for (int i = 0; i < first_set_size; ++i) {
        for (int j = curr_set_idx; j < second_set_size && !has_elem; ++j) {

            if ( strcmp(first_set[i], second_set[j]) == 0 ){
                has_elem = 1;
                curr_set_idx = j + 1;
            }
        }
        if (!has_elem){
            printf("false\n");
            return 1;
        }
        has_elem = 0;
    }

    printf("true\n");
    return 1;
}

/// ======================================================================== ///

/**
 * Function prints:
 * true - set is subseteq of other set
 * false - in other case
 *
 * @param[in] set_list
 * @param[in] set_number_1
 * @param[in] set_number_2
 * @return 0 given set numbers are invalid, 1 input is correct
 */
int is_subseteq(Set_list *set_list, int set_number_1, int set_number_2)
{
    if (   !check_set_existence(set_list, &set_number_1)
           || !check_set_existence(set_list, &set_number_2)){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    char** first_set = set_list->sets[set_number_1].elements;
    int first_set_size = set_list->sets[set_number_1].cardinality;

    char** second_set = set_list->sets[set_number_2].elements;
    int second_set_size = set_list->sets[set_number_2].cardinality;

    // Subseteq can't be greater than superset
    if (first_set_size > second_set_size){
        printf("false\n");
    }
    else if (first_set_size == 0){
        printf("true\n");
    }
    else {
        int found;
        int has_elem = 0;
        for (int i = 0; i < first_set_size; i++){
            found = 0;
            for (int j = 0; j < second_set_size; j++){
                if (strcmp(first_set[i], second_set[j])== 0){
                    found = 1;
                    has_elem++;
                    break;
                }
            }
            if (!found){
                printf("false\n");
                break;
            }
        }

        if (found){
            printf("true\n");
        }
    }

    return 1;
}

/// ======================================================================== ///

/**
 * Function prints:
 * true - set is empty
 * false - not empty
 *
 * @param[in] set_list
 * @param[in] set_number
 * @return 0 given set number is valid, 1 input is correct
 */
int is_set_empty(Set_list *set_list, int set_number)
{
    if ( !check_set_existence(set_list, &set_number) ){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    if (set_list->sets[set_number].cardinality == 0){
        printf("true\n");
    }
    else {
        printf("false\n");
    }

    return 1;
}

/// ======================================================================== ///

/**
* Prints true - equal, false - not equal
*
* @param[in] set_list
* @param[in] set_number_1
* @param[in] set_number_2
*/
int are_sets_equal(Set_list *set_list, int set_number_1, int set_number_2)
{
    if (!check_set_existence(set_list, &set_number_1)
           || !check_set_existence(set_list, &set_number_2)){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    char** first_set = set_list->sets[set_number_1].elements;
    int first_set_size = set_list->sets[set_number_1].cardinality;

    char** second_set = set_list->sets[set_number_2].elements;
    int second_set_size = set_list->sets[set_number_2].cardinality;

    int match = 1;
    if (first_set_size != second_set_size){
        printf("false\n");
    }
    else if ((first_set_size == 0) && (second_set_size == 0)){
        printf("true\n");
    }
    else {
        for (int i = 0; i < first_set_size; i++){
            if (strcmp(first_set[i], second_set[i]) != 0){
                printf("false\n");
                match = 0;
                break;
            }
            else {
                continue;
            }
        }
        if (match){
            printf("true\n");
        }
    }

    return 1;
}

/// ======================================================================== ///

/**
* Function prints intersect of 2 sets
*
* @param[in] set_list
* @param[in] set_number_1
* @param[in] set_number_2
*/
int intersect_of_sets(Set_list *set_list, int set_number_1, int set_number_2)
{
    if (   !check_set_existence(set_list, &set_number_1)
           || !check_set_existence(set_list, &set_number_2)){
        fprintf(stderr, "Can't step on nonexistent row!\n");
        return 0;
    }

    char** first_set = set_list->sets[set_number_1].elements;
    int first_set_size = set_list->sets[set_number_1].cardinality;

    char** second_set = set_list->sets[set_number_2].elements;
    int second_set_size = set_list->sets[set_number_2].cardinality;


    printf("S");

    for (int i = 0; i < first_set_size; i++){
        for (int j = 0; j < second_set_size; j++){
            if (strcmp(first_set[i], second_set[j]) == 0){
                printf(" %s", first_set[i]);
            }
        }
    }
    printf("\n");

    return 1;
}

/// ======================================================================== ///

/**
 * Function to check syntax of element
 *
 * @param[in] element - element to check
 * @param[in] set_list
 * @return 0 - element has wrong syntax, 1 in other case
 */
int check_element_syntax(char *element, Set_list* set_list)
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

    int found = 0;
    if (set_list->size > 0){
        for (int i = 0; i < set_list->sets[0].cardinality; i++){
            if (strcmp(element, set_list->sets[0].elements[i]) == 0){
                found = 1;
            }
        }
        if (!found){
            fprintf(stderr, "Error: Element %s isn't in universe\n", element);
            return 0;
        }
    }

    return 1;
}

/// ======================================================================== ///

/**
 *  Function parses options given in file
 *
 * @param file
 * @param set_list
 * @return 0 - command was wrong, 1 in other case
 */
int read_command(FILE *file, Set_list *set_list, Relation_list *relation_list)
{
    char *command[] = {"empty", "card", "complement", "union",
                       "intersect", "minus", "subseteq", "subset", "equals",
                       "reflexive", "symmetric", "antisymmetric", "transitive",
                       "function", "domain", "codomain", "injective",
                       "surjective", "bijective"};

    char c = fgetc(file);
    if (c != ' '){
        fprintf(stderr, "Wrong syntax of input file!\n");
        return 0;
    }

    /// Skip all spaced before command
    while (isblank(c)){
        c = fgetc(file);
    }

    char loaded_command[15];
    int index = 1;

    loaded_command[0] = c;

    while (((c = fgetc(file)) != ' ') && (c != '\n')){
        if (index > 14){
            fprintf(stderr, "Command %s doesn't exist\n", loaded_command);
            return 0;
        }
        if (!isblank(c)){
            loaded_command[index] = (char)c;
            index++;
        }
    }
    loaded_command[index] = '\0';

    int set_number_1 = 0;
    int set_number_2 = 0;
    fscanf(file, "%d", &set_number_1);
    if (!set_number_1){
        fprintf(stderr, "Too few arguments!\n");
        return 0;
    }
    fscanf(file, "%d", &set_number_2);
    if (set_number_2){
        while (isblank(c)){
            c = fgetc(file);
        }

        if ((c != '\n') && (c != EOF)){
            fprintf(stderr, "Too many arguments!\n");
            return 0;
        }
    }

    int i;
    for (i = 0; i < 19; i++){
        if (strcmp(loaded_command, command[i]) == 0){
            break;
        }
    }
    switch (i){
        case 0:{
            if (!is_set_empty(set_list, set_number_1)){
                return 0;
            }
            break;
        }
        case 1:{
            if (!set_card(set_list, set_number_1)){
                return 0;
            }
            break;
        }
        case 2:{
            if (!set_complement(set_list, set_number_1)){
                return 0;
            }
            break;
        }
        case 3:{
            if (!set_number_2){
                fprintf(stderr, "Too few arguments!\n");
                return 0;
            }
            if (!union_of_sets(set_list, set_number_1, set_number_2)){
                return 0;
            }
            break;
        }
        case 4:{
            if (!set_number_2){
                fprintf(stderr, "Too few arguments!\n");
                return 0;
            }
            if (!intersect_of_sets(set_list, set_number_1, set_number_2)){
                return 0;
            }
            break;
        }
        case 5:{
            if (!set_number_2){
                fprintf(stderr, "Too few arguments!\n");
                return 0;
            }
            if (!minus_of_sets(set_list, set_number_1, set_number_2)){
                return 0;
            }
            break;
        }
        case 6:{
            if (!set_number_2){
                fprintf(stderr, "Too few arguments!\n");
                return 0;
            }
            if (!is_subseteq(set_list, set_number_1, set_number_2)){
                return 0;
            }
            break;
        }
        case 7:{
            if (!set_number_2){
                fprintf(stderr, "Too few arguments!\n");
                return 0;
            }
            if (!is_subset(set_list, set_number_1, set_number_2)){
                return 0;
            }
            break;
        }
        case 8:{
            if (!set_number_2){
                fprintf(stderr, "Too few arguments!\n");
                return 0;
            }
            if (!are_sets_equal(set_list, set_number_1, set_number_2)){
                return 0;
            }
            break;
        }
        case 10:{
            if (!is_symmetric(relation_list, set_number_1)){
                return 0;
            }
            break;
        }
        case 14:{
            if (!domain(relation_list, set_number_1)){
                return 0;
            }
            break;
        }
        default:{
            fprintf(stderr, "Command %s doesn't exist\n", loaded_command);
            return 0;
        }
    }
    return 1;
}

/// ======================================================================== ///

/**
 *  Function parses options given in file
 *
 * @param[in] file - pointer to filestream
 * @param[in] rel_list
 * @return 0 in case of error, 1 in other case
 */
int read_relation(FILE *file, Relation_list *relation_list, int current_row)
{
    char c = fgetc(file);
    if (c != ' ' && c != '\n'){
        fprintf(stderr, "Wrong syntax of input file!\n");
        return 0;
    }

    Relation new_relation;
    relation_ctor(&new_relation, current_row);

    if (c == '\n'){
        add_relation_to_list(relation_list, &new_relation);
        print_relation(new_relation);
        return 1;
    }

    while ((c = fgetc(file)) != '\n'){
        if (isblank(c)){
            continue;
        }

        if (c != '('){
            fprintf(stderr, "Wrong syntax of input file!\n");
            return 0;
        }

        int index_elem_1 = 0;
        char element_1[31];
        while ((c = fgetc(file)) != ' '){
            element_1[index_elem_1] = c;
            index_elem_1++;
            if (index_elem_1 > 30){
                fprintf(stderr, "Wrong set element!\n");
                free_relation(&new_relation);
                return 0;
            }
        }
        element_1[index_elem_1] = '\0';

        int index_elem_2 = 0;
        char element_2[31];
        while ((c = fgetc(file)) != ')'){
            element_2[index_elem_2] = c;
            index_elem_2++;
            if (index_elem_2 > 30){
                fprintf(stderr, "Wrong set element!\n");
                free_relation(&new_relation);
                return 0;
            }
        }
        element_2[index_elem_2] = '\0';

        Pair new_pair;
        pair_ctor(&new_pair);

        if (!(add_elements_to_pair(&new_pair, element_1, element_2, index_elem_1, index_elem_2))){
            free_pair(&new_pair);
            return 0;
        }
        if (!(add_pair_to_relation(&new_relation, &new_pair))){
            free_relation(&new_relation);
            return 0;
        }
    }
    
    if (!(add_relation_to_list(relation_list, &new_relation))){
        free_relation(&new_relation);
        return 0;
    }

    print_relation(new_relation);
    
    return 1;
}

/// ======================================================================== ///

/**
 *  Function parses options given in file
 *
 * @param[in] file - pointer to filestream
 * @param[in] set_list
 * @return    0 in case of error, 1 in other case
 */
int read_set(FILE* file, Set_list* set_list, int current_row)
{
    char c = fgetc(file);
    char element[32] = {0};
    int elem_idx = 0;
    int last_elem_idx;

    Set new_set;
    set_ctor(&new_set, current_row);

    if (c != ' ' && c != '\n'){
        fprintf(stderr, "Wrong syntax of input file!\n");
        return 0;
    }

    if (c == '\n'){
        add_set_to_list(set_list, &new_set);
        print_set(set_list, new_set);
        return 1;
    }

    /// Read the first element from the file
    fscanf(file, "%31s", element);
    if (strlen(element) > 30){
        free_set(&new_set);
        fprintf(stderr, "Wrong set element!\n");
        return 0;
    }
    last_elem_idx = strlen(element);


    while ((c = fgetc(file)) != '\n'){
        if (isblank(c)){
            elem_idx = 0;
            continue;
        }

        if (elem_idx == 0){
            element[last_elem_idx] = '\0';
            if (!(check_element_syntax(element, set_list))){
                free_set(&new_set);
                return 0;
            }
            if (!(add_element_to_set(&new_set, element, last_elem_idx))){
                free_set(&new_set);
                return 0;
            }
        }

        if (elem_idx >= 30){
            fprintf(stderr, "Wrong set element!\n");
            free_set(&new_set);
            return 0;
        }

        element[elem_idx] = c;
        elem_idx++;
        last_elem_idx = elem_idx;
    }

    /// Add last element to set
    element[last_elem_idx] = '\0';
    if (!(check_element_syntax(element, set_list))){
        free_set(&new_set);
        return 0;
    }
    if (!(add_element_to_set(&new_set, element, last_elem_idx))){
        free_set(&new_set);
        return 0;
    }

    add_set_to_list(set_list, &new_set);

    /// Sort set elements in alphabetical order
    qsort(new_set.elements, new_set.cardinality, sizeof(char *), str_comparator);
    print_set(set_list, new_set);

    return 1;
}


/// ======================================================================== ///

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

    Relation_list relation_list;
    relation_list_ctor(&relation_list);

    int err_flag = 0;
    int current_row = 1;

    char c;
    while ((c = fgetc(file)) != EOF){

        if (err_flag){
            break;
        }

        if (isblank(c)){
            continue;
        }

        switch (c) {
            case 'U':{}
            case 'S':{
                if (!read_set(file, &set_list, current_row)){
                    err_flag = 1;
                }
                current_row++;
                break;
            }
            case 'R':{
                if (!read_relation(file, &relation_list, current_row)){
                    err_flag = 1;
                }
                current_row++;
                break;
            }
            case 'C':{
                if (!read_command(file, &set_list, &relation_list)){
                    err_flag = 1;
                }
                break;
            }
            default:{
                fprintf(stderr, "Bad option.\n");
                err_flag = 1;
            }
        }
    }

    free_set_list(&set_list);
    free_relation_list(&relation_list);
    fclose(file);

    if (err_flag){
        return 0;
    }

    return 1;
}

/// ======================================================================== ///

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
