#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "StringManip.h"

/*
 * Removes whitespace from the beginning and end of a string. Then returns
 * a pointer to that string.
**/
char *str_trim(char *str) {

    // Checks to see if str is empty.
    if(str == NULL) {
        return 0;
    }

    // Checks for and removes spaces at the beginning of the string.
    while (isspace((unsigned char) * str)) {
        str++;
    }

    // Checks for and removes spaces at the end of the String.
    char *end = str + strlen(str);
    while (end > str && isspace((unsigned char) * (end - 1))) {
        end--;
    }

    // Locks in the size of the modified string and then returns it.
    *end = '\0';
    return str;
}

/*
 * Selects a specific group of characters in an array, from index
 * "start" to index "end." 
**/
char *str_cut(char *str, int start, int end) {

    // Checks to see if the start and end of the function is valid.
    if(str == NULL || start < 0 || end < 0 || start > end || (int)end >= (int)strlen(str)) {
        printf("Error: invalid substring indexes.\n");
        return 0;
    }

    // Creates the sub string.
    char *sub_str = malloc(sizeof(char) * (end - start + 2));

    // Checks to see if malloc succeeded.
    if(sub_str == NULL) {
        printf("Error: memory allocation failed.");
        return 0;
    }

    for(int i = 0; i < end - start + 1; i++) {
        sub_str[i] = str[start + i];
    }

    // Adds null terminator at the end of the string.
    sub_str[end - start + 1] = '\0';

    return sub_str;
}