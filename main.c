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

#define LETTERS 96
#define FIRST_ASCII 32
#define DEFAULT_LENGHT 2;

typedef struct TNode {
    const char * translation;
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
            //free(root->translation);
        }
        free(root);
    }
}

int addTranslation(TNODE ** dictionary, const char * pattern, const char * translation) {
    TNODE * node;
    if (* dictionary == NULL) {
        * dictionary = mallocNode();
    }
    if (pattern[0] == '\0') return 0;
    node = *dictionary;
    unsigned int i, charIndex;
    for (i = 0; pattern[i] != '\0'; i++) {
        if (node->translation != NULL)
            return 0;
        charIndex = pattern[i] - FIRST_ASCII;
        if (node->child[charIndex] == NULL) {
            node->child[charIndex] = mallocNode();
        }
        node = node->child[charIndex];
    }
    if (node->translation != NULL)
        return 0;
    for (i = 0; i < LETTERS; i++) {
        if (node->child[i]) return 0;
    }

    node->translation = translation;

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

const char * findTranslation(const TNODE * dictionary, const char * pattern, unsigned int * index) {
    unsigned i, charIndex;
    for (i = 0; pattern[i] != '\0'; i++) {
        * index = i;
        charIndex = pattern[i] - FIRST_ASCII;
        dictionary = dictionary->child[charIndex];
        if (dictionary == NULL) return NULL;
        else if (dictionary->translation) break;
    }
    return dictionary->translation;
}

char * reallocString(char * string, unsigned lenght) {
    char * newString;
    newString = (char *) malloc(lenght + 1);
    strcpy(newString, string);
    free(string);
    return newString;
}

char * newSpeak(const char * text, const char * (*replace)[2]) {
    TNODE * dictionary;
    if (NULL == (dictionary = newDictionary(replace))) return NULL;

    unsigned int frontIndex = 0, tailIndex = 0, translationLenght = 0, maxLenght = DEFAULT_LENGHT;
    const char * match;
    char * translation = (char *) malloc(maxLenght + 1);
    translation[0] = '\0';
    for (frontIndex = 0; text[frontIndex] != '\0'; frontIndex++) {
        if (NULL != (match = findTranslation(dictionary, text + frontIndex, &tailIndex))) {
            frontIndex += tailIndex;
            translationLenght += strlen(match);
            if(translationLenght >= maxLenght) translation = reallocString(translation, maxLenght *= 2);
            strcat(translation, match);
        } else {
            if(translationLenght + 1 >= maxLenght) translation = reallocString(translation, maxLenght *= 2);
            translation[translationLenght++] = text[frontIndex];
            translation[translationLenght] = '\0';
        }
        //printf("front: %u\ttail: %u\tlenght: %u\n", frontIndex, tailIndex, translationLenght);
    }

    freeDictionary(dictionary);
    return translation;
}

#ifndef __PROGTEST__

int main(int argc, char * argv []) {
    char * res;
    unsigned i = 0;


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
    printf("%u succes: %d\n", i++, strcmp(res, "Everybody is happy.") == 0);
    free(res);
    
    res = newSpeak("The student answered an incorrect answer.", d1);
    printf("%u succes: %d\n", i++, strcmp(res, "The client answered an alternative answer.") == 0);
    free(res);
    
    res = newSpeak("He was dumb, his failure was expected.", d1);
    printf("%u succes: %d\n", i++, strcmp(res, "He was cerebrally challenged, his non-traditional success was expected.") == 0);
    free(res);

    res = newSpeak("The evil teacher became a murderer.", d1);
    printf("%u succes: %d\n", i++, strcmp(res, "The nicenest deprived voluntary knowledge conveyor became a termination specialist.") == 0);
    free(res);

    res = newSpeak("Devil's advocate.", d1);
    printf("%u succes: %d\n", i++, strcmp(res, "Dnicenest deprived's advocate.") == 0);
    free(res);

    res = newSpeak("Hello.", d2);
    //reference = NULL;
    printf("%u succes: %d\n", i++, res == NULL);

    const char * tbl0 [][2] = {
        { "a", "a"},
        { "", "suboptimal result"},
        { NULL, NULL}
    };

    res = newSpeak("fault", tbl0);
    printf("%u succes: %d\n", i++, res == NULL);

    const char * tbl1 [][2] = {
        { "foobar", "xyz"},
        { "foo", "abc"},
        { NULL, NULL}
    };
    res = newSpeak("bar", tbl1);
    printf("%u succes: %d\n", i++, res == NULL);

//    extern const char * searchString;
//    extern const char * replaceTable[][2];
//    extern const char * correctResult;
//    res = newSpeak(searchString, replaceTable);
//    printf("%u succes: %d\n", i++, strcmp(res, correctResult) == 0);
//    free(res);

    return 0;
}
#endif /* __PROGTEST__ */