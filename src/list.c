//
// Created by Richard Harman on 17. 11. 2022.
//

#include "list.h"

void List_Init( List *list) {
    list->firstElement = NULL;
    list->activeElement = NULL;
}
