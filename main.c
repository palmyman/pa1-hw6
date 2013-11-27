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

#define LETTERS 71

typedef struct TNode {
    char * m_Dest;
    struct TNode * m_Child[LETTERS];
} TNODE;

TNODE * mallocNode(void) {
    TNODE * r = (TNODE *) malloc(sizeof ( *r));
    int i;
    r->m_Dest = NULL;
    for (i = 0; i < LETTERS; i++) {
        r->m_Child[i] = NULL;
    }
    return r;
}

int freeTree(TNODE * root) {    
    int i, childrens = 0;
    for (i = 0; i < LETTERS; i++) {
        if (root->m_Child[i]) {
            childrens++;
            if (freeTree(root->m_Child[i])) {
                root->m_Child[i] = NULL;
                childrens--;
            }
        }
    }
    if (!root->m_Dest && !childrens) {
        free(root);
        return 1;
    }
    return 0;
}

char * newSpeak(const char * text, const char * (*replace)[2]) {
    /* todo */
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