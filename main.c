/* 
 * File:   main.c
 * Author: palmyman
 *
 * Created on November 27, 2013, 3:53 PM
 */

#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#endif /* __PROGTEST__ */

#define LETTERS 95
#define FIRSTASCII 32

typedef struct TNode {
    char * translation;
    struct TNode * child[LETTERS];
} TNODE;

TNODE * mallocNode(void) {
    TNODE * node = (TNODE *) malloc(sizeof ( *node));
    int i;
    node->translation = NULL;
    for (i = 0; i < LETTERS; i++) {
        node->child[i] = NULL;
    }
    return node;
}

void freeDictionary(TNODE * root) {
    int i;
    if (root) {
        for (i = 0; i < LETTERS; i++) {
            if (root->child[i])
                freeDictionary(root->child[i]);
        }
        if (root->child) {
            free(root->translation);
        }
        free(root);
    }
}

int addTranslation(TNODE ** dictionary, const char * pattern, const char * translation) {
    TNODE * node;
    if (* dictionary == NULL) {
        * dictionary = mallocNode();
    }
    node = *dictionary;
    int i, charIndex;
    for (i = 0; pattern[i] != '\0'; i++) {
        if (node->translation != NULL)
            return 0;
        charIndex = pattern[i] - FIRSTASCII;
        if (node->child[charIndex] == NULL) {
            node->child[charIndex] = mallocNode();
        }
        node = node->child[charIndex];
    }
    if (node->translation != NULL)
        return 0;
    node->translation = (char *) malloc(strlen(translation) + 1);
    strcpy(node->translation, translation);
    return 1;
}

TNODE * newDictionary(const char * (*replace)[2]) {
    TNODE * dictionary = NULL;
    unsigned int i = 0;
    while (replace[i][0] != NULL && replace[i][1] != NULL) {
        if (!addTranslation(&dictionary, replace[i][0], replace[i][1])) {
            freeDictionary(dictionary);
            return NULL;
        }
        i++;
    }
    return dictionary;
}

char * findTranslation(const TNODE * dictionary, const char * pattern) {
    int i, charIndex;
    for (i = 0; pattern[i] != '\0'; i++) {
        if (dictionary == NULL) return NULL;
        charIndex = pattern[i] - FIRSTASCII;
        dictionary = dictionary->child[charIndex];
    }
    return dictionary->translation;
}

char * newSpeak(const char * text, const char * (*replace)[2]) {
    TNODE * dictionary;
    if (NULL == (dictionary = newDictionary(replace))) return NULL;

    char * translation = NULL;
    freeDictionary(dictionary);
    return translation;
}

#ifndef __PROGTEST__

int main(int argc, char * argv []) {
    char * res;

    const char * d1 [][2] = {
        { "murderer", "termination specialist"},
        { "failure", "non-traditional success"},
        { "specialist", "person with certified level of knowledge"},
        { "dumb", "cerebrally challenged"},
        { "teacher", "voluntary knowledge conveyor"},
        { "evil", "nicenest deprived"},
        { "incorrect answer", "alternative answer"},
        { "student", "client"},
        { NULL, NULL}
    };

    const char * d2 [][2] = {
        { "fail", "suboptimal result"},
        { "failure", "non-traditional success"},
        { NULL, NULL}
    };


    res = newSpeak("Everybody is happy.", d1);
    /* res = "Everybody is happy."*/
    free(res);
    
    res = newSpeak("The student answered an incorrect answer.", d1);
    /* res = "The client answered an alternative answer."*/
    free(res);

    res = newSpeak("He was dumb, his failure was expected.", d1);
    /* res = "He was cerebrally challenged, his non-traditional success was expected."*/
    free(res);

    res = newSpeak("The evil teacher became a murderer.", d1);
    /* res = "The nicenest deprived voluntary knowledge conveyor became a termination specialist."*/
    free(res);

    res = newSpeak("Devil's advocate.", d1);
    /* res = "Dnicenest deprived's advocate."*/
    free(res);

    res = newSpeak("Hello.", d2);
    /* res = NULL */

    return 0;
}
#endif /* __PROGTEST__ */