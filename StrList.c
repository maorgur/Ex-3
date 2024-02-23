#include "StrList.h"
#include <stdio.h>
#include <string.h>
#include <strings.h> //for https://pubs.opengroup.org/onlinepubs/9699919799/functions/strcasecmp.htmls
#define TRUE 1
#define FALSE 0

typedef struct _StrList {
    char *_data;
    struct _StrList * _next;
} StrList;


StrList* StrList_alloc(const char* data){
    //valgrind shows error if using malloc because of uninitialized pointers
    StrList* p= (StrList*)calloc(1, sizeof(StrList));
    if (p == NULL){
        printf("cannot allocate memory for StrList, exiting\n");
        exit(1);
    }
    if (data !=NULL){
        char *passedData = (char*) malloc(sizeof(char) * (strlen(data) + 1)); //copy string so it's not const
        if (passedData == NULL){
            printf("cannot allocate memory for passedData, exiting\n");
            exit(1);
        }
        passedData[strlen(data)] = '\0';
        strcpy(passedData, data);
        p->_data= passedData;
    }
    return p;
}

void StrList_free(StrList* listNode) {
    if (listNode != NULL){
        if (listNode -> _data != NULL) {
            free(listNode->_data);
        }
        free(listNode);
    }
}



size_t StrList_size(const StrList* listNode){
    size_t size = 1;
    if (listNode == NULL){
        return 0;
    }
    if (listNode -> _next){
        size += StrList_size(listNode->_next);
    }
    return size;

}

void StrList_insertLast(StrList* listNode,const char* data){
    if (listNode -> _data == NULL){
        char* temp = (char*) malloc(sizeof(char)*strlen(data)+1); //copy string so it's not const
        if (temp == NULL){
            printf("cannot allocate memory for the new word, exiting\n");
        }
        strcpy(temp, data);
        listNode -> _data = temp;
        return;
    }
    if ((listNode -> _next) == NULL){
        listNode -> _next = StrList_alloc(data);
    } else {
        StrList_insertLast(listNode -> _next, data);
    }
}

void StrList_insertAt(StrList* listNode, const char* data,int wordIndex){
    wordIndex -= 1;

    if (wordIndex < 0){

        StrList *newHead = StrList_alloc(data);
        newHead -> _next = listNode;
        return;
    } else if (wordIndex == 0){
        StrList *newHead = StrList_alloc(data);
        newHead ->_next = listNode -> _next;
        listNode -> _next = newHead;
    } else {
        StrList_insertAt(listNode -> _next, data, wordIndex -1);
    }
}

char* StrList_firstData(const StrList* listNode){
    return (char*) listNode -> _data;
}


void StrList_print(const StrList* listNode) {
    if (listNode != NULL)
    {
        printf("%s ", listNode -> _data);
        StrList_print(listNode -> _next);
    } else {
        printf("\n");
    }


}

void StrList_printAt(const StrList* listNode,int wordIndex) {
    if (!wordIndex){
        printf("%s\n", listNode -> _data);
    } else {
        StrList_printAt(listNode ->_next, wordIndex - 1);
    }
}

int StrList_printLen(const StrList* listNode) {
    if (listNode){
        if (listNode -> _data){
            return (int) strlen(listNode -> _data) + StrList_printLen(listNode -> _next);
        }
    }
    return 0;
}

int StrList_count(StrList* listNode, const char* data){
    int equal;
    if (strcmp(listNode -> _data, data) == 0){
        equal = 1; //if not equal, in both ways
    } else {
        equal = 0;
    }
    if (listNode -> _next){
        return (int) equal + StrList_count(listNode -> _next, data);
    } else {
        return equal;
    }
}
void StrList_remove(StrList* listNode, const char* data){
    if (strcmp(listNode -> _data, data) == 0){
        StrList_removeAt(listNode, 0);
    }
    if (listNode -> _next){
        if (listNode -> _next -> _data){
            if (strcmp(listNode -> _next -> _data, data) == 0){
                StrList_removeAt(listNode, 1);
            }
        }
        StrList_remove(listNode -> _next, data);
    }
}

void StrList_removeAt(StrList* listNode, int wordIndex){
    wordIndex -= 1;
    if (wordIndex < 0){
        StrList_free(listNode);
        return;
    }
    while (listNode -> _next && wordIndex)
    {
        listNode = listNode->_next;
        wordIndex --;
    }
    //free the node, and connect the next node
    StrList *nextStrList = listNode->_next->_next;
    StrList_free(listNode->_next);
    listNode -> _next = nextStrList;
}

int StrList_isEqual(const StrList* StrList1, const StrList* StrList2){
    if (StrList1 == StrList2){ //both NULL or pointers to the *exact* same word
        return 1;

    } else if (StrList1 == NULL || StrList2 == NULL){ //only one of them
        return 0;

    } else if (!strcmp(StrList1->_data, StrList2->_data)){
        return 0;
    } else {
        //recursively check next words
        return StrList_isEqual(StrList1->_next, StrList2->_next);
    }
}

//helping function to convert to a normal dynamic list - returns list of pointers
StrList** StrList_toList(StrList* listNode){
    StrList** StrList_list = (StrList**) malloc(sizeof(StrList*)*StrList_size(listNode));
    if (StrList_list == NULL){
        printf("cannot allocate memory for StrList_list, exiting");
        exit(1);
    }
    int wordIndex = 0;
    while (listNode){
        StrList_list[wordIndex] = listNode;
        wordIndex++;
        listNode = listNode -> _next;
    }
    return StrList_list;
}

StrList* StrList_clone(const StrList* listNode){
    StrList *newStrList = StrList_alloc(listNode -> _data);
    if (listNode -> _next){
        newStrList -> _next = StrList_clone(listNode ->_next);
    }
    return newStrList;
}

void StrList_reverse(StrList* listNode){
    int size = (int) StrList_size(listNode);
    StrList **StrList_list = StrList_toList(StrList_clone(listNode));
    for (int i=1; i<=size; i++){
        listNode->_data = StrList_list[size - i]->_data;
        listNode = listNode -> _next;
    }

    //free the reference
    for (int i = 0; i < size; i++){
        free(StrList_list[i]);
    }
    free(StrList_list);
}

void StrList_sort( StrList* listNode){
    //take the first value and move it to its correct location, then recursively call the next ones
    if (listNode -> _next == NULL){
        return;
    } else {
        if (strcmp(listNode -> _data, listNode -> _next -> _data) > 0){
            //swap
            char* tmp = listNode -> _data;
            listNode -> _data = listNode -> _next -> _data;
            listNode -> _next -> _data = tmp;
        }
        StrList_sort(listNode -> _next);
        StrList_sort(listNode -> _next);
    }
}

int StrList_isSorted(StrList* StrList){
    if (StrList -> _next == NULL){
        return TRUE;
    } else if (strcmp(StrList -> _data, StrList -> _next -> _data) > 0){
        return FALSE;
    } else {
        return StrList_isSorted(StrList -> _next);
    }
}