#include <stdio.h>
#include "StrList.h"
#define TRUE 1
#define FALSE 0
#define MAXWORDLEN 15


int main(){
    int operation;
    char *word = (char*) calloc(MAXWORDLEN+1 , sizeof(char));
    size_t wordsCount;
    int wordIndex;
    int freed = FALSE;
    StrList *Head = StrList_alloc(NULL);
    while (TRUE){
        scanf("%d", &operation);
        switch (operation)
        {
        case (1): //insert words
            scanf("%zd", &wordsCount);
            for (int i = 0; i < wordsCount; i++){
                scanf("%15s", word); //https://stackoverflow.com/a/55392269
                StrList_insertLast(Head, word);
                freed = FALSE;
            }
            break;
        case (2): //insert at an wordIndex
            scanf("%d", &wordIndex);
            scanf("%15s", word);
            StrList_insertAt(Head, word, wordIndex);
            break;
        case (3): //print all
            if (!freed){
                StrList_print(Head);
            }
            break;
        case (4): //print len
            printf("%ld\n", StrList_size(Head));
            break;
        case (5): //print a certain word based wordIndex
            scanf("%d", &wordIndex);
            StrList_printAt(Head, wordIndex);
            break;
        case (6): //total chars
            printf("%d\n", StrList_printLen(Head));
            break;
        case (7): //how many times word found
            scanf("%15s", word);
            printf("%d\n", StrList_count(Head, word));
            break;
        case (8): //remove word
            scanf("%15s", word);
            StrList_remove(Head, word);
            break;
        case (9): //remove word by wordIndex
            scanf("%d", &wordIndex);
            StrList_removeAt(Head, wordIndex);
            break;
        case (10): //reverse
            StrList_reverse(Head);
            break;
        case (11): //delete
            //simple free won't work because the next word will be hanging
            wordsCount = StrList_size(Head);
            for (int i = (int) wordsCount-1; i > 0; i--){ //removing from the end is more efficient
                StrList_removeAt(Head, i);
            }
            StrList_free(Head);
            freed = TRUE;
            break;
        case (12):
            StrList_sort(Head);
            break;
        case (13):
            if (StrList_isSorted(Head)){
                printf("true\n");
            } else {
                printf("false\n");
            }
            break;
        case (0):
            if (!freed){
                wordsCount = StrList_size(Head);
                for (int i = (int) wordsCount-1; i > 0; i--){ //removing from the end is more efficient
                    StrList_removeAt(Head, i);
                }
                StrList_free(Head);
            }
            free(word);
            exit(0);
        default:
            printf("unknown operation %d\n", operation);
            break;
        }
    }

}