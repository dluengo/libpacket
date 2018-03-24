/* 
 This file is part of libpacket.

 libpacket is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 libpacket is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with libpacket.  If not, see <http://www.gnu.org/licenses/>
*/

#include <stdlib.h>

#include "libpacket/stack.h"

/*--------------------- StackItem methods --------------------------*/

StackItem_t * StackItem_createWithParams(
        StackItem_t *prev,
        StackItem_t *next,
        void *owner) {
    StackItem_t *item;

    item = malloc(sizeof(StackItem_t));
    if (item == NULL) {
        goto end;
    }

    item->prev = prev;
    item->next = next;
    item->owner = owner;

end:
    return item;
}

StackItem_t * StackItem_create() {
    return StackItem_createWithParams(NULL, NULL, NULL);
}

void StackItem_delete(StackItem_t *item) {
    free(item);
}

int StackItem_setOwner(StackItem_t *item, void *owner) {
    int ok = 0;

    if (item == NULL || owner == NULL) {
        goto end;
    }

    item->owner = owner;
    ok = 1;

end:
    return ok;
}

void * StackItem_getOwner(const StackItem_t *item) {
    return item != NULL? item->owner: NULL;
}

/*------------------------- Stack methods --------------------------*/

Stack_t * Stack_create() {
    Stack_t *stack;

    stack = malloc(sizeof(Stack_t));
    if (stack == NULL) {
        goto end;
    }

    stack->top = stack->bottom = NULL;
    stack->items = 0;

end:
    return stack;
}

void Stack_delete(Stack_t *stack) {
    StackItem_t *iter;

    if (stack != NULL) {
        for (iter = stack->bottom; iter != NULL; iter->next) {
            StackItem_delete(iter);
        }
    }

    free(stack);
}

int Stack_push(Stack_t *stack, StackItem_t *item) {
    int ret = 0;

    if (stack == NULL || item == NULL || Stack_itemExists(stack, item)) {
        goto end;
    }

    if (stack->items == 0) {
        item->prev = NULL;
        stack->bottom = stack->top = item;
    } else {
        item->prev = stack->top;
        stack->top->next = item;
        stack->top = item;
    }

    item->next = NULL;
    stack->items++;

    ret = 1;

end:
    return ret;
}

StackItem_t * Stack_pop(Stack_t *stack) {
    StackItem_t *item = NULL;

    if (stack == NULL || stack->items == 0) {
        goto end;
    }

    item = stack->top;
    stack->top = item->prev;
    if (stack->items > 1) {
        stack->top->next = NULL;
    }

    stack->items--;

end:
    return item;
}

unsigned int Stack_numItems(const Stack_t *stack) {
    unsigned int ret = 0;

    if (stack == NULL) {
        goto end;
    }

    ret = stack->items;

end:
    return ret;
}

int Stack_itemExists(const Stack_t *stack, StackItem_t *item) {
    StackItem_t *iter;
    int exists = 0;

    if (stack == NULL || item == NULL || stack->items == 0) {
        goto end;
    }

    for (iter = stack->bottom; iter != NULL; iter = iter->next) {
        if (iter == item) {
            exists = 1;
            break;
        }
    }

end:
    return exists;
}

int Stack_setOwner(Stack_t *stack, void *owner) {
    int ret = -1;

    if (stack != NULL) {
        stack->owner = owner;
        ret = 0;
    }

    return ret;
}

void * Stack_getOwner(const Stack_t *stack) {
    return stack != NULL? stack->owner: NULL;
}

